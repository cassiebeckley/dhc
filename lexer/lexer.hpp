#ifndef DHC_LEXER_LEXER_HPP
#define DHC_LEXER_LEXER_HPP

#include <graft/scanner.hpp>

#include <graft/pattern.hpp>
#include <graft/match.hpp>

#include <graft/pattern/character.hpp>
#include <graft/pattern/property.hpp>
#include <graft/pattern/choice.hpp>
#include <graft/pattern/compound.hpp>
#include <graft/pattern/exclude.hpp>
#include <graft/pattern/repetition.hpp>
#include <graft/pattern/string.hpp>

#include "digit.hpp"
#include "qualified.hpp"
#include "integer.hpp"
#include "lit_float.hpp"
#include "lit_char.hpp"
#include "lit_string.hpp"

#include <memory>
#include <sstream>
#include <unordered_map>

#include <unicode/unistr.h>

namespace dhc {
    namespace lexer {
        typedef std::shared_ptr<graft::pattern::pattern> pattern_ptr;
        typedef std::shared_ptr<graft::match::match> match_ptr;

        /**
         * An enumeration of all possible token types.
         */
        enum class type {
            NONE = -1,
            WHITESPACE,
            QVARID,
            QCONID,
            QVARSYM,
            QCONSYM,
            LITERAL,
            SPECIAL,
            RESERVEDOP,
            RESERVEDID
        };

        /**
         * \brief A lexical analyzer for Haskell 2010.
         *
         * Like, stuff
         */
        class lexer {
            public:
                /**
                 * \brief Initialize the lexical analyzer
                 * 
                 * Creates a lexical analyzer conforming to the Haskell
                 * 2010 specification.
                 * @param source the source code to analyze
                 */
                lexer(icu::UnicodeString source) : s(source)
                {
                    // TODO: get rid of this mebbe?
                    using namespace graft::pattern;

                    typenames[static_cast<int>(type::NONE)] = "none";
                    typenames[static_cast<int>(type::WHITESPACE)] = "whitespace";
                    typenames[static_cast<int>(type::QVARID)] = "qvarid";
                    typenames[static_cast<int>(type::QCONID)] = "qconid";
                    typenames[static_cast<int>(type::QVARSYM)] = "qvarsym";
                    typenames[static_cast<int>(type::QCONSYM)] = "qconsym";
                    typenames[static_cast<int>(type::LITERAL)] = "literal";
                    typenames[static_cast<int>(type::SPECIAL)] = "special";
                    typenames[static_cast<int>(type::RESERVEDOP)] = "reservedop";
                    typenames[static_cast<int>(type::RESERVEDID)] = "reservedid";

                    match_func digit_h = [](match_ptr m) {
                        UChar32 c = m->flatten()[0];
                        int32_t d = u_charDigitValue(c);
                        return std::make_shared<dhc::lexer::digit>(m->column, m->type, c, d);
                    };

                    digit = std::make_shared<property>("[:Nd:]", -1, digit_h);
                    octit = std::make_shared<property>("[0-7]", -1, digit_h);

                    hexit = std::make_shared<choice>(std::vector<pattern_ptr> {
                        digit,
                        std::make_shared<property>("[A-F]", -1, [] (match_ptr m) {
                            char c = m->flatten()[0];
                            return std::make_shared<dhc::lexer::digit>(m->column, m->type, c, c - 'A');
                        }),
                        std::make_shared<property>("[a-f]", -1, [] (match_ptr m) {
                            char c = m->flatten()[0];
                            return std::make_shared<dhc::lexer::digit>(m->column, m->type, c, c - 'a');
                        })
                    });

                    special = std::make_shared<property>("[\\(\\),;\\[\\]`\\{\\}]", static_cast<int>(type::SPECIAL));

                    symbol = std::make_shared<exclude>(
                        std::make_shared<property>("[[:Punctuation:][:Symbol:]]"),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            special,
                            std::make_shared<property>("[_\"']")
                        })
                    );

