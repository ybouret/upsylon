#include "y/lang/source.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Lang
    {
#if 0
        Source:: ~Source() throw()
        {
            while( iobuf.size ) cache.store( iobuf.pop_back() );
        }

        Source:: Source(const Input &inp) :
        input(inp),
        iobuf(),
        cache( Manager::instance() )
        {
        }

        void Source:: unget(Char *ch) throw()
        {
            iobuf.push_front(ch);
        }

        void Source:: unget( Token &t ) throw()
        {
            iobuf.merge_front(t);
        }

        void Source:: unget_copy( const Token &t )
        {
            Token tmp(t);
            iobuf.merge_front(tmp);
        }

        Char *Source:: get()
        {
            if(iobuf.size)
            {
                return iobuf.pop_front();
            }
            else
            {
                auto_ptr<Char> pch = cache.make(0);
                char &C = (char &)(pch->code);
                try
                {
                    if( input->query(C) )
                    {
                        return pch.yield();
                    }
                    else
                    {
                        // no more iobuf, no more data
                        return NULL;
                    }
                }
                catch(...)
                {
                    cache.store( pch.yield() );
                    throw;
                }
            }
        }

        void Source:: prefetch(size_t n)
        {
            while(n-->0)
            {
                auto_ptr<Char> pch = cache.make(0);
                char &C = (char &)(pch->code);
                try
                {
                    if( input->query(C) )
                    {
                        iobuf.push_back( pch.yield() );
                    }
                    else return;
                }
                catch(...)
                {
                    cache.store( pch.yield() );
                    throw;
                }
            }
        }
#endif
        
    }
}

