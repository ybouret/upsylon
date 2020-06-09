//! \file
#ifndef Y_MATH_ZFIND_INCLUDED
#define Y_MATH_ZFIND_INCLUDED 1

#include "y/math/types.hpp"
#include "y/math/triplet.hpp"

namespace upsylon
{
    namespace math
    {

        //! finding zero
        class zfind
        {
        public:
            static const unsigned __z  = 0x00; //!< zero
            static const unsigned __p  = 0x01; //!< positive
            static const unsigned __n  = 0x02; //!< negative

            static const unsigned _zz  = (__z<<8)|__z; //!< zero|zero

            static const unsigned _zp  = (__z<<8)|__p; //!< zero|positive
            static const unsigned _pz  = (__p<<8)|__z; //!< positive|zero

            static const unsigned _zn  = (__z<<8)|__n; //!< zero|negative
            static const unsigned _nz  = (__n<<8)|__z; //!< negative|zero

            static const unsigned _np  = (__n<<8)|__p; //!< negative|positive
            static const unsigned _pn  = (__p<<8)|__n; //!< positive|negative

            //! get a named version of the constant
            static const char *sign_text(const unsigned s) throw();

            //! get the sign of x
            template <typename T>
            static inline unsigned __sign(const T x) throw()
            {
                return (x<0) ? __n : ( (0<x) ? __p : __z);
            }

            //! find zero with precomputed triplets at 'a' and 'c'
            template <typename T,typename FUNC>
            static inline
            bool run( FUNC &F, triplet<T> &x, triplet<T> &f )
            {
                T w = fabs_of( x.c-x.a);
                while(true)
                {
                    f.b = F( (x.b = T(0.5)*(x.a+x.c) ) );
                    const unsigned ab     = __sign( f.a*f.b );
                    const unsigned bc     = __sign( f.b*f.c );
                    const unsigned status = (ab<<8) | bc;

                    switch(status)
                    {
                        case _zz: // => f.b = 0
                            x.ld(x.b);
                            f.ld(f.b);
                            return true;
                        case _zn: /* FALLTHRU */
                        case _zp: // => f.a = 0
                            x.ld(x.a);
                            f.ld(f.a);
                            return true;

                        case _nz: /* FALLTHRU */
                        case _pz: // => f.c = 0
                            x.ld(x.c);
                            f.ld(f.c);
                            return true;

                        case _np: // f.b same sign than f.c
                            x.c = x.b;
                            f.c = f.b;
                            break;

                        case _pn: // f.b same sign than f.a
                            x.a = x.b;
                            f.a = f.b;
                            break;

                        default:
                            return false;
                    }

                    const T new_w = fabs_of(x.c-x.a);
                    if(new_w>=w) return true;
                    w = new_w;
                }
            }

            //! wrapper
            template <
            typename T,
            typename FUNC>
            struct __call
            {
                T     value; //!< the value to find
                FUNC *pfunc; //!< function

                //! call wrapping
                inline T operator()(const T x)
                {
                    assert(pfunc);
                    return (*pfunc)(x)-value;
                }
            };

            //! throw exception on error
            static void throw_not_bracketed();

            //! return x such that F(x) approx 0
            template <typename T,typename FUNC> static inline
            T run1( FUNC &F, const T a, const T b)
            {
                triplet<T> x = { a,0,b };
                triplet<T> f = { F(x.a), 0, F(x.c) };
                if(!run(F,x,f)) throw_not_bracketed();
                return x.b;
            }

            //! return x such that F(x) approx value
            template <typename T,typename FUNC> static inline
            T run2( const T value, FUNC &F, const T a, const T b)
            {
                __call<T,FUNC> Z = { value, &F };
                return run1<T, __call<T,FUNC> >(Z,a,b);
            }


        };

    }
}

#endif

