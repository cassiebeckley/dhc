#ifndef DHC_LEXER_LEXER_HPP
#define DHC_LEXER_LEXER_HPP

#include <graft/scanner.hpp>
#include <graft/scanner/character.hpp>

#include <graft/pattern.hpp>
#include <graft/match.hpp>

#include <graft/pattern/character.hpp>
#include <graft/pattern/property.hpp>
#include <graft/pattern/choice.hpp>
#include <graft/pattern/compound.hpp>
#include <graft/pattern/exclude.hpp>
#include <graft/pattern/repetition.hpp>
#include <graft/pattern/string.hpp>

#include "match/digit.hpp"
#include "match/qualified.hpp"
#include "match/integer.hpp"
#include "match/lit_float.hpp"
#include "match/lit_char.hpp"
#include "match/lit_string.hpp"

#include <memory>
#include <sstream>
#include <unordered_map>
#include <stack>
#include <queue>

#include <unicode/unistr.h>

#include <iostream>

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
            RESERVEDID,

            CURLY,
            ANGLE
        };

        /**
         * \brief A lexical analyzer for Haskell 2010.
         *
         * Like, stuff
         */
        class lexer : public graft::scanner::scanner {
            public:
                /**
                 * \brief Initialize the lexical analyzer
                 * 
                 * Creates a lexical analyzer conforming to the Haskell
                 * 2010 specification.
                 * @param source the source code to analyze
                 */
                lexer(icu::UnicodeString source) : s(source), column(0), line_number(0), beginning(true), expecting(false), current_line(0), previous_index(0)
                {
                    // TODO: get rid of this mebbe?
                    using namespace graft::pattern;

                    std::cout << "lexer::lexer" << std::endl;

                    match_func digit_h = [](match_ptr m) {
                        UChar32 c = m->flatten()[0];
                        int32_t d = u_charDigitValue(c);
                        return std::make_shared<dhc::lexer::digit>(m->type, c, d);
                    };

                    auto digit = std::make_shared<property>("[:Nd:]", -1, digit_h);
                    auto octit = std::make_shared<property>("[0-7]", -1, digit_h);

                    auto hexit = std::make_shared<choice>(std::vector<pattern_ptr> {
                        digit,
                        std::make_shared<property>("[A-F]", -1, [] (match_ptr m) {
                            char c = m->flatten()[0];
                            return std::make_shared<dhc::lexer::digit>(m->type, c, c - 'A');
                        }),
                        std::make_shared<property>("[a-f]", -1, [] (match_ptr m) {
                            char c = m->flatten()[0];
                            return std::make_shared<dhc::lexer::digit>(m->type, c, c - 'a');
                        })
                    });

                    auto special = std::make_shared<property>("[\\(\\),;\\[\\]`\\{\\}]", static_cast<int>(type::SPECIAL));

                    auto symbol = std::make_shared<exclude>(
                        std::make_shared<property>("[[:Punctuation:][:Symbol:]]"),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            special,
                            std::make_shared<property>("[_\"']")
                        })
                    );

                    auto large = std::make_shared<property>("[[:Lu:][:Lt:]]");
                    auto small = std::make_shared<property>("[[:Ll:]_]");

                    auto graphic = std::make_shared<choice>(std::vector<pattern_ptr> {
                        small,
                        large,
                        symbol,
                        digit,
                        special,
                        std::make_shared<character>('"'),
                        std::make_shared<character>('\'')
                    });

                    auto carriage_return = std::make_shared<character>('\r');
                    auto linefeed = std::make_shared<character>('\n');
                    auto vertab = std::make_shared<character>('\v');
                    auto formfeed = std::make_shared<character>('\f');
                    auto space = std::make_shared<character>(' ');
                    auto tab = std::make_shared<character>('\t', -1, [&] (match_ptr m) {
                        column += s.index - previous_index;
                        previous_index = s.index;
                        column = ((column == 0 ? 0 : column / 8) + 1) * 8;
                        return m;
                    });

                    auto any = std::make_shared<choice>(std::vector<pattern_ptr> {
                        graphic,
                        space,
                        tab
                    });

                    auto newline = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            carriage_return,
                            linefeed
                        }),
                        carriage_return,
                        linefeed,
                        formfeed
                    }, -1, [&] (match_ptr m) {
                        column = 0;
                        previous_index = s.index;
                        line_number++;
                        return m;
                    });

                    auto uniWhite = std::make_shared<property>("[:White_Space:]");

                    auto whitechar = std::make_shared<choice>(std::vector<pattern_ptr> {
                        newline,
                        tab,
                        uniWhite
                    });

                    auto opencom = std::make_shared<string>("{-");
                    auto closecom = std::make_shared<string>("-}");

                    auto dashes = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<string>("--"),
                        std::make_shared<repetition>(std::make_shared<character>('-'))
                    });

                    auto ANY = std::make_shared<choice>(std::vector<pattern_ptr> {
                        graphic,
                        whitechar
                    });

                    auto ANY_seq = std::make_shared<repetition>(
                        std::make_shared<exclude>(ANY, std::make_shared<choice>(std::vector<pattern_ptr> {
                            opencom,
                            closecom
                        }))
                    );

                    auto comment = std::make_shared<compound>(std::vector<pattern_ptr> {
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

                    auto ncomment = std::make_shared<compound>(std::vector<pattern_ptr> {
                        opencom,
                        ANY_seq
                    });

                    ncomment->add_pattern(std::make_shared<repetition>(
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            ncomment,
                            ANY_seq
                        })
                    ));
                    ncomment->add_pattern(closecom);

                    auto whitestuff = std::make_shared<choice>(std::vector<pattern_ptr> {
                        whitechar,
                        comment,
                        ncomment
                    });

                    auto whitespace = std::make_shared<compound>(std::vector<pattern_ptr> {
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

                            return std::make_shared<dhc::lexer::integer>(m->type, m->length(), dec);
                        };
                    };

                    auto decimal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        digit,
                        std::make_shared<repetition>(digit)
                    }, -1, from_base(10));

                    auto octal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        octit,
                        std::make_shared<repetition>(octit)
                    }, -1, from_base(8));

                    auto hexadecimal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        hexit,
                        std::make_shared<repetition>(hexit)
                    }, -1, from_base(16));

                    auto integer = std::make_shared<choice>(std::vector<pattern_ptr> {
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
                        // The below is necessary for hexadecimal lexing
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

                    auto exponent = std::make_shared<choice>(std::vector<pattern_ptr> {
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
                            return std::make_shared<dhc::lexer::integer>(m->type, m->length(), value);
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
                            auto a = std::make_shared<dhc::lexer::integer>(m->type, m->length(), value);
                            return a;
                        }),
                    });

                    auto floating = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            decimal,
                            std::make_shared<character>('.'),
                            decimal
                        }, -1, [] (match_ptr m) {
                            auto children = m->children();
                            std::shared_ptr<dhc::lexer::integer> in = std::dynamic_pointer_cast<dhc::lexer::integer>(children[0]);
                            std::shared_ptr<dhc::lexer::integer> fr = std::dynamic_pointer_cast<dhc::lexer::integer>(children[2]);
                            return std::make_shared<dhc::lexer::lit_float>(m->type, m->length(), in->data, fr->data, 1);
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
                            return std::make_shared<dhc::lexer::lit_float>(m->type, m->length(), in->data, fr->data, ex->data);

                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            decimal,
                            exponent
                        }, -1, [] (match_ptr m) {
                            auto children = m->children();
                            auto in = std::dynamic_pointer_cast<dhc::lexer::integer>(children[0]);
                            auto ex = std::dynamic_pointer_cast<dhc::lexer::integer>(children[1]);
                            return std::make_shared<dhc::lexer::lit_float>(m->type, m->length(), in->data, 0, ex->data);

                        })
                    });

                    auto charesc = std::make_shared<choice>(std::vector<pattern_ptr> {
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

                    auto cntrl = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<property>("[A-Z]"),
                        std::make_shared<character>('@'),
                        std::make_shared<character>('['),
                        std::make_shared<character>('\\'),
                        std::make_shared<character>(']'),
                        std::make_shared<character>('^'),
                        std::make_shared<character>('_')
                    });

                    auto ascii = std::make_shared<choice>(std::vector<pattern_ptr> {
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

                    auto escape = std::make_shared<compound>(std::vector<pattern_ptr> {
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

                    auto character_literal = std::make_shared<compound>(std::vector<pattern_ptr> {
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
                        return std::make_shared<lit_char>(m->type, middle);
                    });

                    auto gap = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<character>('\\'),
                        whitechar,
                        std::make_shared<repetition>(whitechar),
                        std::make_shared<character>('\\')
                    });

                    auto string_literal = std::make_shared<compound>(std::vector<pattern_ptr> {
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
                                chars.push_back(std::make_shared<lit_char>((*it)->type, (*it)->flatten()));
                            }
                            return std::make_shared<lit_string>(m->type, chars);
                        }),
                        std::make_shared<character>('"')
                    }, -1, [] (match_ptr m) {
                        return m->children()[1];
                    });

                    auto literal = std::make_shared<choice>(std::vector<pattern_ptr> {
                        integer,
                        floating,
                        character_literal,
                        string_literal
                    }, static_cast<int>(type::LITERAL));

                    auto reservedid = std::make_shared<choice>(std::vector<pattern_ptr> {
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
                        return std::make_shared<graft::match::string>(m->type, m->flatten());
                    };

                    auto varid = std::make_shared<exclude>(
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

                    auto conid = std::make_shared<compound>(std::vector<pattern_ptr> {
                        large,
                        std::make_shared<repetition>(std::make_shared<choice>(std::vector<pattern_ptr> {
                            small,
                            large,
                            digit,
                            std::make_shared<character>('\'')
                        }))
                    }, -1, flatten);

                    auto reservedop = std::make_shared<choice>(std::vector<pattern_ptr> {
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

                    auto varsym = std::make_shared<exclude>(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<exclude>(symbol, std::make_shared<character>(':')),
                        std::make_shared<repetition>(symbol)
                    }, -1, flatten 
                    ), std::make_shared<choice>(std::vector<pattern_ptr> {
                        reservedop,
                        dashes
                    }));

                    auto consym = std::make_shared<exclude>(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<character>(':'),
                        std::make_shared<repetition>(symbol)
                    }, -1, flatten), reservedop);

                    auto modid = std::make_shared<compound>(std::vector<pattern_ptr> {
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
                            return std::make_shared<qualified>(m->type, res);
                        })
                    }, -1, [] (match_ptr m) {
                        auto children = m->children();
                        icu::UnicodeString conid = children[0]->flatten();
                        auto second = std::dynamic_pointer_cast<qualified>(children[1]);
                        auto mods = second->modules;
                        mods.insert(mods.begin(), conid);
                        return std::make_shared<qualified>(m->type, mods);
                    });

                    match_func qualify = [](match_ptr m) {
                        auto children = m->children();
                        auto mods = std::dynamic_pointer_cast<qualified>(children[0])->modules;
                        icu::UnicodeString id = children[2]->flatten();
                        mods.push_back(id);
                        return std::make_shared<qualified>(m->type, mods);
                    };

                    auto qvarid = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            modid,
                            std::make_shared<character>('.'),
                            varid
                        }, -1, qualify),
                        varid
                    }, static_cast<int>(type::QVARID));

                    auto qconid = std::make_shared<choice>(std::vector<pattern_ptr> {
                        modid
                    }, static_cast<int>(type::QCONID));

                    auto qvarsym = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            modid,
                            std::make_shared<character>('.'),
                            varsym
                        }, -1, qualify),
                        varsym
                    }, static_cast<int>(type::QVARSYM));

                    auto qconsym = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            modid,
                            std::make_shared<character>('.'),
                            consym
                        }, -1, qualify),
                        consym
                    }, static_cast<int>(type::QCONSYM));

                    auto lexeme = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qvarid,
                        qconid,
                        qvarsym,
                        qconsym,
                        literal,
                        special,
                        reservedop,
                        reservedid
                    }, -1);

                    auto program = std::make_shared<choice>(std::vector<pattern_ptr> {
                        lexeme,
                        whitespace
                    });

                    match_ptr m;
                    while ((m = generate_next(program)) != nullptr)
                    {
                        final_tokens.push_back(m);
                    }
                    std::cout << "Finished creating lexer" << std::endl;
                }

                virtual match_ptr next();

                virtual bool finished();

                /**
                 * \brief Returns an error message.
                 *
                 * Creates an error message to be used if next()
                 * returned an error value.
                 * @param filename the name of the source file
                 * @return An error message
                 */
                std::string error(std::string filename);

            private:
                match_ptr generate_next(pattern_ptr p);

                graft::scanner::character s;
                unsigned int column;
                unsigned int previous_column;
                unsigned int line_number;
                bool beginning;
                bool expecting;
                unsigned int current_line;
                unsigned int previous_index;
                std::queue<match_ptr> tokens;

                std::vector<match_ptr> final_tokens;
        };

    }
}

#endif
