
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
            bool operator()(FUNC &F, triplet_type &x, triplet_type &f)
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


                mutable_type        width = fabs_of(x.c-x.a);
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
                        // update middle point to Ridder's extrapolation
                        //------------------------------------------------------
                        const int  eps  = int(s_c)*int(s_b); // compound sign
                        const_type fbfb = f.b*f.b;           // > 0
                        const_type fafc = f.a*f.c;           // < 0
                        {
                            mutable_type x_r    = x.b - half*width*eps*sqrt_of(one+fafc/(tiny+fbfb-fafc));
                            if(s_b==s_a)
                            {
                                // between x.b and x.c
                                f.b = update(F,x_r,x.b,f.b,x.c,f.c);
                                x.b = x_r;
                            }
                            else
                            {
                                assert(s_b==s_c);
                                // between x.a and x.b
                                f.b = update(F,x_r,x.a,f.a,x.c,f.c);
                                x.b = x_r;
                            }
                        }

                        //------------------------------------------------------
                        // update status
                        //------------------------------------------------------
                        s_b = zseek::sign_of(f.b);
                        if(zseek::__zero__==s_b)
                        {
                            this->exactly(x.b,x,f); return true;
                        }
                        else
                        {
                            if(s_b==s_a)
                            {
                                x.a = x.b;
                                f.a = f.b;
                            }
                            else
                            {
                                assert(s_b==s_c);
                                x.c = x.b;
                                f.c = f.b;
                            }
                            const_type new_width = fabs_of(x.c-x.a);
                            if(new_width>=width)
                            {
                                return true;
                            }
                            width = new_width;
                        }

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
