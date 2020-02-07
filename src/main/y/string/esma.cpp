#include "y/string/esma.hpp"
#include "y/memory/pooled.hpp"
#include "y/code/round.hpp"
#include <cstring>

namespace upsylon {

    namespace core {

        void esma:: bye() throw()
        {
            if(NULL!=wksp)
            {
                assert(size>0);
                assert(m>0);
                assert(k!=NULL);
                assert( memory::pooled::exists() );
                static memory::allocator &mgr = memory::pooled::location();
                mgr.release(wksp,size);
                m=0;
                k=0;
            }
        }

        esma:: ~esma() throw()
        {
            bye();
        }

        void esma:: release() throw()
        {
            bye();
        }

        esma:: esma() throw() : wksp(0), size(0)
        {

        }

        void esma:: hook(const size_t numChars, const size_t kOffset) throw()
        {
            m = numChars;
            k = (ptrdiff_t *)( static_cast<char *>(wksp) + kOffset );
        }

        void esma:: prepare(const size_t numChars,
                            const size_t charSize)
        {
            assert(charSize>0);
            static memory::pooled &mgr       = memory::pooled::instance();
            const size_t           charBytes = numChars * charSize;
            const size_t           kOffset   = Y_MEMALIGN(charBytes);
            size_t                 required  = kOffset + (numChars*sizeof(ptrdiff_t));
            if(required>size)
            {
                mgr.release(wksp,size);
                wksp = mgr.acquire( (size=required) );
                hook(numChars,kOffset);
            }
            else
            {
                if(required<size)
                {
                    if(mgr.compact(wksp, size, required))
                    {
                        hook(numChars,kOffset);
                    }
                    else
                    {
                        memset(wksp,0,size);
                    }
                }
            }
        }


    }
}
