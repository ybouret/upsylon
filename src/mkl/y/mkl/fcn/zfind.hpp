//! \file
#ifndef Y_MKL_ZFIND_INCLUDED
#define Y_MKL_ZFIND_INCLUDED 1

#include "y/mkl/types.hpp"
#include "y/mkl/triplet.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    namespace mkl {

        //! early return at X
#define Y_ZFIND_EARLY_RETURN(X) do { x.ld(X); f.ld(0); return true; } while(false)

        
        
        //! setup triplet F from X and FUNC
#define Y_ZFIND_SETUP(FUNC,X,F) do { F.a = FUNC(X.a); F.b=0; F.c = FUNC(X.c); } while(false)
        
        //______________________________________________________________________
        //
        //
        //! finding zero routines
        //
        //______________________________________________________________________
        struct zfind
        {
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

            static const unsigned _pp  = (__p<<8)|__p; //!< positive|positive
            static const unsigned _nn  = (__n<<8)|__n; //!< positive|positive
            
            //! kind of algorithm
            enum algorithm
            {
                secant,     //!< using the secant/false position method
                ridder,     //!< using the Ridder's method
                bisection   //!< using the bisection method
            };
            
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
            
            //__________________________________________________________________
            //
            // helpers methods
            //__________________________________________________________________

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
             bisection with all possible outcomes study,
             keeping the same sign bracketing all along
             \param F a callable type
             \param x an initialized triplet with a and b
             \param f an initialized triple f(x.a)*f(x.c)<=0
             */
            template <typename T,typename FUNC>
            static inline
            bool _bisection(FUNC &F, triplet<T> &x, triplet<T> &f)
            {
                static const T half(0.5);
                const unsigned s_a   = __sign(f.a); if(__z==s_a) Y_ZFIND_EARLY_RETURN(x.a);
                const unsigned s_c   = __sign(f.c); if(__z==s_c) Y_ZFIND_EARLY_RETURN(x.c);
                if(s_a==s_c) return false; // not bracketed
                T              width = fabs_of(x.c-x.a);
                while(true)
                {
                    const unsigned s_b = __sign( f.b = F( (x.b = half*(x.a+x.c) ) ) );
                    if(__z==s_b)
                    {
                        // early return at x.b
                        Y_ZFIND_EARLY_RETURN(x.b);
                    }
                    else if(s_a==s_b)
                    {
                        // same sign than a
                        x.a = x.b;
                        f.a = f.b;
                    }
                    else
                    {
                        // same sign than c
                        assert(s_c==s_b);
                        x.c = x.b;
                        f.c = f.b;
                    }
                    const T new_w = fabs_of(x.c-x.a);
                    if(new_w>=width) return true;
                    width = new_w;
                }
            }
            
            //! find zero with precomputed triplets at 'a' and 'c'
            /**
             almost quadratic method with Rider's approach
             \param F a callable type
             \param x an initialized triplet with a and b
             \param f an initialized triple f(x.a)*f(x.c)<=0
             */
            template <typename T,typename FUNC> static inline
            bool _secant( FUNC &F, triplet<T> &x, triplet<T> &f )
            {
                //--------------------------------------------------------------
                // initialize triplets and width
                //--------------------------------------------------------------
                if(x.c<x.a)
                {
                    cswap(x.c,x.a);
                    cswap(f.c,f.a);
                }
                const unsigned s_a = __sign(f.a); if(__z==s_a) Y_ZFIND_EARLY_RETURN(x.a);
                const unsigned s_c = __sign(f.c); if(__z==s_c) Y_ZFIND_EARLY_RETURN(x.c);
                if(s_a==s_c) return false;    assert(f.a*f.c<=0); // must be bracketed
                T width = x.c-x.a;            assert(width>=0);
                
                while(true)
                {
                    // sanity check
                    x.b = clamp(x.a,x.a - f.a/(f.c-f.a) * width,x.c);
                    f.b = F(x.b);
                    const unsigned s_b = __sign(f.b);
                    if(__z==s_b)
                    {
                        // early return at x.b
                        Y_ZFIND_EARLY_RETURN(x.b);
                    }
                    else if(s_a==s_b)
                    {
                        // same sign than a
                        x.a = x.b;
                        f.a = f.b;
                    }
                    else
                    {
                        // same sign than c
                        assert(s_c==s_b);
                        x.c = x.b;
                        f.c = f.b;
                    }
                    const T new_w = fabs_of(x.c-x.a);
                    if(new_w>=width) return true;
                    width = new_w;
                }
            }

            //! find zero with precomputed triplets at 'a' and 'c'
            /**
             almost quadratic method with Rider's approach
             \param F a callable type
             \param x an initialized triplet with a and b
             \param f an initialized triple f(x.a)*f(x.c)<=0
             */
            template <typename T,typename FUNC> static inline
            bool _ridder( FUNC &F, triplet<T> &x, triplet<T> &f )
            {
                static const T half(0.5);

                //--------------------------------------------------------------
                // initialize triplets and width
                //--------------------------------------------------------------
                if(x.c<x.a)
                {
                    cswap(x.c,x.a);
                    cswap(f.c,f.a);
                }
                triplet<unsigned> s ={ __sign(f.a), 0, __sign(f.c) };
                if(__z==s.a) Y_ZFIND_EARLY_RETURN(x.a);
                if(__z==s.c) Y_ZFIND_EARLY_RETURN(x.c);
                if(s.a==s.c) return false;    assert(f.a*f.c<=0); // must be bracketed
                T width = x.c-x.a;            assert(width>=0);
                T den   = DeltaPrime(F,x,f);  assert(x.is_increasing());
                if( __z == (s.b=__sign(f.b)) ) Y_ZFIND_EARLY_RETURN(x.b);
                
                //--------------------------------------------------------------
                // loop, with defined values
                //--------------------------------------------------------------
                while(true)
                {
                    // sanity check
                    assert(__z!=s.a);
                    assert(__z!=s.c);
                    assert(__z!=s.b);
                    
                    // compute factor according to the sign of f.c
                    const T factor = (__p==s.c) ? (width*half) : -(width*half);
                    switch( (s.a<<8) | s.c )
                    {
                            //--------------------------------------------------
                            //
                            // works with opposite boundary signs
                            //
                            //--------------------------------------------------
                        case _np:
                        case _pn:
                        {
                            //__________________________________________________
                            //
                            // Ridder's position
                            //__________________________________________________
                            const T        x_z = clamp(x.a,x.b - factor*(f.b)/den,x.c);
                            const T        f_z = F(x_z);
                            const unsigned s_z = __sign(f_z);
                            if(__z==s_z) Y_ZFIND_EARLY_RETURN(x_z);

                            //__________________________________________________
                            //
                            // build the arrays of computed data
                            //__________________________________________________
                            T        xx[4] = { x.a, x.b, x_z, x.c };
                            T        ff[4] = { f.a, f.b, f_z, f.c };
                            unsigned ss[4] = { s.a, s.b, s_z, s.c };

                            //__________________________________________________
                            //
                            // sort them
                            //__________________________________________________
                            if(x_z<x.b)
                            {
                                cswap(xx[1],xx[2]);
                                cswap(ff[1],ff[2]);
                                cswap(ss[1],ss[2]);
                            }
                            assert(xx[0]<=xx[1]); assert(xx[1]<=xx[2]); assert(xx[2]<=xx[3]);
                            assert(ss[0]!=__z);
                            assert(ss[1]!=__z);
                            assert(ss[2]!=__z);
                            assert(ss[3]!=__z);

#if 0
                            std::cerr << "iter=" << ++iter << std::endl;
                            for(size_t i=0;i<4;++i)
                            {
                                std::cerr << "@" << i << ": x=" << xx[i] << ", f=" << ff[i] << ", s=" << ss[i] << std::endl;
                            }
#endif
                            //__________________________________________________
                            //
                            // look for the first bracketing interval
                            //__________________________________________________
                            T      w = -1;
                            size_t j =  0;
                            for(size_t i=0;i<3;++i)
                            {
                                if(ss[i+1]!=ss[i])
                                {
                                    j = i;
                                    w = xx[i+1]-xx[i];
                                    break;
                                }
                            }
                            assert(w>=0);

                            //__________________________________________________
                            //
                            // study next bracketing intervals
                            //__________________________________________________
                            for(size_t i=j+1;i<3;++i)
                            {
                                if(ss[i+1]!=ss[i])
                                {
                                    const T tmp = xx[i+1]-xx[i];
                                    if(tmp<w)
                                    {
                                        j=i;
                                        w=tmp;
                                    }
                                }
                            }
                            //std::cerr << "w=" << w << "@" << j << std::endl;

                            //__________________________________________________
                            //
                            // update boundaries
                            //__________________________________________________
                            const size_t jp=j+1;
                            x.a = xx[j]; x.c = xx[jp];
                            f.a = ff[j]; f.c = ff[jp];
                            s.a = ss[j]; s.c = ss[jp];
                            
                            //__________________________________________________
                            //
                            // update midpoint
                            //__________________________________________________
                            den = DeltaPrime(F,x,f); assert(x.is_increasing());

                            //__________________________________________________
                            //
                            // check convergence in value
                            //__________________________________________________
                            if( __z == (s.b=__sign(f.b) ) )
                            {
                                Y_ZFIND_EARLY_RETURN(x.b);
                            }

                            //__________________________________________________
                            //
                            // check convergence in width
                            //__________________________________________________
                            if(w>=width)
                            {
                                x.ld(x.b);
                                f.ld(f.b);
                                return true;
                            }
                            
                            //__________________________________________________
                            //
                            // update width for next cycle
                            //__________________________________________________
                            width = w;
                        } break;

                            //--------------------------------------------------
                            //
                            // same boundary signs => bad bracket!
                            //
                            //--------------------------------------------------
                        default:
                            return false;
                    }


                }


            }



        private:
            //! compute x.b and f.b and associated discriminant
            template <typename T,typename FUNC> static inline
            T DeltaPrime( FUNC &F, triplet<T> &x, triplet<T> &f )
            {
                static const T half(0.5);
                assert(x.a<=x.c);
                assert(f.a*f.c<=0);
                x.b = half*(x.a+x.c); assert( x.is_increasing() );
                f.b = F(x.b);
                return sqrt_of( square_of(f.b) - (f.a*f.c) );
            }

        public:
            //! throw exception on error
            static void throw_not_bracketed();


            //! wrapper to find zero of a callable object
            template <typename T,typename FUNC>
            static inline
            T get( FUNC &F, const T a, const T c, const algorithm algo)
            {
                triplet<T> x = { a,0, c };
                triplet<T> f = { F(x.a), 0, F(x.c) };
                bool ans = false;
                switch(algo)
                {
                    case bisection: ans = _bisection(F,x,f); break;
                    case ridder:    ans = _ridder(F,x,f);    break;
                    case secant:    ans = _secant(F,x,f);    break;
                }
                if(!ans) throw_not_bracketed();
                return x.b;
            }
 

            //! return x such that F(x) approx value
            template <typename T,typename FUNC> static inline
            T get(const T value, FUNC &F, const T a, const T c, const algorithm algo)
            {
                local_call<T,FUNC> Z = { value, &F };
                return get(Z,a,c,algo);
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




        };



    }
}

#endif

