
#include "y/gfx/filter.hpp"

namespace upsylon
{

    namespace graphic
    {

        namespace crux
        {
            filter:: ~filter() throw()
            {
            }

            filter:: filter(const string &id) : entity(), name(id)
            {
            }

            filter:: filter(const char *id) : entity(), name(id)
            {
            }

            void filter:: suppress(void *&wksp, size_t &wlen) throw()
            {
                if(wksp)
                {
                    assert(wlen>0);
                    static memory::allocator &mgr = memory::dyadic::location();
                    out_of_reach::fill(wksp,0,wlen);
                    mgr.release(wksp,wlen);
                }
                else
                {
                    assert(wlen<=0);
                }
            }

            void * filter:: allocate(memory::embed emb[], const size_t num, size_t &wlen)
            {
                static memory::allocator &mgr = memory::dyadic::instance();
                assert(emb);
                assert(num>0);
                return memory::embed::create(emb, num,mgr,wlen);
            }

        }

    }

}