                    large = std::make_shared<property>("[[:Lu:][:Lt:]]");
                    small = std::make_shared<property>("[[:Ll:]_]");

                    graphic = std::make_shared<choice>(std::vector<pattern_ptr> {
                        small,
                        large,
                        symbol,
                        digit,
                        special,
                        std::make_shared<character>('"'),
                        std::make_shared<character>('\'')
                    });

                    carriage_return = std::make_shared<character>('\r');
                    linefeed = std::make_shared<character>('\n');
                    vertab = std::make_shared<character>('\v');
                    formfeed = std::make_shared<character>('\f');
                    space = std::make_shared<character>(' ');
                    tab = std::make_shared<character>('\t');

                    any = std::make_shared<choice>(std::vector<pattern_ptr> {
                        graphic,
                        space,
                        tab
                    });

                    newline = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            carriage_return,
                            linefeed
                        }),
                        carriage_return,
                        linefeed,
                        formfeed
                    }, -1, [&] (match_ptr m) {
                        s.newline();
                        return m;
                    });

                    uniWhite = std::make_shared<property>("[:White_Space:]");

                    whitechar = std::make_shared<choice>(std::vector<pattern_ptr> {
                        uniWhite
                    });

                    opencom = std::make_shared<string>("{-");
                    closecom = std::make_shared<string>("-}");

                    dashes = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<string>("--"),
                        std::make_shared<repetition>(std::make_shared<character>('-'))
                    });

                    ANY = std::make_shared<choice>(std::vector<pattern_ptr> {
                        graphic,
                        whitechar
                    });

                    ANY_seq = std::make_shared<repetition>(
                        std::make_shared<exclude>(ANY, std::make_shared<choice>(std::vector<pattern_ptr> {
                            opencom,
                            closecom
                        }))
                    );

                    comment = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                dashes,
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<exclude>(any, symbol),
                                    std::make_shared<repetition>(any)
                                }),
                                newline
                            }),
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                dashes,
                                newline
                            })
                        }),
                    });

                    std::shared_ptr<compound> ncm = std::make_shared<compound>(std::vector<pattern_ptr> {
                        opencom,
                        ANY_seq
                    });

                    ncm->add_pattern(std::make_shared<repetition>(
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            ncm,
                            ANY_seq
                        })
                    ));
                    ncm->add_pattern(closecom);

                    ncomment = ncm;

                    whitestuff = std::make_shared<choice>(std::vector<pattern_ptr> {
                        whitechar,
                        comment,
                        ncomment
                    });

                    whitespace = std::make_shared<compound>(std::vector<pattern_ptr> {
                        whitestuff,
                        std::make_shared<repetition>(whitestuff)
                    }, static_cast<int>(type::WHITESPACE));

                    std::function<match_func(int)> from_base = [] (int base)
                    {
                        return [base] (match_ptr m)
                        {
                            std::vector<match_ptr> children = m->children();
                            std::shared_ptr<dhc::lexer::digit> head = std::dynamic_pointer_cast<dhc::lexer::digit>(children[0]);
                            int64_t dec = head->d;

                            auto tail = children[1]->children();

                            for (auto it = tail.begin(); it != tail.end(); ++it)
                            {
                                std::shared_ptr<dhc::lexer::digit> c = std::dynamic_pointer_cast<dhc::lexer::digit>(*it);
                                dec *= base;
                                dec += c->d;
                            }

                            return std::make_shared<dhc::lexer::integer>(m->column, m->type, m->length(), dec);
                        };
                    };

                    decimal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        digit,
                        std::make_shared<repetition>(digit)
                    }, -1, from_base(10));

                    octal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        octit,
                        std::make_shared<repetition>(octit)
                    }, -1, from_base(8));

                    hexadecimal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        hexit,
                        std::make_shared<repetition>(hexit)
                    }, -1, from_base(16));

                    integer = std::make_shared<choice>(std::vector<pattern_ptr> {
                        decimal,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<character>('0'),
                                std::make_shared<choice>(std::vector<pattern_ptr> {
                                    std::make_shared<character>('o'),
                                    std::make_shared<character>('O')
                                })
                            }),
                            octal
                        }, -1, [] (match_ptr m) {
                            return m->children()[1];
                        }),
                        // TODO:
                        // Hm? Double check below
                        // Find out if it's weird stuff
                        // or if it can be removed
                        //
                        // sidenote: what was I even thinking
                        // when I wrote the above? The below
                        // is necessary for hexadecimal lexing
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<character>('0'),
                                std::make_shared<choice>(std::vector<pattern_ptr> {
                                    std::make_shared<character>('x'),
                                    std::make_shared<character>('X')
                                })
                            }),
                            hexadecimal
                        }, -1, [] (match_ptr m) {
                            return m->children()[1];
                        })
                    });

                    exponent = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<character>('e'),
                                std::make_shared<character>('E')
                            }),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<character>('+'),
                                std::make_shared<character>('-')
                            }),
                            decimal
                        }, -1, [] (match_ptr m) {
                            auto children = m->children();
                            std::shared_ptr<dhc::graft::match::character> sign = std::dynamic_pointer_cast<dhc::graft::match::character>(children[1]);
                            std::shared_ptr<dhc::lexer::integer> d = std::dynamic_pointer_cast<dhc::lexer::integer>(children[2]);
                            auto value = d->data;
                            if (sign->data == '-')
                                value = -value;
                            return std::make_shared<dhc::lexer::integer>(m->column, m->type, m->length(), value);
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<character>('e'),
                                std::make_shared<character>('E')
                            }),
                            decimal
                        }, -1, [] (match_ptr m) {
                            auto children = m->children();
                            std::shared_ptr<dhc::lexer::integer> d = std::dynamic_pointer_cast<dhc::lexer::integer>(children[1]);
                            auto value = d->data;
                            auto a = std::make_shared<dhc::lexer::integer>(m->column, m->type, m->length(), value);
                            return a;
                        }),
                    });

                    floating = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            decimal,
                            std::make_shared<character>('.'),
                            decimal
                        }, -1, [] (match_ptr m) {
                            auto children = m->children();
                            std::shared_ptr<dhc::lexer::integer> in = std::dynamic_pointer_cast<dhc::lexer::integer>(children[0]);
                            std::shared_ptr<dhc::lexer::integer> fr = std::dynamic_pointer_cast<dhc::lexer::integer>(children[2]);
                            return std::make_shared<dhc::lexer::lit_float>(m->column, m->type, m->length(), in->data, fr->data, 1);
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            decimal,
                            std::make_shared<character>('.'),
                            decimal,
                            exponent
                        }, -1, [] (match_ptr m) {
                            auto children = m->children();
                            std::shared_ptr<dhc::lexer::integer> in = std::dynamic_pointer_cast<dhc::lexer::integer>(children[0]);
                            std::shared_ptr<dhc::lexer::integer> fr = std::dynamic_pointer_cast<dhc::lexer::integer>(children[2]);
                            std::shared_ptr<dhc::lexer::integer> ex = std::dynamic_pointer_cast<dhc::lexer::integer>(children[3]);
                            return std::make_shared<dhc::lexer::lit_float>(m->column, m->type, m->length(), in->data, fr->data, ex->data);

                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            decimal,
                            exponent
                        }, -1, [] (match_ptr m) {
                            auto children = m->children();
                            auto in = std::dynamic_pointer_cast<dhc::lexer::integer>(children[0]);
                            auto ex = std::dynamic_pointer_cast<dhc::lexer::integer>(children[1]);
                            return std::make_shared<dhc::lexer::lit_float>(m->column, m->type, m->length(), in->data, 0, ex->data);

                        })
                    });

                    charesc = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<character>('a'),
                        std::make_shared<character>('b'),
                        std::make_shared<character>('f'),
                        std::make_shared<character>('n'),
                        std::make_shared<character>('r'),
                        std::make_shared<character>('t'),
                        std::make_shared<character>('v'),
                        std::make_shared<character>('\\'),
                        std::make_shared<character>('"'),
                        std::make_shared<character>('\''),
                        std::make_shared<character>('&')
                    });

                    cntrl = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<property>("[A-Z]"),
                        std::make_shared<character>('@'),
                        std::make_shared<character>('['),
                        std::make_shared<character>('\\'),
                        std::make_shared<character>(']'),
                        std::make_shared<character>('^'),
                        std::make_shared<character>('_')
                    });

                    ascii = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<character>('^'),
                            cntrl
                        }),
                        std::make_shared<string>("NUL"),
                        std::make_shared<string>("SOH"),
                        std::make_shared<string>("STX"),
                        std::make_shared<string>("ETX"),
                        std::make_shared<string>("EOT"),
                        std::make_shared<string>("ENQ"),
                        std::make_shared<string>("ACK"),
                        std::make_shared<string>("BEL"),
                        std::make_shared<string>("BS"),
                        std::make_shared<string>("HT"),
                        std::make_shared<string>("LF"),
                        std::make_shared<string>("VT"),
                        std::make_shared<string>("FF"),
                        std::make_shared<string>("CR"),
                        std::make_shared<string>("SO"),
                        std::make_shared<string>("SI"),
                        std::make_shared<string>("DLE"),
                        std::make_shared<string>("DC1"),
                        std::make_shared<string>("DC2"),
                        std::make_shared<string>("DC3"),
                        std::make_shared<string>("DC4"),
                        std::make_shared<string>("NAK"),
                        std::make_shared<string>("SYN"),
                        std::make_shared<string>("ETB"),
                        std::make_shared<string>("CAN"),
                        std::make_shared<string>("EM"),
                        std::make_shared<string>("SUB"),
                        std::make_shared<string>("ESC"),
                        std::make_shared<string>("FS"),
                        std::make_shared<string>("GS"),
                        std::make_shared<string>("RS"),
                        std::make_shared<string>("US"),
                        std::make_shared<string>("SP"),
                        std::make_shared<string>("DEL")
                    });

                    escape = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<character>('\\'),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            charesc,
                            ascii,
                            decimal,
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<character>('o'),
                                octal
                            }),
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<character>('x'),
                                hexadecimal
                            })
                        })
                    });

                    character_literal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<character>('\''),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<exclude>(graphic, std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<character>('\''),
                                std::make_shared<character>('\\')
                            })),
                            space,
                            std::make_shared<exclude>(escape, std::make_shared<string>("\\&"))
                        }),
                        std::make_shared<character>('\'')
                    }, -1, [] (match_ptr m) {
                        auto children = m->children();
                        auto middle = children[1]->flatten();
                        return std::make_shared<lit_char>(m->column, m->type, middle);
                    });

                    gap = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<character>('\\'),
                        whitechar,
                        std::make_shared<repetition>(whitechar),
                        std::make_shared<character>('\\')
                    });

                    string_literal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<character>('"'),
                        std::make_shared<repetition>(std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<exclude>(graphic, std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<character>('"'),
                                std::make_shared<character>('\\')
                            })),
                            space,
                            escape,
                            gap
                        }), -1, [] (match_ptr m) {
                            auto children = m->children();
                            std::vector<std::shared_ptr<lit_char>> chars;
                            for (auto it = children.begin(); it != children.end(); ++it)
                            {
                                chars.push_back(std::make_shared<lit_char>((*it)->column, (*it)->type, (*it)->flatten()));
                            }
                            return std::make_shared<lit_string>(m->column, m->type, chars);
                        }),
                        std::make_shared<character>('"')
                    }, -1, [] (match_ptr m) {
                        return m->children()[1];
                    });

                    literal = std::make_shared<choice>(std::vector<pattern_ptr> {
                        integer,
                        floating,
                        character_literal,
                        string_literal
                    }, static_cast<int>(type::LITERAL));

                    reservedid = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<string>("case"),
                        std::make_shared<string>("class"),
                        std::make_shared<string>("data"),
                        std::make_shared<string>("default"),
                        std::make_shared<string>("deriving"),
                        std::make_shared<string>("do"),
                        std::make_shared<string>("else"),
                        std::make_shared<string>("foreign"),
                        std::make_shared<string>("if"),
                        std::make_shared<string>("import"),
                        std::make_shared<string>("in"),
                        std::make_shared<string>("infix"),
                        std::make_shared<string>("infix1"),
                        std::make_shared<string>("infixr"),
                        std::make_shared<string>("instance"),
                        std::make_shared<string>("let"),
                        std::make_shared<string>("module"),
                        std::make_shared<string>("newtype"),
                        std::make_shared<string>("of"),
                        std::make_shared<string>("then"),
                        std::make_shared<string>("type"),
                        std::make_shared<string>("where"),
                        std::make_shared<character>('_')
                    }, static_cast<int>(type::RESERVEDID)); 

                    match_func flatten = [] (match_ptr m) {
                        return std::make_shared<graft::match::string>(m->column, m->type, m->flatten());
                    };

                    varid = std::make_shared<exclude>(
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            small,
                            std::make_shared<repetition>(std::make_shared<choice>(std::vector<pattern_ptr> {
                                small,
                                large,
                                digit,
                                std::make_shared<character>('\'')
                            }))
                        }, -1, flatten
                        ), reservedid
                    );

                    conid = std::make_shared<compound>(std::vector<pattern_ptr> {
                        large,
                        std::make_shared<repetition>(std::make_shared<choice>(std::vector<pattern_ptr> {
                            small,
                            large,
                            digit,
                            std::make_shared<character>('\'')
                        }))
                    }, -1, flatten);

                    reservedop = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<string>(".."),
                        std::make_shared<character>(':'),
                        std::make_shared<string>("::"),
                        std::make_shared<character>('='),
                        std::make_shared<character>('\\'),
                        std::make_shared<character>('|'),
                        std::make_shared<string>("<-"),
                        std::make_shared<string>("->"),
                        std::make_shared<character>('@'),
                        std::make_shared<character>('~'),
                        std::make_shared<string>("=>")
                    }, static_cast<int>(type::RESERVEDOP));

                    varsym = std::make_shared<exclude>(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<exclude>(symbol, std::make_shared<character>(':')),
                        std::make_shared<repetition>(symbol)
                    }, -1, flatten 
                    ), std::make_shared<choice>(std::vector<pattern_ptr> {
                        reservedop,
                        dashes
                    }));

                    consym = std::make_shared<exclude>(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<character>(':'),
                        std::make_shared<repetition>(symbol)
                    }, -1, flatten), reservedop);

                    modid = std::make_shared<compound>(std::vector<pattern_ptr> {
                        conid,
                        std::make_shared<repetition>(
                            std::make_shared<compound>(
                                std::vector<pattern_ptr> {
                                    std::make_shared<character>('.'),
                                    conid
                                }, -1, [] (match_ptr m) {
                                    return m->children()[1];
                                }
                        ), -1, [] (match_ptr m) {
                            auto mods = m->children();
                            std::vector<icu::UnicodeString> res;
                            for (auto it = mods.begin(); it != mods.end(); ++it)
                            {
                                res.push_back((*it)->flatten());
                            }
                            return std::make_shared<qualified>(m->column, m->type, res);
                        })
                    }, -1, [] (match_ptr m) {
                        auto children = m->children();
                        icu::UnicodeString conid = children[0]->flatten();
                        auto second = std::dynamic_pointer_cast<qualified>(children[1]);
                        auto mods = second->modules;
                        mods.insert(mods.begin(), conid);
                        return std::make_shared<qualified>(m->column, m->type, mods);
                    });

                    match_func qualify = [](match_ptr m) {
                        auto children = m->children();
                        auto mods = std::dynamic_pointer_cast<qualified>(children[0])->modules;
                        icu::UnicodeString id = children[2]->flatten();
                        mods.push_back(id);
                        return std::make_shared<qualified>(m->column, m->type, mods);
                    };

                    qvarid = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            modid,
                            std::make_shared<character>('.'),
                            varid
                        }, -1, qualify),
                        varid
                    }, static_cast<int>(type::QVARID));

                    qconid = std::make_shared<choice>(std::vector<pattern_ptr> {
                        modid
                    }, static_cast<int>(type::QCONID));

                    qvarsym = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            modid,
                            std::make_shared<character>('.'),
                            varsym
                        }, -1, qualify),
                        varsym
                    }, static_cast<int>(type::QVARSYM));

                    qconsym = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            modid,
                            std::make_shared<character>('.'),
                            consym
                        }, -1, qualify),
                        consym
                    }, static_cast<int>(type::QCONSYM));

                    lexeme = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qvarid,
                        qconid,
                        qvarsym,
                        qconsym,
                        literal,
                        special,
                        reservedop,
                        reservedid
                    });

                    program = std::make_shared<choice>(std::vector<pattern_ptr> {
                        lexeme,
                        whitespace
                    });
                }

                /**
                 * \brief Get the next token.
                 *
                 * Returns the next token from the source code.
                 * @return The match object representing the lexeme found.
                 *         nullptr is returned if an error has occurred.
                 */
                match_ptr next();

                /**
                 * \brief Checks if the end of the source has been reached.
                 * @return A boolean value representing whether or not lexical
                 *         analysis is finished.
                 */
                bool finished();

                /**
                 * \brief Returns an error message.
                 *
                 * Creates an error message to be used if next()
                 * returned an error value.
                 * @param filename the name of the source file
                 * @return An error message
                 */
                std::string error(std::string filename);

                /**
                 * \brief Maps each token type to a string representing it.
                 */
                std::unordered_map<int, std::string> typenames;
            private:
                pattern_ptr digit;
                pattern_ptr octit;
                pattern_ptr hexit;
                pattern_ptr symbol;
                pattern_ptr large;
                pattern_ptr small;
                pattern_ptr special;
                pattern_ptr graphic;
                pattern_ptr carriage_return;
                pattern_ptr linefeed;
                pattern_ptr vertab;
                pattern_ptr formfeed;
                pattern_ptr space;
                pattern_ptr tab;
                pattern_ptr any;
                pattern_ptr newline;
                pattern_ptr uniWhite;
                pattern_ptr whitechar;
                pattern_ptr opencom;
                pattern_ptr closecom;
                pattern_ptr dashes;
                pattern_ptr ANY;
                pattern_ptr ANY_seq;
                pattern_ptr comment;
                pattern_ptr ncomment;
                pattern_ptr whitestuff;
                pattern_ptr whitespace;
                pattern_ptr decimal;
                pattern_ptr octal;
                pattern_ptr hexadecimal;
                pattern_ptr integer;
                pattern_ptr exponent;
                pattern_ptr floating;
                pattern_ptr charesc;
                pattern_ptr cntrl;
                pattern_ptr ascii;
                pattern_ptr escape;
                pattern_ptr character_literal;
                pattern_ptr gap;
                pattern_ptr string_literal;
                pattern_ptr literal;
                pattern_ptr reservedid;
                pattern_ptr varid;
                pattern_ptr conid;
                pattern_ptr reservedop;
                pattern_ptr varsym;
                pattern_ptr consym;
                pattern_ptr modid;
                pattern_ptr qvarid;
                pattern_ptr qconid;
                pattern_ptr qvarsym;
                pattern_ptr qconsym;
                pattern_ptr lexeme;
                pattern_ptr program;

                graft::scanner s;
        };

    }
}

#endif
