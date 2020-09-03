
#include "y/jive/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            namespace {

                class Linker
                {
                public:
                    Source **h;

                    inline Linker(Source  &source,
                                  Source **handle ) throw() :
                    h(handle)
                    {
                        *h = &source;
                    }

                    inline ~Linker() throw()
                    {
                        *h = NULL;
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Linker);

                };

                struct RuleNode
                {
                    const Rule *rule;
                    RuleNode   *next;
                };

                static const char fn[] = "Jive::Scanner::probe: ";

            }

#define YJS_PRINTLN(CODE) do { if(Scanner::Verbose) { std::cerr << "[" << label << "] " << CODE << std::endl; } } while(false)

            static inline void mustAccept(Token      &token,
                                          const Rule &rule,
                                          Source     &source)
            {
                assert(0==token.size);

                if(!rule.motif->accept(token,source))
                {
                    throw exception("%s<%s> failed to accept stream!",fn,**(rule.label));
                }
                source.uncpy(token);
                if(Scanner::Verbose) { std::cerr << "|_<" << rule.label << "> => '" << token << "'" << std::endl; }
            }

            Unit * Scanner:: probe(Source &source, Directive &directive)
            {
                const Linker      link(source,&origin); assert(origin==&source);
                YJS_PRINTLN("probe " << source.context().tag );
                assert(0==directive);

            //PROBE:
                Char *ch = source.get();
                if(!ch)
                {
                    //------------------------------------------------------
                    //
                    YJS_PRINTLN("<EOS> " << source.context().tag );
                    //
                    //------------------------------------------------------

                    switch(atEOS)
                    {
                        case AcceptEOS:
                            //--------------------------------------------------
                            YJS_PRINTLN("accepted => return NULL");
                            //--------------------------------------------------
                            return NULL; //! probable normal end

                        case RejectEOS: {
                            //--------------------------------------------------
                            YJS_PRINTLN("rejected!!!");
                            //--------------------------------------------------
                            exception excp;
                            source.context().cat(excp).cat("[%s] unexpected end of stream",**label);
                            throw excp;
                        }
                    }
                    throw exception("%scorrupted code at line %d!",fn,__LINE__);
                }
                else
                {
                    //------------------------------------------------------
                    //
                    // get code and node from table
                    //
                    //------------------------------------------------------
                    source.unget(ch);
                    const uint8_t      code = ch->code;
                    const RuleNode    *node = (const RuleNode *)( table[code].head );
                    if(node)
                    {
                        //------------------------------------------------------
                        //
                        YJS_PRINTLN("probe #rule=" << table(code) << " which may start with '" << cchars::visible[code] << "'" );
                        //
                        //------------------------------------------------------

                        //------------------------------------------------------
                        // initialize search
                        //------------------------------------------------------
                        const Rule *bestRule=node->rule;
                        Token       bestUnit;
                        mustAccept(bestUnit,*bestRule,source);

                        //------------------------------------------------------
                        // subsequent searches
                        //------------------------------------------------------
                        for(node=node->next;node;node=node->next)
                        {
                            const Rule *tempRule=node->rule;
                            Token       tempUnit;
                            mustAccept(tempUnit,*tempRule,source);
                            if(tempUnit.size>bestUnit.size)
                            {
                                tempUnit.swap_with(bestUnit);
                                cswap(tempRule,bestRule);
                            }
                        }

                        //------------------------------------------------------
                        // check unit size
                        //------------------------------------------------------
                        if(bestUnit.size<=0)
                        {
                            exception excp;
                            source.context().cat(excp).cat("[%s] all rules starting with '%s' are feeble!",**label,cchars::visible[code]);
                        }

                        YJS_PRINTLN("found <" << bestRule->label << "> = '" << bestUnit << "'");

                        //------------------------------------------------------
                        // update source
                        //------------------------------------------------------
                        assert( source.in_cache() >= bestUnit.size );
                        source.skip(bestUnit.size);

                        //------------------------------------------------------
                        // take action
                        //------------------------------------------------------
                        const Event &event = *(bestRule->event);



                    }
                    else
                    {
                        //------------------------------------------------------
                        YJS_PRINTLN("no rule starting with '" << cchars::visible[code] << "'" );
                        //------------------------------------------------------
                        exception excp;
                        source.context().cat(excp).cat("[%s] no rule starting with '%s'",**label,cchars::visible[code]);
                        throw excp;
                    }
                }



                return 0;
            }


        }

    }

}


