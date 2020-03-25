
#include "y/jargon/lexical/analyzer.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            
            Analyzer:: ~Analyzer() throw()
            {
                scanners.release();
                (void) liberate();
            }
            
            
            
            
            
            Analyzer:: Analyzer(const string &id) :
            Scanner( new string(id) ),
            current(this),
            units(),
            calls(),
            scanners(),
            dict()
            {
                withhold();
                const Scanner::Handle scan = this;
                try {
                    if( !scanners.insert(scan) )
                    {
                        throw exception("[[%s]] startup failure",**label);
                    }
                }
                catch(...)
                {
                    (void) liberate();
                    throw;
                }
                dict_ = &dict;
            }
            
            void Analyzer:: restart() throw()
            {
                calls.free();
                units.release();
                current = this;
            }
            
            Scanner & Analyzer:: declare(const string &id)
            {
                Scanner::Handle scan = new Scanner(id);;
                if( !scanners.insert(scan) )
                {
                    throw exception("[[%s]] multiple sub-scanner [%s]",**label,**(scan->label));
                }
                scan->dict_ = &dict;
                return *scan;
            }
            
            void Analyzer:: leap(const string &id, const char *when)
            {
                assert(when);
                Scanner::Handle *ppS = scanners.search(id);
                if(!ppS) throw exception("[[%s]] no [%s] to %s",**label,*id,when);
                current = & (**ppS);
            }
            
            Unit * Analyzer:: get(Source &source)
            {
            TRY_GET:
                assert(current);
                if(units.size)
                {
                    return units.pop_front();
                }
                else
                {
                    Directive ctrl = 0;
                    Unit     *unit = current->probe(source,ctrl);
                    if(unit)
                    {
                        return unit;
                    }
                    else
                    {
                        if(ctrl)
                        {
                            switch(ctrl->type)
                            {
                                case ControlEvent::Call:
                                    Y_JSCANNER( std::cerr << '[' << label << ']' << "@call " << '[' << ctrl->suid << ']' << std::endl);
                                    calls.push(current);
                                    leap( *(ctrl->suid),"call");
                                    break;
                                    
                                case ControlEvent::Jump:
                                    Y_JSCANNER( std::cerr << '[' << label << ']' << "@jump " << '[' << ctrl->suid << ']' << std::endl);
                                    leap( *(ctrl->suid),"jump to");
                                    break;
                                    
                                case ControlEvent::Back:
                                    Y_JSCANNER( std::cerr << '[' << label << ']' << "@back" << std::endl);
                                    if( calls.size() <= 0)
                                    {
                                        throw exception("[[%s]] unexpected no previous call!",**label);
                                    }
                                    current = calls.peek(); assert(current);
                                    calls.pop();
                                    break;
                            }
                            goto TRY_GET;
                            
                        }
                        else
                        {
                            return NULL; // End Of Source
                        }
                    }
                }
            }
            
            
        }
        
    }
    
}


