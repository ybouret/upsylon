
#include "y/jargon/lexical/analyzer.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

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
            lexemes(),
            history(),
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
                history.free();
                aliasing::_(lexemes).release();
                current = this;
            }
            
            Scanner & Analyzer:: insert( Scanner::Handle &scan)
            {
                if( !store(scan) )
                {
                    throw exception("[[%s]] multiple sub-scanner [%s]",**label,**(scan->label));
                }
                return *scan;
            }
            
            bool Analyzer:: store( Scanner::Handle &scan )
            {
                if( scanners.insert(scan) )
                {
                    scan->dict_ = &dict;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            
            Plugin & Analyzer:: insert( Plugin *plugin )
            {
                assert(plugin);
                assert(plugin->plug_==plugin);
                Y_JSCANNER( std::cerr << '[' << label << ']' << " <== " << '[' << plugin->label << ']' << std::endl);
                
                Scanner::Handle scan( plugin );
                if(!store(scan))
                {
                    throw exception("[[%s]] multiple plugin [%s]",**label,**(scan->label));
                }
                return *plugin;
            }
            
            Plugin & Analyzer:: extract(const Scanner::Handle *pps, const string &key) const
            {
                if(!pps)  throw exception("[[%s]] has no scanner/plugin [%s]", **label,*key);
                Scanner::Handle scan = *pps;
                Plugin         *plug = scan->plug_;
                if(!plug) throw exception("[[%s]] scanner [%s] is not a plugin",**label,*key);
                return *(scan->plug_);
            }
            
            
            void Analyzer:: leap(const string &id, const char *when)
            {
                assert(when);
                Scanner::Handle *ppS = scanners.search(id);
                if(!ppS) throw exception("[[%s]] no [%s] to %s",**label,*id,when);
                current = & (**ppS);
            }
            
            
            void Analyzer:: unget( Unit *unit ) throw()
            {
                assert(unit);
                aliasing::_(lexemes).push_front(unit);
            }
            
            //const Unit * Analyzer:: peek() const throw() { return units.head; }
            
            
            Unit * Analyzer:: get(Source &source)
            {
            TRY_GET:
                assert(current);
                if(lexemes.size)
                {
                    return aliasing::_(lexemes).pop_front();
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
                                    Y_JSCANNER( std::cerr << '[' << label << ']' << "@call " << '[' << ctrl->label << ']' << std::endl);
                                    history.push(current);
                                    leap( *(ctrl->label),"call");
                                    break;
                                    
                                case ControlEvent::Jump:
                                    Y_JSCANNER( std::cerr << '[' << label << ']' << "@jump " << '[' << ctrl->label << ']' << std::endl);
                                    leap( *(ctrl->label),"jump to");
                                    break;
                                    
                                case ControlEvent::Back:
                                    Y_JSCANNER( std::cerr << '[' << label << ']' << "@back" << std::endl);
                                    if( history.size() <= 0)
                                    {
                                        throw exception("[[%s]] unexpected no previous call!",**label);
                                    }
                                    current = history.peek(); assert(current);
                                    history.pop();
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
            
            
            void Analyzer:: finalize(const Context &ctx)
            {
                assert(current);
                if(this!=current)
                {
                    switch(current->onEOS)
                    {
                        case Scanner::Attached: try
                        {
                            string        data;
                            throw exception("%s:%d: [%s] has unfinished <%s> = '%s...'",
                                            **(ctx.tag),
                                            ctx.line,
                                            **label,**(current->label),*data);
                        } catch(...) { restart(); throw; }
                            
                            
                        case Scanner::Detached:
                            restart();
                            break;
                    }
                }
            }
        }
        
    }
    
}


