

//! \file

#ifndef Y_MKL_SOLVE_BISECTION_INCLUDED
#define Y_MKL_SOLVE_BISECTION_INCLUDED 1

#include "y/mkl/solve/zalgo.hpp"

namespace upsylon {

    namespace mkl {

        namespace kernel {

            //! common content
            struct zbis {
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
        class zbis : public zalgo<T>
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
            inline virtual ~zbis() throw() {}

            //! setup
            inline explicit zbis() : zalgo<T>() {}

            //! name
            inline virtual const char * method() const throw() { return kernel::zbis::name; }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve f.b=F(x.b)=0
            template <typename FUNC> inline
            bool run(FUNC &F, triplet_type &x, triplet_type &f)
            {
                static const_type half(0.5);

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

                mutable_type width = fabs_of(x.c-x.a);
                while(true)
                {
                    const zseek::sign_t  s_b = zseek::sign_of( f.b = F( x.b=half*(x.a+x.c) ) );
                    if( zseek::__zero__ == s_b)
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
                        std::cerr << "x=" << x << "; f=" << f << std::endl;
                        if(this->stop(width,x))
                            return true;
                        
                    }
                }
                // never get here
                return false;
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(zbis);

        };

    }

}

#endif
