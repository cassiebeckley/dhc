#ifndef DHC_PARSER_PARSER_HPP
#define DHC_PARSER_PARSER_HPP

#include <graft/pattern.hpp>
#include <graft/match.hpp>

#include <graft/pattern/type.hpp>
#include <graft/pattern/choice.hpp>
#include <graft/pattern/compound.hpp>
#include <graft/pattern/exclude.hpp>
#include <graft/pattern/repetition.hpp>

#include <memory>

#include "../lexer/lexer.hpp"

namespace dhc {
    namespace parser {
        typedef std::shared_ptr<graft::pattern::pattern> pattern_ptr;
        typedef std::shared_ptr<graft::match::match> match_ptr;

        /**
         * \brief A parser for Haskell 2010.
         *
         * Like, stuff
         */
        class parser {
            public:
                /**
                 * \brief Initialize the parser
                 * 
                 * Creates a parser conforming to the Haskell
                 * 2010 specification.
                 * @param source the source code to analyze
                 */
                parser(icu::UnicodeString source) : lex(source)
                {
                    // TODO: get rid of this mebbe?
                    using namespace graft::pattern;

                    auto exp = std::make_shared<choice>(std::vector<pattern_ptr>{});
                    auto pat = std::make_shared<choice>(std::vector<pattern_ptr> {});
                    auto infixexp = std::make_shared<choice>(std::vector<pattern_ptr> {});
                    auto decls = std::make_shared<compound>(std::vector<pattern_ptr>{});
                    auto h_type = std::make_shared<choice>(std::vector<pattern_ptr>{});
                    auto qvar = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::QVARID)),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM)),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        })
                    });

                    auto gconsym = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), ":"),
                        std::make_shared<type>(static_cast<int>(lexer::type::QCONSYM))
                    });

                    auto qcon = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::QCONID)),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            gconsym,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        })
                    });

                    auto qvarop = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM)),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "`"),
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARID)),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "`")
                        })
                    });

                    auto qconop = std::make_shared<choice>(std::vector<pattern_ptr> {
                        gconsym,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "`"),
                            std::make_shared<type>(static_cast<int>(lexer::type::QCONID)),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "`")
                        })
                    });

                    auto qop = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qvarop,
                        qconop
                    });

                    auto gcon = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "["),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "]")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            std::make_shared<repetition>(std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ",")),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        qcon
                    });

                    auto qual = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "<-"),
                            exp
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "let"),
                            decls
                        }),
                        exp
                    });

                    auto fbind = std::make_shared<compound>(std::vector<pattern_ptr> {
                        qvar,
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "="),
                        exp
                    });

                    auto aexp = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qvar,
                        gcon,
                        std::make_shared<type>(static_cast<int>(lexer::type::LITERAL)),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            exp,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            exp,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            exp,
                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                exp
                            })),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "["),
                            exp,
                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                exp
                            })),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "]")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "["),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    exp,
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                    exp
                                }),
                                exp
                            }),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), ".."),
                                    exp,
                                }),
                                std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "..")
                            }),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "]")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "["),
                            exp,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "|"),
                            qual,
                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                qual
                            })),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "]"),
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            infixexp,
                            qop,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")"),
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<exclude>(qop, std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM), "-")),
                            infixexp,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            qcon,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    fbind,
                                    std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                        fbind
                                    })),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}"),
                                }),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                            }),
                        })
                    });

                    /* TODO: fix
                     * too much lookahead :/
                    aexp->add_pattern(std::make_shared<compound>
                        (std::vector<pattern_ptr> {
                            std::make_shared<exclude>(aexp, qcon),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                            fbind,
                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                fbind
                            })),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                        })
                    );*/

                    auto fexp = std::make_shared<repetition>(aexp);

                    auto apat = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qvar, // TODO: should just be var
                        gcon,
                        std::make_shared<type>(static_cast<int>(lexer::type::LITERAL)),
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "_")
                    });

                    apat->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        qvar,
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "@"),
                        apat
                    }));

                    auto lpat = std::make_shared<choice>(std::vector<pattern_ptr> {
                        apat,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM), "-"),
                            std::make_shared<type>(static_cast<int>(lexer::type::LITERAL)), // TODO: only allow integer or float
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            gcon,
                            std::make_shared<repetition>(apat)
                        }),

                    });

                    pat->add_pattern(lpat);

                    pat->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        lpat,
                        qconop,
                        pat
                    }));

                    apat->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                        pat,
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                    }));

                    apat->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                        })),
                        pat,
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                    }));

                    apat->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "["),
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                        })),
                        pat,
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "]")
                    }));

                    apat->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "~"),
                        pat
                    }));

                    auto stmt = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            exp,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "<-"),
                            exp,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "let"),
                            decls,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";")
                        }),
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";")
                    });

                    auto exp_semi_opt = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            exp,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";")
                        }),
                        exp
                    });

                    auto stmts = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<repetition>(stmt),
                            exp
                        }),
                        std::make_shared<repetition>(stmt)
                    });

                    auto guard = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "<-"),
                            infixexp
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "let"),
                            decls
                        }),
                        infixexp
                    });

                    auto guards = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "|"),
                        std::make_shared<repetition>(guard)
                    });

                    auto gdrhs = std::make_shared<compound>(std::vector<pattern_ptr>{});
                    gdrhs->add_pattern(guards);
                    gdrhs->add_pattern(std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "="));
                    gdrhs->add_pattern(std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr>{
                            exp,
                            gdrhs
                        }),
                        exp
                    }));

                    auto rhs = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                exp,
                                std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "where"),
                                decls
                            }),
                            exp
                        }),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                gdrhs,
                                std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "where"),
                                decls
                            }),
                            gdrhs
                        })

                    });

                    auto funlhs = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            qvar, // TODO: should be var
                            apat,
                            std::make_shared<repetition>(apat)
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            qvarop, // TODO: should be varop
                            pat
                        })
                    });

                    funlhs->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr>{
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                        funlhs,
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")"),
                        apat,
                        std::make_shared<repetition>(apat)
                    }));

                    auto ops = std::make_shared<compound>(std::vector<pattern_ptr> {
                        qop, // TODO: should be op
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            qop // TODO: should be op
                        }))
                    });

                    auto vars = std::make_shared<compound>(std::vector<pattern_ptr> {
                        qvar, // TODO: should be var
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            qvar // TODO: should be var
                        }))
                    });

                    auto fixity = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "infixl"),
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "infixr"),
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "infix")
                    });

                    auto gtycon = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::QCONID)),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "["),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "]")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "->"),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            std::make_shared<repetition>(std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ",")),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        })
                    });

                    auto atype = std::make_shared<choice>(std::vector<pattern_ptr> {
                        gtycon,
                        std::make_shared<type>(static_cast<int>(lexer::type::QVARID)), // TODO: use varid instead
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            h_type,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            h_type,
                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                h_type
                            })),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "["),
                            h_type,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "]")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            h_type,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        })
                    });

                    auto h_class = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::QCONID)),
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARID)), // TODO: should just be varid
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::QCONID)),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARID)), // TODO: should just be varid
                            atype,
                            std::make_shared<repetition>(atype),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        })
                    });

                    auto context = std::make_shared<choice>(std::vector<pattern_ptr> {
                        h_class,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                        h_class,
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ",")
                                    })),
                                    h_class,
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                }),
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                })

                            })
                        })
                    });

                    auto gendecl = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            vars,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "::"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    context,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "=>"),
                                    h_type
                                }),
                                h_type
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            fixity,
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::LITERAL)), // TODO: only match integers
                                    ops
                                }),
                                ops
                            })
                        })

                    });

                    auto decl = std::make_shared<choice>(std::vector<pattern_ptr> {
                        gendecl,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                funlhs,
                                pat
                            }),
                            rhs
                        })
                    });

                    decls->add_pattern(std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"));
                    decls->add_pattern(std::make_shared<choice>(std::vector<pattern_ptr>{
                        std::make_shared<compound>(std::vector<pattern_ptr>{
                            decl,
                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                decl
                            })),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                        }),
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                    }));

                    auto gdpat = std::make_shared<compound>(std::vector<pattern_ptr> {
                        guards,
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "->")
                    });

                    gdpat->add_pattern(std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            exp,
                            gdpat
                        }),
                        exp
                    }));

                    auto alt = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "->"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    exp,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "where"),
                                    decls
                                }),
                                exp
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    gdpat,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "where"),
                                    decls
                                }),
                                gdpat
                            })
                        }),
                    });


                    auto alts = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<repetition>(std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                alt,
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";")
                            }),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";")
                        })),
                        alt
                    });

                    auto lexp = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "\\"),
                            std::make_shared<repetition>(apat),
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "->"),
                            exp
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "let"),
                            decls,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "in"),
                            exp
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "if"),
                            exp_semi_opt,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "then"),
                            exp_semi_opt,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "else"),
                            exp
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "case"),
                            exp,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "of"),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    alts,
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}"),
                                }),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "do"),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                            stmts,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                        }),
                        fexp
                    });


                    infixexp->add_pattern(lexp);
                    infixexp->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr>{
                        lexp,
                        qop,
                        infixexp
                    }));

                    infixexp->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr>{
                        std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM), "-"),
                        infixexp
                    }));

                    auto btype = std::make_shared<repetition>(atype);

                    h_type->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        btype,
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "->"),
                        h_type
                    }));

                    h_type->add_pattern(btype);

                    exp->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        infixexp,
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "::"),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                context,
                                std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "=>"),
                                h_type
                            }),
                            h_type
                        })
                    }));

                    exp->add_pattern(infixexp);

                    final = decl;
                }

                /**
                 * \brief Get the next token.
                 *
                 * Returns the next token from the source code.
                 * @return The match object representing the lexeme found.
                 *         nullptr is returned if an error has occurred.
                 */
                match_ptr next();

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

            private:
                pattern_ptr final;

                lexer::lexer lex;
        };

    }
}

#endif
