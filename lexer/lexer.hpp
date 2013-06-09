#ifndef LEXER_HPP
#define LEXER_HPP

#include "pattern.hpp"
#include "scanner.hpp"
#include "match.hpp"

#include "pattern/character.hpp"
#include "pattern/choice.hpp"
#include "pattern/compound.hpp"
#include "pattern/exclude.hpp"
#include "pattern/repetition.hpp"
#include "pattern/string.hpp"

#include <memory>
#include <sstream>
#include <unordered_map>

namespace dhc {
    namespace lexer {
        typedef std::shared_ptr<dhc::lexer::pattern::pattern> pattern_ptr;
        typedef std::shared_ptr<dhc::lexer::match::match> match_ptr;

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
                lexer(std::string source) : s(source)
                {
                    using namespace pattern;

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

                    ascDigit = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new character('0')),
                        pattern_ptr (new character('1')),
                        pattern_ptr (new character('2')),
                        pattern_ptr (new character('3')),
                        pattern_ptr (new character('4')),
                        pattern_ptr (new character('5')),
                        pattern_ptr (new character('6')),
                        pattern_ptr (new character('7')),
                        pattern_ptr (new character('8')),
                        pattern_ptr (new character('9'))
                    }));

                    digit = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        ascDigit
                    }));

                    octit = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new character('0')),
                        pattern_ptr (new character('1')),
                        pattern_ptr (new character('2')),
                        pattern_ptr (new character('3')),
                        pattern_ptr (new character('4')),
                        pattern_ptr (new character('5')),
                        pattern_ptr (new character('6')),
                        pattern_ptr (new character('7'))
                    }));

                    hexit = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        digit,
                        pattern_ptr (new character('A')),
                        pattern_ptr (new character('B')),
                        pattern_ptr (new character('C')),
                        pattern_ptr (new character('D')),
                        pattern_ptr (new character('E')),
                        pattern_ptr (new character('F')),
                        pattern_ptr (new character('a')),
                        pattern_ptr (new character('b')),
                        pattern_ptr (new character('c')),
                        pattern_ptr (new character('d')),
                        pattern_ptr (new character('e')),
                        pattern_ptr (new character('f')),
                    }));

                    ascSymbol = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new character('!')),
                        pattern_ptr (new character('#')),
                        pattern_ptr (new character('$')),
                        pattern_ptr (new character('%')),
                        pattern_ptr (new character('&')),
                        pattern_ptr (new character('*')),
                        pattern_ptr (new character('+')),
                        pattern_ptr (new character('.')),
                        pattern_ptr (new character('/')),
                        pattern_ptr (new character('<')),
                        pattern_ptr (new character('=')),
                        pattern_ptr (new character('>')),
                        pattern_ptr (new character('?')),
                        pattern_ptr (new character('@')),
                        pattern_ptr (new character('\\')),
                        pattern_ptr (new character('^')),
                        pattern_ptr (new character('|')),
                        pattern_ptr (new character('-')),
                        pattern_ptr (new character('~')),
                        pattern_ptr (new character(':'))
                    }));

                    symbol = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        ascSymbol
                    }));

                    ascLarge = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new character('A')),
                        pattern_ptr (new character('B')),
                        pattern_ptr (new character('C')),
                        pattern_ptr (new character('D')),
                        pattern_ptr (new character('E')),
                        pattern_ptr (new character('F')),
                        pattern_ptr (new character('G')),
                        pattern_ptr (new character('H')),
                        pattern_ptr (new character('I')),
                        pattern_ptr (new character('J')),
                        pattern_ptr (new character('K')),
                        pattern_ptr (new character('L')),
                        pattern_ptr (new character('M')),
                        pattern_ptr (new character('N')),
                        pattern_ptr (new character('O')),
                        pattern_ptr (new character('P')),
                        pattern_ptr (new character('Q')),
                        pattern_ptr (new character('R')),
                        pattern_ptr (new character('S')),
                        pattern_ptr (new character('T')),
                        pattern_ptr (new character('U')),
                        pattern_ptr (new character('V')),
                        pattern_ptr (new character('W')),
                        pattern_ptr (new character('X')),
                        pattern_ptr (new character('Y')),
                        pattern_ptr (new character('Z'))
                    }));

                    large = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        ascLarge
                    }));

                    ascSmall = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new character('a')),
                        pattern_ptr (new character('b')),
                        pattern_ptr (new character('c')),
                        pattern_ptr (new character('d')),
                        pattern_ptr (new character('e')),
                        pattern_ptr (new character('f')),
                        pattern_ptr (new character('g')),
                        pattern_ptr (new character('h')),
                        pattern_ptr (new character('i')),
                        pattern_ptr (new character('j')),
                        pattern_ptr (new character('k')),
                        pattern_ptr (new character('l')),
                        pattern_ptr (new character('m')),
                        pattern_ptr (new character('n')),
                        pattern_ptr (new character('o')),
                        pattern_ptr (new character('p')),
                        pattern_ptr (new character('q')),
                        pattern_ptr (new character('r')),
                        pattern_ptr (new character('s')),
                        pattern_ptr (new character('t')),
                        pattern_ptr (new character('u')),
                        pattern_ptr (new character('v')),
                        pattern_ptr (new character('w')),
                        pattern_ptr (new character('x')),
                        pattern_ptr (new character('y')),
                        pattern_ptr (new character('z'))
                    }));

                    small = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        ascSmall,
                        pattern_ptr (new character('_'))
                    }));

                    special = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new character('(')),
                        pattern_ptr (new character(')')),
                        pattern_ptr (new character(',')),
                        pattern_ptr (new character(';')),
                        pattern_ptr (new character('[')),
                        pattern_ptr (new character(']')),
                        pattern_ptr (new character('`')),
                        pattern_ptr (new character('{')),
                        pattern_ptr (new character('}'))
                    }, static_cast<int>(type::SPECIAL)));

                    graphic = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        small,
                        large,
                        symbol,
                        digit,
                        special,
                        pattern_ptr (new character('"')),
                        pattern_ptr (new character('\''))
                    }));

                    carriage_return = pattern_ptr (new character('\r'));
                    linefeed = pattern_ptr (new character('\n'));
                    vertab = pattern_ptr (new character('\v'));
                    formfeed = pattern_ptr (new character('\f'));
                    space = pattern_ptr (new character(' '));
                    tab = pattern_ptr (new character('\t'));

                    any = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        graphic,
                        space,
                        tab
                    }));

                    newline = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            carriage_return,
                            linefeed
                        })),
                        carriage_return,
                        linefeed,
                        formfeed
                    }));

                    whitechar = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        newline,
                        vertab,
                        space,
                        tab
                    }));

                    opencom = pattern_ptr (new string("{-"));
                    closecom = pattern_ptr (new string("-}"));

                    dashes = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new string("--")),
                        pattern_ptr (new repetition(pattern_ptr(new character('-'))))
                    }));

                    ANY = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        graphic,
                        whitechar
                    }));

                    ANY_seq = pattern_ptr (new repetition(
                        pattern_ptr (new exclude(ANY, pattern_ptr (new choice(std::vector<pattern_ptr> {
                            opencom,
                            closecom
                        }))))
                    ));

                    comment = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new choice(std::vector<pattern_ptr> {
                            pattern_ptr (new compound(std::vector<pattern_ptr> {
                                dashes,
                                pattern_ptr (new compound(std::vector<pattern_ptr> {
                                    pattern_ptr (new exclude(any, symbol)),
                                    pattern_ptr (new repetition(any))
                                })),
                                newline
                            })),
                            pattern_ptr (new compound(std::vector<pattern_ptr> {
                                dashes,
                                newline
                            }))
                        })),
                    }));

                    ncomment = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        opencom,
                        ANY_seq
                    }));

                    std::static_pointer_cast<compound>(ncomment)->add_pattern(pattern_ptr (new repetition(
                        pattern_ptr(new compound(std::vector<pattern_ptr> {
                            ncomment,
                            ANY_seq
                        }))
                    )));
                    std::static_pointer_cast<compound>(ncomment)->add_pattern(closecom);

                    whitestuff = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        whitechar,
                        comment,
                        ncomment
                    }));

                    whitespace = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        whitestuff,
                        pattern_ptr (new repetition(whitestuff))
                    }, static_cast<int>(type::WHITESPACE)));

                    decimal = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        digit,
                        pattern_ptr (new repetition(digit))
                    }));

                    octal = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        octit,
                        pattern_ptr (new repetition(octit))
                    }));

                    hexadecimal = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        hexit,
                        pattern_ptr (new repetition(hexit))
                    }));

                    integer = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        decimal,
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            pattern_ptr (new compound(std::vector<pattern_ptr> {
                                pattern_ptr (new character('0')),
                                pattern_ptr (new choice(std::vector<pattern_ptr> {
                                    pattern_ptr (new character('o')),
                                    pattern_ptr (new character('O'))
                                }))
                            })),
                            octal
                        })),
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            pattern_ptr (new compound(std::vector<pattern_ptr> {
                                pattern_ptr (new character('0')),
                                pattern_ptr (new choice(std::vector<pattern_ptr> {
                                    pattern_ptr (new character('x')),
                                    pattern_ptr (new character('X'))
                                }))
                            })),
                            hexadecimal
                        }))
                    }));

                    exponent = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new choice(std::vector<pattern_ptr> {
                            pattern_ptr (new compound(std::vector<pattern_ptr> {
                                pattern_ptr (new choice(std::vector<pattern_ptr> {
                                    pattern_ptr (new character('e')),
                                    pattern_ptr (new character('E'))
                                })),
                                pattern_ptr (new choice(std::vector<pattern_ptr> {
                                    pattern_ptr (new character('+')),
                                    pattern_ptr (new character('-'))
                                })),
                                decimal
                            })),
                            pattern_ptr (new compound(std::vector<pattern_ptr> {
                                pattern_ptr (new choice(std::vector<pattern_ptr> {
                                    pattern_ptr (new character('e')),
                                    pattern_ptr (new character('E'))
                                })),
                                decimal
                            }))
                        }))
                    }));

                    floating = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            decimal,
                            pattern_ptr (new character('.')),
                            decimal
                        })),
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            decimal,
                            pattern_ptr (new character('.')),
                            decimal,
                            exponent
                        })),
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            decimal,
                            exponent
                        }))
                    }));

                    charesc = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new character('a')),
                        pattern_ptr (new character('b')),
                        pattern_ptr (new character('f')),
                        pattern_ptr (new character('n')),
                        pattern_ptr (new character('r')),
                        pattern_ptr (new character('t')),
                        pattern_ptr (new character('v')),
                        pattern_ptr (new character('\\')),
                        pattern_ptr (new character('"')),
                        pattern_ptr (new character('\'')),
                        pattern_ptr (new character('&'))
                    }));

                    cntrl = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        ascLarge,
                        pattern_ptr (new character('@')),
                        pattern_ptr (new character('[')),
                        pattern_ptr (new character('\\')),
                        pattern_ptr (new character(']')),
                        pattern_ptr (new character('^')),
                        pattern_ptr (new character('_'))
                    }));

                    ascii = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            pattern_ptr (new character('^')),
                            cntrl
                        })),
                        pattern_ptr (new string("NUL")),
                        pattern_ptr (new string("SOH")),
                        pattern_ptr (new string("STX")),
                        pattern_ptr (new string("ETX")),
                        pattern_ptr (new string("EOT")),
                        pattern_ptr (new string("ENQ")),
                        pattern_ptr (new string("ACK")),
                        pattern_ptr (new string("BEL")),
                        pattern_ptr (new string("BS")),
                        pattern_ptr (new string("HT")),
                        pattern_ptr (new string("LF")),
                        pattern_ptr (new string("VT")),
                        pattern_ptr (new string("FF")),
                        pattern_ptr (new string("CR")),
                        pattern_ptr (new string("SO")),
                        pattern_ptr (new string("SI")),
                        pattern_ptr (new string("DLE")),
                        pattern_ptr (new string("DC1")),
                        pattern_ptr (new string("DC2")),
                        pattern_ptr (new string("DC3")),
                        pattern_ptr (new string("DC4")),
                        pattern_ptr (new string("NAK")),
                        pattern_ptr (new string("SYN")),
                        pattern_ptr (new string("ETB")),
                        pattern_ptr (new string("CAN")),
                        pattern_ptr (new string("EM")),
                        pattern_ptr (new string("SUB")),
                        pattern_ptr (new string("ESC")),
                        pattern_ptr (new string("FS")),
                        pattern_ptr (new string("GS")),
                        pattern_ptr (new string("RS")),
                        pattern_ptr (new string("US")),
                        pattern_ptr (new string("SP")),
                        pattern_ptr (new string("DEL"))
                    }));

                    escape = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new character('\\')),
                        pattern_ptr (new choice(std::vector<pattern_ptr> {
                            charesc,
                            ascii,
                            decimal,
                            pattern_ptr (new compound(std::vector<pattern_ptr> {
                                pattern_ptr (new character('o')),
                                octal
                            })),
                            pattern_ptr (new compound(std::vector<pattern_ptr> {
                                pattern_ptr (new character('x')),
                                hexadecimal
                            }))
                        }))
                    }));

                    character_literal = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new character('\'')),
                        pattern_ptr (new choice(std::vector<pattern_ptr> {
                            pattern_ptr (new exclude(graphic, pattern_ptr (new choice(std::vector<pattern_ptr> {
                                pattern_ptr (new character('\'')),
                                pattern_ptr (new character('\\'))
                            })))),
                            space,
                            pattern_ptr (new exclude(escape, pattern_ptr (new string("\\&"))))
                        })),
                        pattern_ptr (new character('\''))
                    }));

                    gap = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new character('\\')),
                        whitechar,
                        pattern_ptr (new repetition(whitechar)),
                        pattern_ptr (new character('\\'))
                    }));

                    string_literal = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new character('"')),
                        pattern_ptr (new repetition(pattern_ptr (new choice(std::vector<pattern_ptr> {
                            pattern_ptr (new exclude(graphic, pattern_ptr (new choice(std::vector<pattern_ptr> {
                                pattern_ptr (new character('"')),
                                pattern_ptr (new character('\\'))
                            })))),
                            space,
                            escape,
                            gap
                        })))),
                        pattern_ptr (new character('"'))
                    }));

                    literal = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        integer,
                        floating,
                        character_literal,
                        string_literal
                    }, static_cast<int>(type::LITERAL)));

                    reservedid = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new string("case")),
                        pattern_ptr (new string("class")),
                        pattern_ptr (new string("data")),
                        pattern_ptr (new string("default")),
                        pattern_ptr (new string("deriving")),
                        pattern_ptr (new string("do")),
                        pattern_ptr (new string("else")),
                        pattern_ptr (new string("foreign")),
                        pattern_ptr (new string("if")),
                        pattern_ptr (new string("import")),
                        pattern_ptr (new string("in")),
                        pattern_ptr (new string("infix")),
                        pattern_ptr (new string("infix1")),
                        pattern_ptr (new string("infixr")),
                        pattern_ptr (new string("instance")),
                        pattern_ptr (new string("let")),
                        pattern_ptr (new string("module")),
                        pattern_ptr (new string("newtype")),
                        pattern_ptr (new string("of")),
                        pattern_ptr (new string("then")),
                        pattern_ptr (new string("type")),
                        pattern_ptr (new string("where")),
                        pattern_ptr (new character('_'))
                    }, static_cast<int>(type::RESERVEDID)));

                    varid = pattern_ptr (new exclude(
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            small,
                            pattern_ptr (new repetition(pattern_ptr (new choice(std::vector<pattern_ptr> {
                                small,
                                large,
                                digit,
                                pattern_ptr (new character('\''))
                            }))))
                        })),
                        reservedid
                    ));

                    conid = pattern_ptr (pattern_ptr (new compound(std::vector<pattern_ptr> {
                        large,
                        pattern_ptr (new repetition(pattern_ptr (new choice(std::vector<pattern_ptr> {
                            small,
                            large,
                            digit,
                            pattern_ptr (new character('\''))
                        }))))
                    })));

                    reservedop = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new string("..")),
                        pattern_ptr (new character(':')),
                        pattern_ptr (new string("::")),
                        pattern_ptr (new character('=')),
                        pattern_ptr (new character('\\')),
                        pattern_ptr (new character('|')),
                        pattern_ptr (new string("<-")),
                        pattern_ptr (new string("->")),
                        pattern_ptr (new character('@')),
                        pattern_ptr (new character('~')),
                        pattern_ptr (new string("=>"))
                    }, static_cast<int>(type::RESERVEDOP)));

                    varsym = pattern_ptr (new exclude(pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new exclude(symbol, pattern_ptr (new character(':')))),
                        pattern_ptr (new repetition(symbol))
                    })), pattern_ptr (new choice(std::vector<pattern_ptr> {
                        reservedop,
                        dashes
                    }))));

                    consym = pattern_ptr (new exclude(pattern_ptr (new compound(std::vector<pattern_ptr> {
                        pattern_ptr (new character(':')),
                        pattern_ptr (new repetition(symbol))
                    })), reservedop));

                    modid = pattern_ptr (new compound(std::vector<pattern_ptr> {
                        conid,
                        pattern_ptr (new repetition(pattern_ptr (new compound(std::vector<pattern_ptr> {
                            pattern_ptr (new character('.')),
                            conid
                        }))))
                    }, static_cast<int>(type::QCONID)));

                    qvarid = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            modid,
                            pattern_ptr (new character('.')),
                            varid
                        })),
                        varid
                    }, static_cast<int>(type::QVARID)));

                    qconid = pattern_ptr (modid);

                    qvarsym = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            modid,
                            pattern_ptr (new character('.')),
                            varsym
                        })),
                        varsym
                    }, static_cast<int>(type::QVARSYM)));

                    qconsym = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        pattern_ptr (new compound(std::vector<pattern_ptr> {
                            modid,
                            pattern_ptr (new character('.')),
                            consym
                        })),
                        consym
                    }, static_cast<int>(type::QCONSYM)));

                    lexeme = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        qvarid,
                        qconid,
                        qvarsym,
                        qconsym,
                        literal,
                        special,
                        reservedop,
                        reservedid
                    }));

                    program = pattern_ptr (new choice(std::vector<pattern_ptr> {
                        lexeme,
                        whitespace
                    }));
                }
                match_ptr next();
                bool finished();

                std::string error(std::string filename);
                std::unordered_map<int, std::string> typenames;
            private:
                pattern_ptr ascDigit;
                pattern_ptr digit;
                pattern_ptr octit;
                pattern_ptr hexit;
                pattern_ptr ascSymbol;
                pattern_ptr symbol;
                pattern_ptr ascLarge;
                pattern_ptr large;
                pattern_ptr ascSmall;
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

                scanner s;
        };

    }
}

#endif
