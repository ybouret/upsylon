
#include "y/jive/lexer.hpp"
#include "y/exception.hpp"
#include "y/core/temporary-link.hpp"

namespace upsylon {

    namespace Jive
    {
        void Lexer::push( Lexical::Unit *unit) throw()
        {
            assert(unit);
            assert(io.size<=0);
            io.push_back(unit);
        }

        Lexer:: ~Lexer() throw()
        {
            db.release_all();
            if(refcount()>0)
            {
                (void) liberate();
            }
        }


        void Lexer:: reset() throw()
        {
            scan = this;
            hist.free();
            io.release();
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
                assert(0==origin);
                if(lexeme)
                {
                    return lexeme;
                }
                else
                {
                    if(directive)
                    {
                        // do something
                        const core::temporary_link<Source> link(source,&origin);
                        assert(0!=origin);
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


        void Lexer:: unget(Lexeme *lexeme) throw()
        {
            assert(lexeme);
            io.push_front(lexeme);
        }

        void Lexer:: unget( Lexemes &lexemes ) throw()
        {
            io.merge_front(lexemes);
        }

        void Lexer:: ready(Source &source, size_t n)
        {
            Lexemes backup;
            backup.swap_with(io);
            try {
                while(n-- > 0)
                {
                    Lexeme *lexeme = get(source);
                    if(!lexeme) break;
                    backup.push_back(lexeme);
                }
                backup.swap_with(io);
            }
            catch(...)
            {
                backup.swap_with(io);
                throw;
            }
        }


        void Lexer:: newPlugin(Plugin *plg)
        {
            assert(plg);
            const Plugin::Pointer  thePlugin  = plg; assert( &(thePlugin->Q)==this);
            const Scanner::Pointer theScanner = plg;
            const string          &name       = *(plg->label);

            if( !ex.insert_by(name,thePlugin))
            {
                throw exception("[%s] multiple plugin [%s]", **label, *name);
            }

            try
            {
                if( !db.insert_by(name,theScanner))
                {
                    throw exception("[%s] plugin [%s] collides with scanner", **label, *name);
                }
            }
            catch(...)
            {
                ex.no(name);
                throw;
            }
        }

        void Lexer:: noPlugin(const string &s) const
        {
            noPlugin(*s);
        }

        void Lexer:: noPlugin(const char   *s) const
        {
            assert(s);
            throw exception("[%s] no plugin <%s>", **label, s);
        }

    }

}
