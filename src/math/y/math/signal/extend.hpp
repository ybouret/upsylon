//! \file
#ifndef Y_MATH_SIGNAL_EXTEND_INCLUDED
#define Y_MATH_SIGNAL_EXTEND_INCLUDED 1

#include "y/sequence/array.hpp"

namespace upsylon
{
    namespace math
    {
        //! how to extend data
        enum extend_boundary
        {
            extend_constant,
            extend_even,
            extend_odd,
            extend_cyclic
        };

        template <typename T>
        class extend
        {
        public:
            const extend_boundary lower;
            const extend_boundary upper;
        private:
            const unit_t          N;
            const unit_t          N2;
            const array<T>       &X;
            const array<T>       &Y;
            const T               X1;
            const T               XN;
            const T               dX;
            const T               XN2;
            const T               X12;
            const T               Y1;
            const T               YN;
        public:

            //! prepare data
            inline explicit extend(const array<T> &XX, const array<T> &YY, const extend_boundary lo, const extend_boundary up) throw() :
            lower(lo),
            upper(up),
            N( XX.size() ),
            N2(N+N),
            X( XX ),
            Y( YY ),
            X1( X[1] ),
            XN( X[N] ),
            dX( XN-X1 ),
            XN2(XN+XN),
            X12(X1+X1),
            Y1( Y[1] ),
            YN( Y[N] )
            {
                assert(XX.size()==YY.size());
            }

            inline T getX( unit_t i) const
            {
                assert( X.size() == size_t(N) );
                if(i<=0)
                {
                    if(extend_cyclic)
                    {
                        T ans = 0;
                        while(i<=0)
                        {
                            ans -= dX;
                            i   += N;
                        }
                        assert(i<=N);
                        return ans + X[i];
                    }
                    else
                    {
                        return X12 - getX(2-i);
                    }
                }
                else if( i>N )
                {
                    if(extend_cyclic)
                    {
                        T ans = 0;
                        while(i>N)
                        {
                            i-=N;
                            ans+=dX;
                        }
                        assert(i>0);
                        return ans + X[i];
                    }
                    else
                    {
                        return XN2 - getX(N2-i);
                    }
                }
                else
                {
                    return X[i];
                }
            }

            inline T getY( unit_t i) const
            {
                if(i<=0)
                {
                    switch(lower)
                    {
                        case extend_cyclic:   while(i<=0) i+=N; return Y[i];
                        default: break;
                    }
                    return Y1;
                }
                else if(i>N)
                {
                    switch(upper)
                    {
                        case extend_cyclic:   while(i>N) i-=N; return Y[i];
                        default: break;
                    }
                    return YN;
                }
                else
                {
                    return Y[i];
                }
            }


            inline virtual ~extend() throw() {}


            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(extend);

        };
    }
}

#endif

