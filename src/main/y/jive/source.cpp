
#include "y/jive/source.hpp"

namespace upsylon {

    namespace Jive
    {
        Source:: ~Source() throw()
        {
        }


        Source:: Source(Module *m) throw() :
        handle(m),
        cache(),
        history()
        {

        }

        void Source:: push(Module *m)
        {
            assert(m);
            const Module::Handle h(m);
            history.store( MetaModule::from<Module::Handle>(handle) );
            handle = h;

        }

        size_t Source:: depth() const throw()
        {
            return history.size;
        }

        Char  * Source:: get()
        {
            return cache.size ? cache.pop_front() : probe();
        }

        const Char * Source:: peek() const throw()
        {
            assert(cache.size>0);
            return cache.head;
        }

        const Context & Source:: context() const throw()
        {
            if(cache.size)
            {
                return *cache.head;
            }
            else
            {
                return *handle;
            }
        }
        
        Char * Source:: probe()
        {
        PROBE:
            Char *ch = handle->getChar();
            if(!ch && history.size )
            {
                MetaModule *hist = history.query();
                handle  = *hist;
                delete hist;
                goto PROBE;
            }
            return ch;
        }

        void Source:: unget(Char *ch) throw()
        {
            assert(ch);
            cache.push_front(ch);
        }

        void Source:: unget(Char::List &content) throw()
        {
            cache.merge_front(content);
        }

        void Source:: uncpy(const Char::List &content)
        {
            Char::List _(content);
            unget(_);
        }

        size_t Source:: in_cache() const throw()
        {
            return cache.size;
        }

        bool Source:: is_active()
        {
            if(cache.size)
            {
                return true;
            }
            else
            {
                Char *ch = probe();
                if(ch)
                {
                    cache.push_front(ch);
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        void Source:: skip() throw()
        {
            assert(cache.size>0);
            Char::Release( cache.pop_front() );
        }

        void Source:: skip(size_t n) throw()
        {
            assert(cache.size>=n);
            while(n-- > 0 )
            {
                assert(cache.size>0);
                Char::Release( cache.pop_front() );
            }
        }

        void   Source:: newLine() throw()
        {
            handle->newLine();
        }

        
        bool Source:: query(char &C)
        {
            Char *ch = get();
            if(ch)
            {
                C = ch->code;
                Char::Release(ch);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        void Source:: store(char C)
        {
            cache.push_front( Char::Acquire(*handle,C) );
            
        }
    }

}

