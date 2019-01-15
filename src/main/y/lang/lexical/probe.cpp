
#include "y/lang/lexical/scanner.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Lexeme *Scanner:: probe( Source &source, Message &event )
            {
                probed = *source;
                event  = NULL;
                
                while(true)
                {
                    //__________________________________________________________
                    //
                    //
                    // are we done ?
                    //
                    //__________________________________________________________
                    if(! source.active() )
                    {
                        return NULL; //! End Of Source
                    }
                    assert(source.loaded()>0);

                    //__________________________________________________________
                    //
                    //
                    // look up for bestData
                    //
                    //__________________________________________________________
                    Token bestData;
                    Rule *bestRule = rules.head;
                    for(;bestRule;bestRule=bestRule->next)
                    {
                        if(bestRule->motif->match(bestData,source))
                        {
                            // ok, we got a match
                            break;
                        }
                        assert(0==bestData.size);
                    }

                    if(!bestRule)
                    {
                        assert(source.loaded()>0);
                        const Char *ch = source.peek();
                        throw exception("%s:%d:%d unexpected '%s' for [%s]",**(ch->origin),ch->line,ch->column,visible_char[ch->code],**label);
                    }

                    //__________________________________________________________
                    //
                    //
                    // look up for betterData
                    //
                    //__________________________________________________________

                    //----------------------------------------------------------
                    // restore source
                    //----------------------------------------------------------
                    source.ungetCopy(bestData);

                    //----------------------------------------------------------
                    // scan remaining rules
                    //----------------------------------------------------------
                    for(Rule *tempRule=bestRule->next;tempRule;tempRule=tempRule->next)
                    {
                        Token tempData;
                        if(tempRule->motif->match(tempData,source))
                        {
                            if(tempData.size>bestData.size)
                            {
                                //----------------------------------------------
                                // accept
                                //----------------------------------------------
                                source.ungetCopy(tempData);
                                bestData.swap_with(tempData);
                                bestRule = tempRule;
                            }
                            else
                            {
                                //----------------------------------------------
                                // reject
                                //----------------------------------------------
                                source.unget(tempData);
                            }
                        }

                    }
                    assert(bestData.size>0);
                    assert(source.loaded() >= bestData.size);

                    //__________________________________________________________
                    //
                    //
                    // act according to bestRule/bestData
                    //
                    //__________________________________________________________

                    //----------------------------------------------------------
                    // update source
                    //----------------------------------------------------------
                    source.forward( bestData.size );

                    //----------------------------------------------------------
                    // take action
                    //----------------------------------------------------------
                    bestRule->event->action( bestData );

                    //----------------------------------------------------------
                    // check production
                    //----------------------------------------------------------
                    switch(bestRule->event->type)
                    {
                        case Event::Regular: {

                            const RegularEvent *ev = static_cast<const RegularEvent *>(bestRule->handle);
                            switch (ev->type)
                            {
                                    //------------------------------------------
                                case RegularEvent::Forward: { // make a lexeme!
                                    Lexeme *lx = new Lexeme(bestRule->label);
                                    lx->swap_with(bestData);
                                    return lx;
                                }

                                    //------------------------------------------
                                case RegularEvent::Discard: // take next step!!
                                    //-------------------------------------------
                                    break;
                            }
                        } break;

                        case Event::Control:
                            event = static_cast<const ControlEvent *>(bestRule->handle);
                            return NULL;
                    }

                }

            }

        }

    }

}


