
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
                    const void *rule;
                    RuleNode   *next;
                };
            }

#define YJS_PRINTLN(CODE) do { if(Scanner::Verbose) { std::cerr << "[" << label << "] " << CODE << std::endl; } } while(false)

            Unit * Scanner:: probe(Source &source, Directive &directive)
            {
                static const char fn[] = "Jive::Scanner::probe: ";
                const Linker      link(source,&origin); assert(origin==&source);
                YJS_PRINTLN("probe " << source.context().tag );
                assert(0==directive);

                Char *ch = source.get();
                if(!ch)
                {
                    //------------------------------------------------------
                    YJS_PRINTLN("<EOS> " << source.context().tag );
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
                    // get code and node from table
                    //------------------------------------------------------
                    source.unget(ch);
                    const uint8_t      code = ch->code;
                    const RuleNode    *node = (const RuleNode *)( table[code] );
                    if(node)
                    {
                        //------------------------------------------------------
                        YJS_PRINTLN("probe #rule=" << table(code) << " starting with '" << cchars::visible[code] << "'" );
                        //------------------------------------------------------
                        

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


