//! \file
#ifndef Y_MKL_ZFIND_INCLUDED
#define Y_MKL_ZFIND_INCLUDED 1

#include "y/mkl/types.hpp"
#include "y/mkl/triplet.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    namespace mkl {

        //______________________________________________________________________
        //
        //
        //! finding zero routines
        //
        //______________________________________________________________________
        class zfind
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
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
            /**
             bisection with all possible outcomes study.
             \param F a callable type
             \param x an initialized triplet with a and b
             \param f an initialized triple f(x.a)*f(x,b)<=0
             */
            template <typename T,typename FUNC>
            static inline
            bool run( FUNC &F, triplet<T> &x, triplet<T> &f )
            {
                static const T half(0.5);
                T w = fabs_of(x.c-x.a);
                while(true)
                {
                    f.b = F( (x.b = half*(x.a+x.c) ) );
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



            //! throw exception on error
            static void throw_not_bracketed();

            //! return x such that F(x) approx 0
            template <typename T,typename FUNC> static inline
            T get( FUNC &F, const T a, const T b)
            {
                triplet<T> x = { a,0,b };
                triplet<T> f = { F(x.a), 0, F(x.c) };
                if(!run(F,x,f)) throw_not_bracketed();
                return x.b;
            }

            //! wrapper to find F(x) = value
            template <
            typename T,
            typename FUNC>
            struct local_call
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
            
            //! return x such that F(x) approx value
            template <typename T,typename FUNC> static inline
            T get( const T value, FUNC &F, const T a, const T b)
            {
                local_call<T,FUNC> Z = { value, &F };
                return get(Z,a,b);
            }

            //! find zero for a linear part
            /**
             - compute x.b such that 0=f.a+(x.b-x.a)/(x.c-x.a)*(f.c-f.a)
             - return true if that's possible
             */
            template <typename T> static inline
            bool linear(triplet<T> &x, triplet<T> &f) throw()
            {
                const bool     xch = (x.c<x.a);       if(xch) { cswap(x.a,x.c); cswap(f.a,f.c); }
                const unsigned s_a = __sign(f.a);
                const unsigned s_c = __sign(f.c);
                const unsigned who = (s_a<<8) | s_c;
                assert(x.a<=x.c);

                switch(who)
                {
                    case _zz: f.b=0; x.b=T(0.5)*(x.a+x.c); break;

                    case _zp:
                    case _zn: f.b=f.a=0; x.b=x.a; break;

                    case _pz:
                    case _nz: f.b=f.c=0; x.b=x.c; break;

                    case _np: assert(f.a<0); assert(0<f.c);
                        f.b = 0;
                        x.b = clamp(x.a,x.a + (-f.a)/(f.c-f.a) * (x.c-x.a), x.c);
                        break;

                    case _pn: assert(0<f.a);assert(f.c<0);
                        f.b = 0;
                        x.b = clamp(x.a,x.a + (f.a)/(f.a-f.c) * (x.c-x.a), x.c);
                        break;
                        
                    default:
                        return false; //! not bracketed
                }
                assert(x.is_increasing());
                assert(abs_of(f.b)<=0);
                if(xch)
                {
                    cswap(x.a,x.c);
                    cswap(f.a,f.c);
                }
                return true;
            }


            template <typename T,typename FUNC>
            static inline
            bool quad( FUNC &F, triplet<T> &x, triplet<T> &f )
            {
                static const T half(0.5);
                // initialize
                if(x.c<x.a)
                {
                    cswap(x.c,x.a);
                    cswap(f.c,f.a);
                }
                T width = x.c-x.a; assert(width>=0);
                while(true)
                {
                    const unsigned sfa = __sign(f.a);
                    const unsigned sfc = __sign(f.c);
                    const unsigned who = (sfa<<8) | sfc;

                    switch(who)
                    {
                        case _zz:
                            x.b=(x.a+x.c)*half; assert( x.is_increasing() );
                            f.b=0;
                            return true;

                        case _zn:
                        case _zp:
                            x.ld(x.a);
                            f.ld(f.a);
                            return true;

                        case _nz:
                        case _pz:
                            x.ld(x.c);
                            f.ld(f.c);
                            return true;

                        case _np: {
                            // f.a <0, f.c>0, sign(f.c)=1
                            const T den = DeltaPrime(F,x,f); assert(x.is_increasing());
                            const T num = -f.b;
                            const T x_z = clamp(x.a,x.b+width*half*(num*den),x.c);
                            const T f_z = F(x_z);
                            // replace boundary of the same sign
                            switch( __sign(f_z) )
                            {
                                case __z: x.ld(x_z); f.ld(f_z); return true;
                                case __p: x.c= x_z;  f.c= f_z;  break;
                                case __n: x.a= x_z;  f.a= x_z;  break;
                            }
                        } break;

                        case _pn: {
                            //! fa>0, f.c<0, sign(f.c)=-1
                            const T den = DeltaPrime(F,x,f);
                            const T num = f.b;
                            const T x_z = clamp(x.a,x.b+width*half*(num*den),x.c);
                            const T f_z = F(x_z);
                            // replace boundary of the same sign
                            switch( __sign(f_z) )
                            {
                                case __z: x.ld(x_z); f.ld(f_z); return true;
                                case __n: x.c= x_z;  f.c= f_z;  break;
                                case __p: x.a= x_z;  f.a= x_z;  break;
                            }
                        } break;

                        default:
                            return false;
                    }

                    assert( x.is_ordered() );
                    std::cerr << "\tx=" << x << std::endl;
                    std::cerr << "\tf=" << f << std::endl;
                    const T new_width = x.c-x.a;
                    if(new_width>=width)
                    {
                        return true;
                    }
                    width = new_width;
                }
            }

        private:
            template <typename T,typename FUNC> static inline
            T DeltaPrime( FUNC &F, triplet<T> &x, triplet<T> &f )
            {
                return sqrt_of( square_of( f.b = F( x.b=T(0.5)*(x.a+x.c) ) ) - (f.a*f.c) );
            }




        };

    }
}

#endif

