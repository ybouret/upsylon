
#include "y/jive/lexer.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive
    {
        Lexer:: ~Lexer() throw()
        {
            db.release_all();
            if(refcount()>0)
            {
                (void) liberate();
            }
        }

        void Lexer:: initialize()
        {
            withhold();
            const Scanner::Pointer self = this;
            try
            {
                if( !db.insert_by( *label, self) )
                {
                    throw exception("[%s] unexpected initialize failure", **label);
                }
            }
            catch(...)
            {
                (void) liberate();
                throw;
            }
        }

        Lexical::Scanner & Lexer:: newScanner( const Tag &t )
        {
            Scanner::Pointer handle = new Scanner(t);
            if(!db.insert_by(*(handle->label),handle))
            {
                throw exception("[%s] multiple scanners [%s]", **label, **(handle->label));
            }
            return *handle;
        }

        void Lexer:: jmp(const Tag &target)
        {
            assert(scan);
            const Scanner::Pointer *ppScan = db.search_by(*target);
            if(!ppScan)
            {
                throw exception("[%s] no [%s] to go to from [%s]",**label, **target, **(scan->label) );
            }
            scan = & aliasing::_(**ppScan);
        }

        Lexeme * Lexer:: get(Source &source)
        {

        CYCLE:
            if(io.size)
            {
                return io.pop_front();
            }
            else
            {
                Lexical::Directive directive = 0;
                Lexeme            *lexeme    = scan->probe(source,directive);
                if(lexeme)
                {
                    return lexeme;
                }
                else
                {
                    if(directive)
                    {
                        // do something
                        origin = &source;
                        switch( directive->type )
                        {
                            case Lexical::ControlEvent::Jump:
                                jmp(directive->label);
                                break;

                            case Lexical::ControlEvent::Call:
                                try {
                                    hist.push_back(scan);
                                    jmp(directive->label);
                                } catch(...) {
                                    hist.pop_back();
                                    throw;
                                }
                                break;

                            case Lexical::ControlEvent::Back:
                                if(hist.size()<=0)
                                {
                                    throw exception("[%s] no previous call to [%s] when back [%s]",**label,**(scan->label),**(directive->label));
                                }
                                scan = hist.back();
                                hist.pop_back();
                                break;
                        }
                        goto CYCLE;
                    }
                    else
                    {
                        return NULL; // end of file
                    }
                }
            }

        }

    }

}
