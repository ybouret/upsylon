//! \file

#ifndef Y_MKL_SOLVE_SECANT_INCLUDED
#define Y_MKL_SOLVE_SECANT_INCLUDED 1

#include "y/mkl/solve/zalgo.hpp"

namespace upsylon {

    namespace mkl {

        namespace kernel {

            //! common content
            struct zsec {
                static const char name[]; //!< "bisection"
            };
        }

        //______________________________________________________________________
        //
        //
        //! bisection method
        //
        //______________________________________________________________________
        template <typename T>
        class zsec : public zalgo<T>
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
            inline virtual ~zsec() throw() {}

            //! setup
            inline explicit zsec() : zalgo<T>() {}

            //! name
            inline virtual const char * method() const throw() { return kernel::zsec::name; }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve f.b=F(x.b)=0
            template <typename FUNC> inline
            bool run(FUNC &F, triplet_type &x, triplet_type &f)
            {
                // reordering for clamping
                if(x.c<x.a)
                {
                    cswap(x.a,x.c);
                    cswap(f.a,f.c);
                }

                // initialize
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

                assert(x.c>=x.a);
                mutable_type width = fabs_of(x.c-x.a); //!< round-off security
                std::cerr << "ini x=" << x << "; f=" << f << std::endl;
                while(true)
                {
                    //----------------------------------------------------------
                    // compute secant point: x.b
                    //----------------------------------------------------------
                    mutable_type num = f.a;
                    mutable_type den = f.c - f.a;
                    if(den<=0)
                    {
                        num = -num;
                        den = -den + numeric<T>::tiny;
                    }
                    else
                    {
                        den += numeric<T>::tiny;
                    }
                    x.b = clamp<T>(x.a,x.a-(width*num)/den,x.c);
                    
                    //----------------------------------------------------------
                    // comput f.b and its sign
                    //----------------------------------------------------------
                    const zseek::sign_t s_b = zseek::sign_of( f.b = F(x.b) );
                    std::cerr << "run x=" << x << "; f=" << f << std::endl;
                    if( zseek::__zero__ == s_b)
                    {
                        this->exactly(x.b,x,f);
                        return true;
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
                        std::cerr << "out x=" << x << "; f=" << f << std::endl;
                        if(this->stop(width,x))
                            return true;
                    }
                }
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(zsec);

        };

    }

}

#endif
