
//! \file

#ifndef Y_MKL_SOLVE_RIDDER_INCLUDED
#define Y_MKL_SOLVE_RIDDER_INCLUDED 1

#include "y/mkl/solve/zalgo.hpp"

namespace upsylon {

    namespace mkl {

        namespace kernel {

            //! common content
            struct zrid {
                static const char name[]; //!< "ridder"
            };
        }

        //______________________________________________________________________
        //
        //
        //! bisection method
        //
        //______________________________________________________________________
        template <typename T>
        class zrid : public zalgo<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                                  //!< aliases
            typedef typename zalgo<T>::triplet_type triplet_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~zrid() throw() {}

            //! setup
            inline explicit zrid() : zalgo<T>() {}

            //! name
            inline virtual const char * method() const throw() { return kernel::zrid::name; }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve f.b=F(x.b)=0
            template <typename FUNC> inline
            bool run(FUNC &F, triplet_type &x, triplet_type &f)
            {
                static const_type half(0.5);
                static const_type one(1);
                static const_type tiny = numeric<mutable_type>::tiny;

                //--------------------------------------------------------------
                // reordering for clamping
                //--------------------------------------------------------------
                if(x.c<x.a)
                {
                    cswap(x.a,x.c);
                    cswap(f.a,f.c);
                }

                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                zseek::sign_t s_a = zseek::__zero__;
                zseek::sign_t s_c = zseek::__zero__;
                switch(this->setup(s_a,s_c,x,f))
                {
                    case zseek::success:      break;
                    case zseek::failure:      return false;
                    case zseek::early_return: return true;
                }
                assert(s_a!=zseek::__zero__);
                assert(s_c!=zseek::__zero__);
                assert(s_a!=s_c);


                mutable_type  width = fabs_of(x.c-x.a);
                mutable_type  xx[4] = { 0 };
                mutable_type  ff[4] = { 0 };
                zseek::sign_t ss[4] = { zseek::__zero__ };
                while(true)
                {
                    //----------------------------------------------------------
                    // take the middle point
                    //----------------------------------------------------------
                    zseek::sign_t s_b = zseek::sign_of( f.b = F( x.b = half * (x.a+x.c) ));
                    if(zseek::__zero__==s_b)
                    {
                        this->exactly(x.b,x,f); return true;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // start populating new values
                        //
                        //------------------------------------------------------
                        assert(s_b!=zseek::__zero__);
                        xx[0] = x.a; ff[0] = f.a; ss[0] = s_a;
                        xx[3] = x.c; ff[3] = f.c; ss[3] = s_c;
                        {
                            //--------------------------------------------------
                            // update ridder's value
                            //--------------------------------------------------
                            const int    eps  = int(s_c)*int(s_b); // compound sign
                            const_type   fbfb = f.b*f.b;           // > 0
                            const_type   fafc = f.a*f.c;           // < 0
                            mutable_type x_r  = x.b - half*width*eps*sqrt_of(one+fafc/(tiny+fbfb-fafc));
                            int          i_r  = 0;
                            if(eps<0)
                            {
                                // between x.b and x.c:
                                xx[1]     = x.b; ff[1] = f.b; ss[1] = s_b;
                                ff[i_r=2] = update(F,x_r,x.b,f.b,x.c,f.c);
                            }
                            else
                            {
                                // between x.a and x.b
                                xx[2] = x.b; ff[2] = f.b; ss[2] = s_b;
                                ff[i_r=1] = update(F,x_r,x.a,f.a,x.c,f.c);
                            }

                            // update sign
                            if( zseek::__zero__ == (ss[i_r] = zseek::sign_of(ff[i_r]) ) )
                            {
                                this->exactly(x_r,x,f); return true;
                            }

                            // final update of ridder's position
                            xx[i_r] = x_r;

                        }
                        assert(xx[0]<=xx[1]); assert(xx[1]<=xx[2]); assert(xx[2]<=xx[3]);

                        //------------------------------------------------------
                        //
                        // find optimal bracketing interval
                        //
                        //------------------------------------------------------
                        int          iopt = -1;
                        {
                            // find a first interval
                            mutable_type wopt = -1;
                            int          i    = 0;
                            for(;i<3;++i)
                            {
                                const int ip = i+1;
                                if(zseek::are_opposite(ss[i],ss[ip]))
                                {
                                    iopt = i;
                                    wopt = fabs_of(xx[ip]-xx[i]);
                                    ++i;
                                    break;
                                }
                            }
                            if(iopt<0) return false; // failure

                            // find a better one?
                            for(;i<3;++i)
                            {
                                const int ip = i+1;
                                if(zseek::are_opposite(ss[i],ss[ip]))
                                {
                                    const mutable_type wtmp = fabs_of(xx[ip]-xx[i]);
                                    if(wtmp<wopt)
                                    {
                                        iopt=i;
                                        wopt=wtmp;
                                    }
                                }
                            }
                        }

                        //------------------------------------------------------
                        //
                        // update triplets
                        //
                        //------------------------------------------------------
                        {
                            const int jopt = iopt+1;
                            x.a = xx[iopt]; f.a = ff[iopt];
                            x.c = xx[jopt]; f.c = ff[jopt];
                        }
                        if(fabs_of(f.a)<fabs_of(f.c))
                        {
                            x.b = x.a;
                            f.b = f.a;
                        }
                        else
                        {
                            x.b = x.c;
                            f.b = f.c;
                        }
                        if(this->stop(width,x))
                            return true;
                    }

                }

            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(zrid);
            template <typename FUNC> static inline
            mutable_type update(FUNC         &F,
                                mutable_type &x,
                                const_type    x_lo,
                                const_type    f_lo,
                                const_type    x_up,
                                const_type    f_up)
            {
                assert(x_lo<=x_up);
                if(x<=x_lo)
                {
                    x=x_lo;
                    return f_lo;
                }
                else
                {
                    if(x>=x_up)
                    {
                        x=x_up;
                        return f_up;
                    }
                    else
                    {
                        return F(x);
                    }
                }
            }


        };

    }

}

#endif
