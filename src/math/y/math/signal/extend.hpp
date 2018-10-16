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
            extend_constant, //!< keep constant value
            extend_even,     //!< even function at boundary
            extend_odd,      //!< odd function at boundart
            extend_cyclic    //!< cyclic function
        };

        //! light weight computation
        template <typename T>
        class extend
        {
        public:
            const extend_boundary lower; //!< how to extend lower bound
            const extend_boundary upper; //!< how to extend upper bound

        private:
            const array<T>       &X;
            const array<T>       &Y;

        public:

            //! prepare data
            inline explicit extend(const array<T> &XX, const array<T> &YY, const extend_boundary lo, const extend_boundary up) throw() :
            lower(lo),
            upper(up),
            X( XX ),
            Y( YY )
            {
            }

            //! get the X position
            inline T getX( unit_t i) const
            {
                assert( X.size() == Y.size() );
                assert( X.size() > 1 );
                const unit_t N = X.size();
                if(i<=0)
                {
                    if(lower==extend_cyclic)
                    {
                        const T dX = X[N] - X[1]; assert(dX>0);
                        T ans = 0;
                        --i;
                        while(i<=0)
                        {
                            ans -= dX;
                            i   += N;
                        }
                        return ans + X[i];
                    }
                    else
                    {
                        return X[1]+X[1] - getX(2-i);
                    }
                }
                else if( i>N )
                {
                    if(upper==extend_cyclic)
                    {
                        const T dX = X[N] - X[1]; assert(dX>0);
                        T ans = 0;
                        ++i;
                        while(i>N)
                        {
                            ans += dX;
                            i   -= N;
                        }
                        return ans + X[i];
                    }
                    else
                    {
                        return X[N]+X[N] - getX(N+N-i);
                    }
                }
                else
                {
                    return X[i];
                }
            }

            //! get the Y position
            inline T getY( unit_t i) const
            {
                assert( X.size() == Y.size() );
                assert( X.size() > 1 );
                const unit_t N = X.size();
                if(i<=0)
                {
                    switch(lower)
                    {
                        case extend_cyclic:   --i; while(i<=0) i+=N; return Y[i];
                        case extend_even:     return getY(2-i);
                        case extend_odd:      return Y[1]+Y[1]-getY(2-i);
                        default: break;
                    }
                    return Y[1];
                }
                else if(i>N)
                {
                    switch(upper)
                    {
                        case extend_cyclic:   ++i; while(i>N) i-=N; return Y[i];
                        case extend_even:     return getY(N+N-i);
                        case extend_odd :     return Y[N]+Y[N] - getY(N+N-i);
                        default: break;
                    }
                    return Y[N];
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

