#ifndef LEXER_HPP
#define LEXER_HPP

#include "../graft/scanner.hpp"

#include "../graft/pattern.hpp"
#include "../graft/match.hpp"

#include "../graft/pattern/character.hpp"
#include "../graft/pattern/property.hpp"
#include "../graft/pattern/choice.hpp"
#include "../graft/pattern/compound.hpp"
#include "../graft/pattern/exclude.hpp"
#include "../graft/pattern/repetition.hpp"
#include "../graft/pattern/string.hpp"

#include <memory>
#include <sstream>
#include <unordered_map>

#include <unicode/unistr.h>

namespace dhc {
    namespace lexer {
        typedef std::shared_ptr<graft::pattern::pattern> pattern_ptr;
        typedef std::shared_ptr<graft::match::match> match_ptr;

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

        class lexer {
            public:
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

                    uniDigit = std::make_shared<property>("[:Nd:]");

                    digit = std::make_shared<choice>(std::vector<pattern_ptr> {
                        uniDigit
                    });

                    octit = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<character>('0'),
                        std::make_shared<character>('1'),
                        std::make_shared<character>('2'),
                        std::make_shared<character>('3'),
                        std::make_shared<character>('4'),
                        std::make_shared<character>('5'),
                        std::make_shared<character>('6'),
                        std::make_shared<character>('7')
                    });

                    hexit = std::make_shared<choice>(std::vector<pattern_ptr> {
                        digit,
                        std::make_shared<character>('A'),
                        std::make_shared<character>('B'),
                        std::make_shared<character>('C'),
                        std::make_shared<character>('D'),
                        std::make_shared<character>('E'),
                        std::make_shared<character>('F'),
                        std::make_shared<character>('a'),
                        std::make_shared<character>('b'),
                        std::make_shared<character>('c'),
                        std::make_shared<character>('d'),
                        std::make_shared<character>('e'),
                        std::make_shared<character>('f'),
                    });

                    uniSymbol = std::make_shared<property>("[[:Punctuation:][:Symbol:]]");

                    symbol = std::make_shared<choice>(std::vector<pattern_ptr> {
                        uniSymbol
                    });

                    uniLarge = std::make_shared<property>("[[:Lu:][:Lt:]]");

                    large = std::make_shared<choice>(std::vector<pattern_ptr> {
                        uniLarge
                    });

                    uniSmall = std::make_shared<property>("[:Ll:]");

                    small = std::make_shared<choice>(std::vector<pattern_ptr> {
                        uniSmall,
                        std::make_shared<character>('_')
                    });

                    special = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<character>('('),
                        std::make_shared<character>(')'),
                        std::make_shared<character>(','),
                        std::make_shared<character>(';'),
                        std::make_shared<character>('['),
                        std::make_shared<character>(']'),
                        std::make_shared<character>('`'),
                        std::make_shared<character>('{'),
                        std::make_shared<character>('}')
                    }, static_cast<int>(type::SPECIAL));

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
                    });

                    uniWhite = std::make_shared<property>("[:White_Space:]");

                    whitechar = std::make_shared<choice>(std::vector<pattern_ptr> {
                        /** omit because redundant **
                         * newline,
                         * vertab,
                         * space,
                         * tab,
                         */
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

                    ncomment = std::make_shared<compound>(std::vector<pattern_ptr> {
                        opencom,
                        ANY_seq
                    });

                    std::static_pointer_cast<compound>(ncomment)->add_pattern(std::make_shared<repetition>(
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            ncomment,
                            ANY_seq
                        })
                    ));
                    std::static_pointer_cast<compound>(ncomment)->add_pattern(closecom);

                    whitestuff = std::make_shared<choice>(std::vector<pattern_ptr> {
                        whitechar,
                        comment,
                        ncomment
                    });

                    whitespace = std::make_shared<compound>(std::vector<pattern_ptr> {
                        whitestuff,
                        std::make_shared<repetition>(whitestuff)
                    }, static_cast<int>(type::WHITESPACE));

                    decimal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        digit,
                        std::make_shared<repetition>(digit)
                    });

                    octal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        octit,
                        std::make_shared<repetition>(octit)
                    });

                    hexadecimal = std::make_shared<compound>(std::vector<pattern_ptr> {
                        hexit,
                        std::make_shared<repetition>(hexit)
                    });

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
                        }),
                        // TODO:
                        // Hm? Double check below
                        // Find out if it's weird stuff
                        // or if it can be removed
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<character>('0'),
                                std::make_shared<choice>(std::vector<pattern_ptr> {
                                    std::make_shared<character>('x'),
                                    std::make_shared<character>('X')
                                })
                            }),
                            hexadecimal
                        })
                    });

                    exponent = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<choice>(std::vector<pattern_ptr> {
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
                            }),
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<choice>(std::vector<pattern_ptr> {
                                    std::make_shared<character>('e'),
                                    std::make_shared<character>('E')
                                }),
                                decimal
                            })
                        })
                    });

                    floating = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            decimal,
                            std::make_shared<character>('.'),
                            decimal
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            decimal,
                            std::make_shared<character>('.'),
                            decimal,
                            exponent
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            decimal,
                            exponent
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
                        })),
                        std::make_shared<character>('"')
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
                    varid = std::make_shared<exclude>(
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            small,
                            std::make_shared<repetition>(std::make_shared<choice>(std::vector<pattern_ptr> {
                                small,
                                large,
                                digit,
                                std::make_shared<character>('\'')
                            }))
                        }),
                        reservedid
                    );

                    conid = std::make_shared<compound>(std::vector<pattern_ptr> {
                        large,
                        std::make_shared<repetition>(std::make_shared<choice>(std::vector<pattern_ptr> {
                            small,
                            large,
                            digit,
                            std::make_shared<character>('\'')
                        }))
                    });

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
                    }), std::make_shared<choice>(std::vector<pattern_ptr> {
                        reservedop,
                        dashes
                    }));

                    consym = std::make_shared<exclude>(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<character>(':'),
                        std::make_shared<repetition>(symbol)
                    }), reservedop);

                    modid = std::make_shared<compound>(std::vector<pattern_ptr> {
                        conid,
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<character>('.'),
                            conid
                        }))
                    }, static_cast<int>(type::QCONID)); // TODO: why is this set to QCONID rather than, say, MODID?

                    qvarid = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            modid,
                            std::make_shared<character>('.'),
                            varid
                        }),
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
                        }),
                        varsym
                    }, static_cast<int>(type::QVARSYM));

                    qconsym = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            modid,
                            std::make_shared<character>('.'),
                            consym
                        }),
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
                match_ptr next();
                bool finished();

                std::string error(std::string filename);
                std::unordered_map<int, std::string> typenames;
            private:
                pattern_ptr uniDigit;
                pattern_ptr digit;
                pattern_ptr octit;
                pattern_ptr hexit;
                pattern_ptr uniSymbol;
                pattern_ptr symbol;
                pattern_ptr uniLarge;
                pattern_ptr large;
                pattern_ptr uniSmall;
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
