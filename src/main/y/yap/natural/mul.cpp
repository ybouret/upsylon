
#include "y/yap/natural.hpp"
#include "y/code/base2.hpp"
#include "y/fft/fft.hpp"

namespace upsylon
{
    namespace yap
    {

        typedef double          real_t;
        typedef complex<real_t> cplx_t;

        natural natural:: mul(const word_type *lhs, const size_t lnw,
                              const word_type *rhs, const size_t rnw)
        {
            if(lnw>0&&rnw>0)
            {
                static memory_allocator &mgr = instance();

                //--------------------------------------------------------------
                // initialize product memory
                //--------------------------------------------------------------
                const size_t pnw = lnw+rnw;         // words for product
                const size_t np  = pnw * word_size; // space for product
                natural      p(np,as_capacity);    // product

                //--------------------------------------------------------------
                // get common power of two bytes
                //--------------------------------------------------------------
                const size_t nn = p.width; assert( is_a_power_of_two(nn) ); assert(nn>=np);

                //--------------------------------------------------------------
                // get memory to hold arrays
                //--------------------------------------------------------------
                size_t  cplx_count = nn << 1;
                size_t  cplx_bytes = 0;
                size_t  cplx_shift = 0;
                cplx_t *L      = mgr.acquire_field<cplx_t>(cplx_count,cplx_bytes,cplx_shift);
                cplx_t *R      = L+nn;

                //--------------------------------------------------------------
                // packing data
                //--------------------------------------------------------------
                

                mgr.release_field(L,cplx_count,cplx_bytes,cplx_shift);
                p.bytes=np;
                p.update();
                return p;

            }
            else
            {
                return natural();
            }
        }


    }

}
