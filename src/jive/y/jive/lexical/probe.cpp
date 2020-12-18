#include "y/jive/lexical/scanner.hpp"
#include "y/exception.hpp"
#include "y/momentary/link.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            namespace {


                //! mapping scatter node
                struct RuleNode
                {
                    const Rule *rule;
                    RuleNode   *next;
                };


                //! try accept and restore source
                static inline bool tryAccept(const Rule &rule, Token &token, Source &source)
                {
                    assert(0==token.size);
                    if(rule.motif->accept(token,source))
                    {
                        source.uncpy(token);
                        return true;
                    }
                    else
                    {
                        assert(0==token.size);
                        return false;
                    }
                }
                
            }

#define YJS_PRINTLN(CODE) do { if(Scanner::Verbose) { std::cerr << "[" << label << "] " << CODE << std::endl; } } while(false)


            Unit * Scanner:: endOfStream(const Source &source) const
            {
                //--------------------------------------------------------------
                //
                /*--*/ YJS_PRINTLN("<EOS> " << source.context().tag );
                //
                //--------------------------------------------------------------
                switch(atEOS)
                {
                    case RejectEOS: {
                        //------------------------------------------------------
                        /*--*/ YJS_PRINTLN("rejected!!!");
                        //------------------------------------------------------
                        exception excp;
                        source.context().cat(excp).cat("[%s] unexpected end of stream",**label);
                        throw excp;
                    }

                    case AcceptEOS:
                        //------------------------------------------------------
                        /*--*/ YJS_PRINTLN("accepted => return NULL");
                        //------------------------------------------------------
                        break;
                }
                return NULL;
            }




            Unit * Scanner:: probe(Source &source, Directive &directive)
            {
                const core::temporary_link<Source> link(source,&origin);
                assert(0==directive);

            PROBE:
                Char *ch = source.get();
                //--------------------------------------------------------------
                //
                // detect end of stream
                //
                //------------------------------------------------------
                if(!ch) return endOfStream(source);

                //--------------------------------------------------------------
                //
                // get code and node from table
                //
                //--------------------------------------------------------------
                source.unget(ch);
                const uint8_t      code = ch->code;
                const RuleNode    *node = (const RuleNode *)( table[code].head );

                //--------------------------------------------------------------
                //
                // check possible production level-1
                //
                //--------------------------------------------------------------
                if(!node) throw syntaxError(source);


                //--------------------------------------------------------------
                //
                // check possible production level2
                //
                //--------------------------------------------------------------

                //--------------------------------------------------------------
                // initialize search
                //--------------------------------------------------------------
                const Rule *bestRule=NULL;
                Token       bestUnit;
                for(;node;node=node->next)
                {
                    if(tryAccept( *(node->rule), bestUnit,source))
                    {
                        bestRule = node->rule;
                        break;
                    }
                }

                if(!bestRule) throw syntaxError(source);


                //--------------------------------------------------------------
                // subsequent searches
                //--------------------------------------------------------------
                for(node=node->next;node;node=node->next)
                {
                    const Rule *tempRule=node->rule;
                    Token       tempUnit;
                    if(tryAccept(*tempRule,tempUnit,source) && tempUnit.size>bestUnit.size)
                    {
                        tempUnit.swap_with(bestUnit);
                        cswap(tempRule,bestRule);
                    }
                }

                //--------------------------------------------------------------
                // check unit size
                //--------------------------------------------------------------
                if(bestUnit.size<=0)
                {
                    exception excp;
                    source.context().cat(excp).cat("[%s] unexpected failure for <%s> to accept '%s...'!",**label,**(bestRule->label),cchars::visible[code]);
                }


                //--------------------------------------------------------------
                // update source
                //--------------------------------------------------------------
                assert( source.in_cache() >= bestUnit.size );
                source.skip(bestUnit.size);


                //--------------------------------------------------------------
                //
                // process (best) event
                //
                //--------------------------------------------------------------
                const Event &event = *(bestRule->event);

                //--------------------------------------------------------------
                // apply action
                //--------------------------------------------------------------
                assert(origin!=NULL);
                aliasing::_(event.action)(bestUnit);
                

                //--------------------------------------------------------------
                // apply policy
                //--------------------------------------------------------------
                switch(event.kind)
                {

                        //------------------------------------------------------
                        // REGULAR
                        //------------------------------------------------------
                    case Event::Regular: {
                        switch(static_cast<const RegularEvent *>( event.self() )->type)
                        {
                            case RegularEvent::Forward: YJS_PRINTLN("forwarding <" << bestRule->label << "> = '" << bestUnit << "'" );
                                return tokenToUnit(bestUnit,bestRule->label);

                            case RegularEvent::Discard: YJS_PRINTLN("discarding <" << bestRule->label << ">" );
                                break;
                        }
                    } goto PROBE;
                        //------------------------------------------------------
                        // CONTROL
                        //------------------------------------------------------
                    case Event::Control: YJS_PRINTLN("directive  <" << bestRule->label << ">" );
                        directive = static_cast<const ControlEvent *>( event.self() );
                        break;
                }
                assert(Event::Control==event.kind);
                return NULL;
            }

            Unit * Scanner:: tokenToUnit(Token &token, const Tag &which) const
            {
                assert(token.size>0);
                Unit *unit = Unit::Create(*token.head,which,stamp);
                unit->swap_with(token);
                return unit;
            }

            exception Scanner:: syntaxError(Source &source) const
            {
                assert(source.in_cache());

                static const char delim[] = " \t\n\r";

                exception excp;
                {
                    Token     token;

                    //----------------------------------------------------------
                    // put the first char, initialize exception with its context
                    //----------------------------------------------------------
                    token.push_back( source.get() )->cat(excp);

                    //----------------------------------------------------------
                    // try to read until next delim
                    //----------------------------------------------------------
                    bool add_extra = false;
                    while(true)
                    {
                        Char *ch = source.get();
                        if(!ch) break;
                        if( strchr(delim, ch->code) )
                        {
                            source.unget(ch);
                            break;
                        }
                        token.push_back(ch);
                        if(token.size>=32)
                        {
                            add_extra = true;
                            break;
                        }
                    }
                    excp.cat("%s syntax error ",**label);
                    excp << '\'';
                    token.cat(excp);
                    if(add_extra)
                    {
                        excp << "...";
                    }
                    excp << '\'';
                    source.unget(token);
                }
                return excp;
            }

        }

    }

}


