
#include "y/jargon/lexical/scanner.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            Unit * Scanner::probe(Source &source, Directive &ctrl)
            {
                chars = &source;
                ctrl  = 0;
            PROBE:
                if(source.isEmpty())
                {
                    assert(0==source.io.size);
                    return NULL;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // looking for a first bestRule
                    //
                    //----------------------------------------------------------
                    Cache       &cache    = source.io.cache;
                    const Rule  *bestRule = NULL;
                    Token        bestExpr( cache );
                    
                    for(bestRule=rules.head;bestRule;bestRule=bestRule->next)
                    {
                        if(bestRule->motif->match(bestExpr,source))
                        {
                            break;
                        }
                    }
                    
                    if(!bestRule)
                    {
                        //------------------------------------------------------
                        //
                        // syntax error
                        //
                        //------------------------------------------------------
                        assert(0==bestExpr.size);
                        assert(source.io.size>0);
                        const Char &ctx = *(source.io.head);
                        exception   excp("%s:%d:%d: [%s] unexpected lexical expression starting with '%s'",
                                         **ctx.tag,
                                         ctx.line,
                                         ctx.column,
                                         **label,
                                         cchars::encoded[ctx.code]);
                        throw excp;
                    }
                    
                    size_t bestSize = bestExpr.size;
                    if(bestSize<=0)
                    {
                        throw exception("<%s> corrupted rule '%s' returned an empty token!!!",**label,**(bestRule->label));
                    }
                    source.uncpy(bestExpr);
                    
                    //----------------------------------------------------------
                    //
                    // looking for a overcoming Rule
                    //
                    //----------------------------------------------------------
                    for(const Rule *rule=bestRule->next;rule;rule=rule->next)
                    {
                        Token expr(cache);
                        if(rule->motif->match(expr,source))
                        {
                            const size_t size = expr.size;
                            if(size>bestSize)
                            {
                                // new best
                                source.uncpy(expr);
                                bestSize = size;
                                bestExpr.swap_with(expr);
                                bestRule = rule;
                            }
                            else
                            {
                                // too late
                                source.unget(expr);
                            }
                        }
                        else
                        {
                            assert(0==expr.size);
                        }
                    }
                    
                    //----------------------------------------------------------
                    //
                    // processing bestRule
                    //
                    //----------------------------------------------------------
                    
                    //__________________________________________________________
                    //
                    // skip source bestSize
                    //__________________________________________________________
                    source.skip(bestSize);
                    
                    //__________________________________________________________
                    //
                    // find the proper action
                    //__________________________________________________________
                    const Event &event  = *(bestRule->event);
                    Action      &action = aliasing::_(event.action);
                    action(bestExpr);
                    
                    //__________________________________________________________
                    //
                    // then produce something of control
                    //__________________________________________________________
                    assert(event.self);
                    
                    switch( event.kind )
                    {
                            //--------------------------------------------------
                            //
                            // regular event: forward or discard
                            //
                            //--------------------------------------------------
                        case Event::Regular: {
                            switch( static_cast<const RegularEvent *>(event.self)->type )
                            {
                                case RegularEvent::Forward:
                                {
                                    Y_JSCANNER(std::cerr << '[' << label << ']' << "+<" << bestRule->label << "> '" << bestExpr << "'" << std::endl);
                                    Unit *unit = new Unit(cache,*bestExpr.head,bestRule->label);
                                    unit->swap_with(bestExpr);
                                    return unit;
                                }
                                    
                                case RegularEvent::Discard:
                                {
                                    //Y_JSCANNER(std::cerr << '[' << label << ']' << "/<" << bestRule->label << ">" << std::endl);
                                    goto PROBE;
                                }
                            }
                        } break;
                            
                            //--------------------------------------------------
                            //
                            // control Event
                            //
                            //--------------------------------------------------
                        case Event::Control:
                            ctrl = static_cast<const ControlEvent *>(event.self);
                            return NULL;
                    }
                    throw exception("[[%s]] corrupted scanner code!", **label);
                    
                }
                
            }
            
        }
    }
    
}


