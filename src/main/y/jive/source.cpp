
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

        bool Source:: alive()
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


    }

}

