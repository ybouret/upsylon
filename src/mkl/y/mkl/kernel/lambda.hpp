//! \file

#ifndef Y_MKL_LAMBDAS_INCLUDED
#define Y_MKL_LAMBDAS_INCLUDED 1

#include "y/memory/allocator/global.hpp"
#include "y/core/ipower.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace kernel
        {


            //__________________________________________________________________
            //
            //
            //! precomputed (power of ten) scaling factors for algorithm
            //
            //__________________________________________________________________
            template <typename T>
            class lambdas
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                static const int pmin; //!< 10^pmin   => 0
                static const int pmax; //!< 10^pmax+1 => singularity

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                
                //! setup
                inline explicit lambdas() : bytes( (1+pmax-pmin) * sizeof(T) ),
                value( static_cast<T*>(memory::global::instance().acquire(bytes))-pmin )
                {
                    for(int p=pmin+1;p<0;++p)
                    {
                        static const T fac = T(0.1);
                        value[p] = ipower<T>(fac,-p);
                    }
                    value[0] = 1;
                    for(int p=1;p<=pmax;++p)
                    {
                        static const T fac = T(10.0);
                        value[p] = ipower<T>(fac,p);
                    }
                }

                
                //! cleanup
                inline virtual ~lambdas() throw()
                {
                    value += pmin;
                    memory::global::location().release(*(void**)&value,bytes);
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                
                //! fetch idx in [pmin..pmax]
                inline T operator[](const int idx) const throw()
                {
                    assert(idx>=pmin); assert(idx<=pmax);
                    return value[idx];
                }

                //! return a clamped value of p
                static int clamped(const int p) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(lambdas);
                size_t bytes;
                T     *value;
            };

        }

    }
}

#endif

