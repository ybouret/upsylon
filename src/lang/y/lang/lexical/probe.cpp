
#include "y/lang/lexical/scanner.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Lexeme *Scanner:: probe( Source &source, ControlEvent * &event )
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
                        throw exception("%s:%d:%d unexpected '%s'",**(ch->origin),ch->line,ch->column,visible_char[ch->code]);
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
                    assert( source.loaded() >= bestData.size );

                }

                return NULL;
            }

        }

    }

}


