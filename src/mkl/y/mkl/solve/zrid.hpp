
//! \file

#ifndef Y_MKL_ROOT_RIDDER_INCLUDED
#define Y_MKL_ROOT_RIDDER_INCLUDED 1

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

                //--------------------------------------------------------------
                // take the middle point
                //--------------------------------------------------------------
                const zseek::sign_t s_b = zseek::sign_of( f.b = F( x.b = half * (x.a+x.c) ));
                if(zseek::__zero__==s_b)
                {
                    this->exactly(x.b,x,f); return true;
                }
                else
                {
                    if(s_b==s_a)
                    {
                        // between x.b and x.c
                    }
                    else
                    {
                        assert(s_b==s_c)
                        // between x.a and x.b
                    }
                }
                //const_type sd = sqrt_of( max_of<mutable_type>(0,f.b * f.b - f.a*f.c) );

                return false;
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(zrid);

        };

    }

}

#endif
