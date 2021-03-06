
#include "y/jive/lexer.hpp"
#include "y/exception.hpp"
#include "y/momentary/link.hpp"
#include "y/type/aliasing.hpp"

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
            db.release();
            if(refcount()>0)
            {
                (void) liberate();
            }
        }


        void Lexer:: reset() throw()
        {
            counter = 0;
            scan    = this;
            hist.free();
            io.release();
            
        }

        void Lexer:: initialize()
        {
            withhold();
            const Scanner::Pointer self = this;
            try
            {
                if( !db.insert( *label, self) )
                {
                    throw exception("[%s] unexpected initialize failure", **label);
                }
            }
            catch(...)
            {
                (void) liberate();
                throw;
            }
            aliasing::_(dict) = &dictionary;
        }

        Lexical::Scanner & Lexer:: newScanner( const Tag &t )
        {
            Scanner::Pointer handle = new Scanner(t,Scanner::AcceptEOS,&counter);
            if(!db.insert(*(handle->label),handle))
            {
                throw exception("[%s] multiple scanners [%s]", **label, **(handle->label));
            }
            aliasing::_(handle->dict) = &dictionary;
            return *handle;
        }

        void Lexer:: jmp(const Tag &target)
        {
            assert(scan);
            const Scanner::Pointer *ppScan = db.search(*target);
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
                        const momentary_link<Source> link(source,&origin);
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

#if 0
        const Lexeme * Lexer:: peek() const throw()
        {
            return io.tail;
        }

        const Lexeme * Lexer:: last() const throw()
        {
            return io.tail;
        }
#endif

        void Lexer:: unget(Lexeme *lexeme) throw()
        {
            assert(lexeme);
            io.push_front(lexeme);
        }

        void Lexer:: unget(Lexemes &lexemes) throw()
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

            if( !ex.insert(name,thePlugin))
            {
                throw exception("[%s] multiple plugin [%s]", **label, *name);
            }

            try
            {
                if( !db.insert(name,theScanner))
                {
                    throw exception("[%s] plugin [%s] collides with scanner", **label, *name);
                }
            }
            catch(...)
            {
                (void)ex.remove(name);
                throw;
            }
        }

        void Lexer:: noPlugin(const string &s) const
        {
            noPlugin(*s);
        }

       
        static const char nil[] = "nil";
        
        void Lexer:: noPlugin(const char   *s) const
        {
            if(!s) s=nil;
            throw exception("[%s] no plugin <%s>", **label, s);
        }

        void Lexer:: noScanner(const string &s) const
        {
            noScanner(*s);
        }
        
        void Lexer:: noScanner(const char *s) const
        {
            if(!s) s=nil;
            throw exception("[%s] no scanner <%s>", **label, s);
        }
        
        const Lexeme * Lexer:: next(Source &source)
        {
            Lexeme *lexeme = get(source);
            if(lexeme)
            {
                return io.push_front(lexeme);
            }
            else
            {
                return NULL;
            }
        }

        void Lexer:: multipleDictionaryID(const char *id) const
        {
            throw exception("[%s] multiple dictionary '%s'", **label, id);
        }

        const Lexemes & Lexer:: operator*() const throw()
        {
            return io;
        }
    }

}
