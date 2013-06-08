#include "lexer.hpp"

#include "pattern/character.hpp"
#include "pattern/choice.hpp"
#include "pattern/compound.hpp"
#include "pattern/exclude.hpp"
#include "pattern/repetition.hpp"
#include "pattern/string.hpp"



#include <iostream>

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::lexer::next()
{
    using namespace pattern;
    character one('a');
    pattern_ptr ascDigit (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr digit (new choice(std::vector<pattern_ptr> {
        ascDigit
    }));

    pattern_ptr octit (new choice(std::vector<pattern_ptr> {
        pattern_ptr (new character('0')),
        pattern_ptr (new character('1')),
        pattern_ptr (new character('2')),
        pattern_ptr (new character('3')),
        pattern_ptr (new character('4')),
        pattern_ptr (new character('5')),
        pattern_ptr (new character('6')),
        pattern_ptr (new character('7'))
    }));

    pattern_ptr hexit (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr ascSymbol (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr symbol (new choice(std::vector<pattern_ptr> {
        ascSymbol
    }));

    pattern_ptr ascLarge (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr large (new choice(std::vector<pattern_ptr> {
        ascLarge
    }));

    pattern_ptr ascSmall (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr small (new choice(std::vector<pattern_ptr> {
        ascSmall,
        pattern_ptr (new character('_'))
    }));

    pattern_ptr special (new choice(std::vector<pattern_ptr> {
        pattern_ptr (new character('(')),
        pattern_ptr (new character(')')),
        pattern_ptr (new character(',')),
        pattern_ptr (new character(';')),
        pattern_ptr (new character('[')),
        pattern_ptr (new character(']')),
        pattern_ptr (new character('`')),
        pattern_ptr (new character('{')),
        pattern_ptr (new character('}'))
    }));

    pattern_ptr graphic (new choice(std::vector<pattern_ptr> {
        small,
        large,
        symbol,
        digit,
        special,
        pattern_ptr (new character('"')),
        pattern_ptr (new character('\''))
    }));

    pattern_ptr carriage_return (new character('\r'));
    pattern_ptr linefeed (new character('\n'));
    pattern_ptr vertab (new character('\v'));
    pattern_ptr formfeed (new character('\f'));
    pattern_ptr space (new character(' '));
    pattern_ptr tab (new character('\t'));

    pattern_ptr any (new choice(std::vector<pattern_ptr> {
        graphic,
        space,
        tab
    }));

    pattern_ptr newline (new choice(std::vector<pattern_ptr> {
        pattern_ptr (new compound(std::vector<pattern_ptr> {
            carriage_return,
            linefeed
        })),
        carriage_return,
        linefeed,
        formfeed
    }));

    pattern_ptr whitechar (new choice(std::vector<pattern_ptr> {
        newline,
        vertab,
        space,
        tab
    }));

    pattern_ptr opencom (new string("{-"));
    pattern_ptr closecom (new string("-}"));

    pattern_ptr dashes (new compound(std::vector<pattern_ptr> {
        pattern_ptr (new string("--")),
        pattern_ptr (new repetition(pattern_ptr(new character('-'))))
    }));

    pattern_ptr ANY (new choice(std::vector<pattern_ptr> {
        graphic,
        whitechar
    }));

    pattern_ptr ANY_seq (new repetition(
        pattern_ptr (new exclude(ANY, pattern_ptr (new choice(std::vector<pattern_ptr> {
            opencom,
            closecom
        }))))
    ));

    pattern_ptr comment (new compound(std::vector<pattern_ptr> {
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

    pattern_ptr ncomment (new compound(std::vector<pattern_ptr> {
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

    pattern_ptr whitestuff(new choice(std::vector<pattern_ptr> {
        whitechar,
        comment,
        ncomment
    }));

    pattern_ptr whitespace(new compound(std::vector<pattern_ptr> {
        whitestuff,
        pattern_ptr (new repetition(whitestuff))
    }));

    pattern_ptr decimal(new compound(std::vector<pattern_ptr> {
        digit,
        pattern_ptr (new repetition(digit))
    }));

    pattern_ptr octal(new compound(std::vector<pattern_ptr> {
        octit,
        pattern_ptr (new repetition(octit))
    }));

    pattern_ptr hexadecimal(new compound(std::vector<pattern_ptr> {
        hexit,
        pattern_ptr (new repetition(hexit))
    }));

    pattern_ptr integer (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr exponent (new compound(std::vector<pattern_ptr> {
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

    pattern_ptr floating (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr charesc (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr cntrl (new choice(std::vector<pattern_ptr> {
        ascLarge,
        pattern_ptr (new character('@')),
        pattern_ptr (new character('[')),
        pattern_ptr (new character('\\')),
        pattern_ptr (new character(']')),
        pattern_ptr (new character('^')),
        pattern_ptr (new character('_'))
    }));

    pattern_ptr ascii (new choice(std::vector<pattern_ptr> {
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

    pattern_ptr escape (new compound(std::vector<pattern_ptr> {
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

    pattern_ptr character_literal (new compound(std::vector<pattern_ptr> {
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

    pattern_ptr gap (new compound(std::vector<pattern_ptr> {
        pattern_ptr (new character('\\')),
        whitechar,
        pattern_ptr (new repetition(whitechar)),
        pattern_ptr (new character('\\'))
    }));

    pattern_ptr string_literal (new compound(std::vector<pattern_ptr> {
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

    pattern_ptr literal (new choice(std::vector<pattern_ptr> {
        integer,
        floating,
        character_literal,
        string_literal
    }));

    pattern_ptr reservedid (new choice(std::vector<pattern_ptr> {
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
    }));

    pattern_ptr varid (new exclude(
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

    pattern_ptr conid (pattern_ptr (new compound(std::vector<pattern_ptr> {
        large,
        pattern_ptr (new repetition(pattern_ptr (new choice(std::vector<pattern_ptr> {
            small,
            large,
            digit,
            pattern_ptr (new character('\''))
        }))))
    })));

    pattern_ptr reservedop (new choice(std::vector<pattern_ptr> {
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
    }));

    pattern_ptr varsym (new exclude(pattern_ptr (new compound(std::vector<pattern_ptr> {
        pattern_ptr (new exclude(symbol, pattern_ptr (new character(':')))),
        pattern_ptr (new repetition(symbol))
    })), pattern_ptr (new choice(std::vector<pattern_ptr> {
        reservedop,
        dashes
    }))));

    pattern_ptr consym (new exclude(pattern_ptr (new compound(std::vector<pattern_ptr> {
        pattern_ptr (new character(':')),
        pattern_ptr (new repetition(symbol))
    })), reservedop));

    pattern_ptr modid (new compound(std::vector<pattern_ptr> {
        conid,
        pattern_ptr (new repetition(pattern_ptr (new compound(std::vector<pattern_ptr> {
            pattern_ptr (new character('.')),
            conid
        }))))
    }));

    pattern_ptr qvarid (new choice(std::vector<pattern_ptr> {
        pattern_ptr (new compound(std::vector<pattern_ptr> {
            modid,
            pattern_ptr (new character('.')),
            varid
        })),
        varid
    }));

    pattern_ptr qconid (modid);

    pattern_ptr qvarsym (new choice(std::vector<pattern_ptr> {
        pattern_ptr (new compound(std::vector<pattern_ptr> {
            modid,
            pattern_ptr (new character('.')),
            varsym
        })),
        varsym
    }));

    pattern_ptr qconsym (new choice(std::vector<pattern_ptr> {
        pattern_ptr (new compound(std::vector<pattern_ptr> {
            modid,
            pattern_ptr (new character('.')),
            consym
        })),
        consym
    }));

    pattern_ptr lexeme (new choice(std::vector<pattern_ptr> {
        qvarid,
        qconid,
        qvarsym,
        qconsym,
        literal,
        special,
        reservedop,
        reservedid
    }));

    pattern_ptr program (new choice(std::vector<pattern_ptr> {
        lexeme,
        whitespace
    }));

    auto m = program->find(s);

    return m;
}
