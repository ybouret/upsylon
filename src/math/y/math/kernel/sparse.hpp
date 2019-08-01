#ifndef Y_MATH_KERNEL_SPARSE_INCLUDED
#define Y_MATH_KERNEL_SPARSE_INCLUDED 1

#include "y/sparse/matrix.hpp"
#include "y/sparse/array.hpp"

namespace upsylon
{

    namespace math
    {

        //! SPArse Reusable Kernel
        struct spark
        {
            //! a += b;
            template <typename T, typename U> static inline
            void add( sparse_array<T> &a, const sparse_array<U> &b )
            {
                assert(a.size()==b.size());

#if 0
                typename sparse_array<T>::iterator       ia = a.begin();
                size_t                                   na = a.core.size();
                typename sparse_array<U>::const_iterator ib = b.begin();
                size_t                                   nb = b.core.size();

                while(na>0&&nb>0)
                {


                    --na;++ia;
                    --nb;++ib;
                }
#endif

#if 1
                // add to a the shared indices
                {
                    typename sparse_array<T>::iterator ia = a.begin();
                    size_t na = a.core.size();
                    while(na>0)
                    {
                        const U *pb = b( ia.key() );
                        if(pb) { (**ia).value += (*pb); }
                        --na;++ia;
                    }
                }

                //! set in a the remaining indices
                {
                    size_t                                   nb = b.core.size();
                    typename sparse_array<U>::const_iterator ib = b.begin();
                    while(nb>0)
                    {
                        const size_t k = ib.key();
                        if( ! a(k) )
                        {
                            (void) a(k,(**ib).value);
                        }
                        --nb;++ib;
                    }
                }
#endif


            }

        };

    }
}

#endif

