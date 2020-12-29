//! \file

#ifndef Y_FITTING_LAMBDA_INCLUDED
#define Y_FITTING_LAMBDA_INCLUDED 1

#include "y/memory/allocator/global.hpp"
#include "y/core/ipower.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace fitting
        {


            template <typename T>
            class lambdas
            {
            public:
                static const int pmin;
                static const int pmax;

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

                inline T operator[](const int idx) const throw()
                {
                    assert(idx>=pmin); assert(idx<=pmax);
                    return value[idx];
                }

                inline virtual ~lambdas() throw()
                {
                    value += pmin;
                    memory::global::location().release(*(void**)&value,bytes);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(lambdas);
                size_t bytes;
                T     *value;
            };

        }

    }
}

#endif

