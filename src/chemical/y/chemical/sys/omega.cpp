
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include <iomanip>




#include "y/mkl/kernel/apk.hpp"
#include "y/yap/lcm.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        typedef vector<apq,Allocator> qVector;
        typedef arc_ptr<qVector>      qShared;

#if 0
        template <typename ACTORS>
        static inline
        size_t disable(Flags &available, const ACTORS &arr)
        {
            const size_t n=arr.size();
            for(size_t jj=n;jj>0;--jj)
            {
                const Actor   &a = arr[jj];
                const Species &s = a.sp;
                const size_t   j = s.indx;
                assert(true==available[j]);
                available[j] = false;
            }
            return n;
        }


        static inline
        void simplify( qShared &Q )
        {
            const apz fac = yap::lcm::of_denominators( & (*Q)[1], Q->size() );
            tao::mulset(*Q,fac);
        }
#endif

        void System::buildOmega()
        {
            
        }
    }

}
