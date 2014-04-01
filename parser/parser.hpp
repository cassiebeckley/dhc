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
#include <stack>

#include "../lexer/layout.hpp"



#include <iostream>

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
                parser(icu::UnicodeString source) : source(source), lay(source)
                {
                    // TODO: get rid of this mebbe?
                    using namespace graft::pattern;

                    auto exp = std::make_shared<choice>(std::vector<pattern_ptr>{});
                    auto pat = std::make_shared<choice>(std::vector<pattern_ptr> {});
                    auto infixexp = std::make_shared<choice>(std::vector<pattern_ptr> {});
                    auto decls = std::make_shared<compound>(std::vector<pattern_ptr>{}, -1, [] (match_ptr m) {return m->children()[1];});
                    auto h_type = std::make_shared<choice>(std::vector<pattern_ptr>{});
                    auto qvar = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::QVARID)),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM)),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }, -1, [] (match_ptr m) {return m->children()[1];})
                    });

                    auto var = std::make_shared<compound>(std::vector<pattern_ptr> {
                        qvar
                    }, -1, [] (match_ptr m) {
                        auto matched = m->children()[0];
                        if (matched->children().size() > 0)
                            return std::shared_ptr<graft::match::match>(nullptr);
                        else
                            return matched;
                    });

                    auto varid = std::make_shared<type>(static_cast<int>(lexer::type::QVARID), -1, [] (match_ptr m) {
                        if (m->children().size() > 0)
                            return match_ptr(nullptr);
                        else
                            return m;
                    });

                    auto qconid = std::make_shared<type>(static_cast<int>(lexer::type::QCONID), -1, [] (match_ptr m) {return m->children()[0];});

                    auto conid = std::make_shared<type>(static_cast<int>(lexer::type::QCONID), -1, [] (match_ptr m) {
                        auto match = m->children()[0];
                        if (match->children().size() > 0)
                            return match_ptr(nullptr);
                        else
                            return match;
                    });

                    auto gconsym = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), ":"),
                        std::make_shared<type>(static_cast<int>(lexer::type::QCONSYM))
                    });

                    auto qcon = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qconid,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            gconsym,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }, -1, [] (match_ptr m) {return m->children()[1];})
                    });

                    auto con = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qcon
                    }, -1, [] (match_ptr m) {
                        if (m->children().size() > 0)
                            return std::shared_ptr<graft::match::match>(nullptr);
                        else
                            return m;
                    });

                    auto qvarop = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM)),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "`"),
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARID)),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "`")
                        }, -1, [] (match_ptr m) {
                            return m->children()[1];
                        })
                    });

                    auto varop = std::make_shared<compound>(std::vector<pattern_ptr> {
                        qvarop
                    }, -1, [] (match_ptr m) {
                        auto matched = m->children()[0];
                        if (matched->children().size() > 0)
                            return match_ptr(nullptr);
                        else
                            return matched;
                    });

                    auto qconop = std::make_shared<choice>(std::vector<pattern_ptr> {
                        gconsym,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "`"),
                            qconid,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "`")
                        }, -1, [] (match_ptr m) {
                            return m->children()[1];
                        })
                    });

                    auto conop = std::make_shared<compound>(std::vector<pattern_ptr> {
                        qconop
                    }, -1, [] (match_ptr m) {
                        auto matched = m->children()[0];
                        if (matched->children().size() > 0)
                            return match_ptr(nullptr);
                        else
                            return matched;
                    });


                    auto qop = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qvarop,
                        qconop
                    });

                    auto op = std::make_shared<choice>(std::vector<pattern_ptr> {
                        varop,
                        conop
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

                    auto aexp_inner = std::make_shared<choice>(std::vector<pattern_ptr> {
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

                    auto aexp = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<exclude>(aexp_inner, qcon),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                            fbind,
                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                fbind
                            })),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                        }),
                        aexp_inner
                    });

                    auto fexp = std::make_shared<repetition>(aexp);

                    auto apat = std::make_shared<choice>(std::vector<pattern_ptr> {
                        var,
                        gcon,
                        std::make_shared<type>(static_cast<int>(lexer::type::LITERAL)),
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "_")
                    });

                    apat->add_pattern(std::make_shared<compound>(std::vector<pattern_ptr> {
                        qvar,
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "@"),
                        apat
                    }));

                    auto integer = std::make_shared<type>(static_cast<int>(lexer::type::LITERAL), -1, [] (match_ptr m) {
                        std::string flat;
                        auto lit = std::dynamic_pointer_cast<lexer::literal>(m);
                        if (lit->lit_type == lexer::literal_type::INTEGER)
                        {
                            return m;
                        }
                        else
                        {
                            return match_ptr(nullptr);
                        }
                    });

                    auto h_float = std::make_shared<type>(static_cast<int>(lexer::type::LITERAL), -1, [] (match_ptr m) {
                        auto lit = std::dynamic_pointer_cast<lexer::literal>(m);
                        if (lit->lit_type == lexer::literal_type::FLOAT)
                            return m;
                        else
                            return match_ptr(nullptr);
                    });

                    auto lpat = std::make_shared<choice>(std::vector<pattern_ptr> {
                        apat,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM), "-"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                integer,
                                h_float
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            gcon,
                            apat,
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
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "="),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    exp,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "where"),
                                    decls
                                }),
                                exp
                            })
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
                            var,
                            apat,
                            std::make_shared<repetition>(apat)
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            pat,
                            varop,
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
                        op,
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            op
                        }))
                    });

                    auto vars = std::make_shared<compound>(std::vector<pattern_ptr> {
                        var,
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            var
                        }))
                    });

                    auto fixity = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "infixl"),
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "infixr"),
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "infix")
                    });

                    auto gtycon = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qconid,
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
                        varid,
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
                            qconid,
                            varid
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            qconid,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            varid,
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
                                    integer,
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
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";"),
                                decl
                            }, -1, [] (match_ptr m) {return m->children()[1];})),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                        }, -1, [] (match_ptr m) {
                            auto children = m->children();
                            auto second = children[1]->children();
                            second.insert(second.begin(), children[0]);
                            return std::make_shared<graft::match::sequence>(-1, second);
                        }),
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}", -1, [] (match_ptr) {return std::make_shared<graft::match::sequence>(-1, std::vector<match_ptr>());})
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

                    auto simpletype = std::make_shared<compound> (std::vector<pattern_ptr> {
                        qconid,
                        std::make_shared<repetition>(std::make_shared<type>(static_cast<int>(lexer::type::QVARID)))
                    });

                    auto fielddecl = std::make_shared<compound> (std::vector<pattern_ptr> {
                        vars,
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "::"),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            h_type,
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM), "!"),
                                // should just be varsym
                                // might not matter in this case
                                // since flatten must == "!"
                                atype
                            })
                        })
                    });

                    auto constr_inner = std::make_shared<choice> (std::vector<pattern_ptr> {
                        btype,
                        std::make_shared<compound> (std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM), "!"),
                            atype
                        })
                    });

                    auto constr = std::make_shared<choice> (std::vector<pattern_ptr> {
                        std::make_shared<compound> (std::vector<pattern_ptr> {
                            con,
                            std::make_shared<repetition> (std::make_shared<choice> (std::vector<pattern_ptr> {
                                std::make_shared<compound> (std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::QVARSYM), "!"),
                                    atype
                                }),
                                atype
                            })),
                        }),
                        std::make_shared<compound> (std::vector<pattern_ptr> {
                            constr_inner,
                            conop,
                            constr_inner
                        }),
                        std::make_shared<compound> (std::vector<pattern_ptr> {
                            con,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound> (std::vector<pattern_ptr> {
                                    fielddecl,
                                    std::make_shared<repetition>(std::make_shared<compound> (std::vector<pattern_ptr> {
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                        fielddecl
                                    })),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                                }),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                            })
                        })
                    });

                    auto constrs = std::make_shared<compound>(std::vector<pattern_ptr> {
                        constr,
                        std::make_shared<repetition>(std::make_shared<compound> (std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "|"),
                            constr
                        }))
                    });

                    auto dclass = qconid;

                    auto deriving = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "deriving"),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            dclass,
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                                std::make_shared<choice>(std::vector<pattern_ptr> {
                                    std::make_shared<compound>(std::vector<pattern_ptr> {
                                        dclass,
                                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                            dclass
                                        })),
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                    }),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                })
                            })
                        })
                    });

                    auto cdecl = std::make_shared<choice>(std::vector<pattern_ptr> {
                        gendecl,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                funlhs,
                                var
                            }),
                            rhs
                        })
                    });

                    auto cdecls = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                        std::make_shared<choice>(std::vector<pattern_ptr>{
                            std::make_shared<compound>(std::vector<pattern_ptr>{
                                cdecl,
                                std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";"),
                                    cdecl
                                })),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                            }),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                        })
                    });

                    auto newconstr = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            con,
                            atype
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            con,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                            var,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "::"),
                            h_type,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                        })
                    });

                    auto simpleclass = std::make_shared<compound>(std::vector<pattern_ptr> {
                        qconid,
                        varid
                    });

                    auto scontext = std::make_shared<choice>(std::vector<pattern_ptr> {
                        simpleclass,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    simpleclass,
                                    std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                        simpleclass
                                    }))
                                }),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                            })
                        })
                    });

                    auto inst = std::make_shared<choice>(std::vector<pattern_ptr> {
                        gtycon,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            gtycon,
                            varid,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            varid,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                            varid,
                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                varid
                            })),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "["),
                            varid,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "]")
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            varid,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "->"),
                            varid,
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                        }),

                    });

                    auto idecl = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            funlhs,
                            var
                        }),
                        rhs
                    });

                    auto idecls = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                        std::make_shared<choice>(std::vector<pattern_ptr>{
                            std::make_shared<compound>(std::vector<pattern_ptr>{
                                idecl,
                                std::make_shared<repetition>(std::make_shared<choice>(std::vector<pattern_ptr> {
                                    std::make_shared<compound>(std::vector<pattern_ptr> {
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";"),
                                        idecl
                                    }),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";"),
                                })),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                            }),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}")
                        })
                    });

                    // TODO: fdecl here

                    auto simpletype_plus_constrs = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            simpletype,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "="),
                            constrs,
                            deriving
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            simpletype,
                            deriving
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            simpletype,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "="),
                            constrs,
                        }),
                        simpletype
                    });

                    auto topdecl = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "type"),
                            simpletype,
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "="),
                            h_type
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "data"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    context,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "=>"),
                                    simpletype_plus_constrs
                                }),
                                simpletype_plus_constrs
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "newtype"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    context,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "=>"),
                                    simpletype
                                }),
                                simpletype
                            }),
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "="),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    newconstr,
                                    deriving
                                }),
                                newconstr
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "class"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    scontext,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "=>"),
                                    conid,
                                }),
                                conid
                            }),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    varid,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "where"),
                                    cdecls
                                }),
                                varid
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "instance"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    scontext,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), "=>"),
                                    qconid,
                                }),
                                qconid
                            }),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    inst,
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "where"),
                                    idecls
                                }),
                                inst
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "default"),
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    h_type,
                                    std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                        h_type
                                    })),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")"),
                                }),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                            }),
                        }),
                        /* TODO: implement FFI, fdecl */
                        decl
                    });

                    auto topdecls = std::make_shared<compound>(std::vector<pattern_ptr> {
                        topdecl,
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";"),
                            topdecl
                        }, -1, [] (match_ptr m) {
                            return m->children()[1];
                        }))
                    }, -1, [] (match_ptr m) {
                        auto children = m->children();
                        std::vector<match_ptr> second = children[1]->children();
                        second.insert(second.begin(), children[0]);
                        return std::make_shared<graft::match::sequence>(-1, second);
                    });

                    auto cname = std::make_shared<choice>(std::vector<pattern_ptr> {
                        var,
                        con
                    });

                    auto import = std::make_shared<choice>(std::vector<pattern_ptr> {
                        var,
                        conid,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            conid,
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), ".."),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                }),
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                                    std::make_shared<choice>(std::vector<pattern_ptr> {
                                        std::make_shared<compound>(std::vector<pattern_ptr> {
                                            cname,
                                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                                cname
                                            })),
                                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                        }),
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                    }),
                                })
                            })
                        }),
                    });

                    auto impspec_inner = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                import,
                                std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                    import
                                })),
                                std::make_shared<choice>(std::vector<pattern_ptr> {
                                    std::make_shared<compound>(std::vector<pattern_ptr> {
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                    }),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                })
                            }),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                }),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                            })
                        })
                    });

                    auto impspec = std::make_shared<choice>(std::vector<pattern_ptr> {
                        impspec_inner,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARID), "hiding"),
                            impspec_inner
                        })
                    });

                    auto impdecl_inner = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::QVARID), "qualified"),
                            qconid
                        }),
                        qconid
                    });

                    auto impdecl = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "import"),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                impdecl_inner,
                                std::make_shared<type>(static_cast<int>(lexer::type::QVARID), "as"),
                                qconid
                            }),
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                impdecl_inner,
                                impspec
                            }),
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                impdecl_inner,
                                std::make_shared<type>(static_cast<int>(lexer::type::QVARID), "as"),
                                qconid,
                                impspec
                            }),

                            impdecl_inner
                        })
                    });

                    auto impdecls = std::make_shared<compound>(std::vector<pattern_ptr> {
                        impdecl,
                        std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";"),
                            impdecl
                        }, -1, [] (match_ptr m) {
                            return m->children()[1];
                        }))
                    }, -1, [] (match_ptr m) {
                        auto children = m->children();
                        auto second = children[1]->children();
                        second.insert(second.begin(), children[0]);
                        return std::make_shared<graft::match::sequence>(-1, second);
                    });

                    auto body = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "{"),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                impdecls,
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ";"),
                                topdecls
                            }, -1, [] (match_ptr m) {
                                std::vector<match_ptr> first = m->children()[0]->children();
                                std::vector<match_ptr> second = m->children()[2]->children();
                                std::vector<match_ptr> final;
                                final.reserve(first.size() + second.size());
                                final.insert(final.end(), first.begin(), first.end());
                                final.insert(final.end(), second.begin(), second.end());
                                return std::make_shared<graft::match::sequence>(-1, final);
                            }),
                            impdecls,
                            topdecls
                        }),
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "}"),
                    });

                    auto h_export = std::make_shared<choice>(std::vector<pattern_ptr> {
                        qvar,
                        qconid,
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            conid,
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                                    std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDOP), ".."),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                }),
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                                    std::make_shared<choice>(std::vector<pattern_ptr> {
                                        std::make_shared<compound>(std::vector<pattern_ptr> {
                                            cname,
                                            std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                                cname
                                            })),
                                            std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                        }),
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                                    }),
                                })
                            })
                        }),
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "module"),
                            qconid
                        })
                    });

                    auto exports = std::make_shared<compound>(std::vector<pattern_ptr> {
                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), "("),
                        std::make_shared<choice>(std::vector<pattern_ptr> {
                            std::make_shared<compound>(std::vector<pattern_ptr> {
                                h_export,
                                std::make_shared<repetition>(std::make_shared<compound>(std::vector<pattern_ptr> {
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                    h_export
                                })),
                                std::make_shared<choice>(std::vector<pattern_ptr> {
                                    std::make_shared<compound>(std::vector<pattern_ptr> {
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                        std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")"),
                                    }),
                                    std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")"),
                                })
                            }),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ","),
                                std::make_shared<type>(static_cast<int>(lexer::type::SPECIAL), ")")
                            })
                        })
                    });

                    auto module = std::make_shared<choice>(std::vector<pattern_ptr> {
                        std::make_shared<compound>(std::vector<pattern_ptr> {
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "module"),
                            std::make_shared<choice>(std::vector<pattern_ptr> {
                                std::make_shared<compound>(std::vector<pattern_ptr> {
                                    qconid,
                                    exports
                                }),
                                qconid
                            }),
                            std::make_shared<type>(static_cast<int>(lexer::type::RESERVEDID), "where"),
                            body
                        }),
                        body
                    });

                    final = module;
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
                 * @return Whether or not end of input has been reached.
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

            private:
                pattern_ptr final;

                icu::UnicodeString source;
                lexer::layout lay;
                std::stack<int> context;
        };

    }
}

#endif
