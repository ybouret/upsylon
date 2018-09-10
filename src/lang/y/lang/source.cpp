#include "y/lang/source.hpp"
#include "y/ptr/auto.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        Source:: ~Source() throw()
        {
            while( iobuf.size ) cache.store( iobuf.pop_back() );
        }

        void Source:: unget(Char *ch) throw()
        {
            iobuf.push_front(ch);
        }

        void Source:: unget( Token &t ) throw()
        {
            iobuf.merge_front(t);
        }

        void Source:: ungetCopy( const Token &t )
        {
            Token tmp(t);
            iobuf.merge_front(tmp);
        }


        static inline
        Module *check_module_addr(Module *m)
        {
            if(!m) throw exception("NULL module for Lang::Source");
            return m;
        }

        Source:: Source(Module *m)  :
        module(check_module_addr(m)),
        iobuf(),
        cache( Manager::instance() )
        {
        }


        Char *Source:: get()
        {
            return (iobuf.size) ? iobuf.pop_front() : module->get();
        }

        void Source:: prefetch(size_t n)
        {
            while(n-->0)
            {
                Char *ch = module->get();
                if(!ch) return;
                iobuf.push_back(ch);
            }
        }

        void Source:: skip()
        {
            assert(iobuf.size>0);
            cache.store( iobuf.pop_front() );
        }

        bool  Source:: isActive()
        {
            if(iobuf.size)
            {
                return true;
            }
            else
            {
                Char *ch = module->get();
                if(ch)
                {
                    iobuf.push_front(ch);
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        bool Source:: peek( char &C )
        {
            if(iobuf.size<=0)
            {
                Char *ch = module->get();
                if(!ch)
                {
                    return false;
                }
                else
                {
                    iobuf.push_front(ch);
                }
            }
            assert(iobuf.size>0);
            C = iobuf.head->code;
            return true;
        }


    }
}

