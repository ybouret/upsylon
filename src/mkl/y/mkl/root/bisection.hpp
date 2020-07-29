
//! \file

#ifndef Y_MKL_ROOT_BISECTION_INCLUDED
#define Y_MKL_ROOT_BISECTION_INCLUDED 1

#include "y/mkl/root/zfind.hpp"

namespace upsylon {

    namespace mkl {

        namespace kernel {

            //! common content
            struct bisection {
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
        class bisection : public zroot<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                                  //!< aliases
            typedef typename zroot<T>::triplet_type triplet_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~bisection() throw() {}

            //! setup
            inline explicit bisection() : zroot<T>() {}

            //! name
            inline virtual const char * method() const throw() { return kernel::bisection::name; }
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve f.b=F(x.b)=0
            template <typename FUNC> inline
            bool operator()(FUNC &F, triplet_type &x, triplet_type &f)
            {
                static const_type half(0.5);

                zfind::sign_type s_a = zfind::__zero__;
                zfind::sign_type s_c = zfind::__zero__;
                switch(this->setup(s_a,s_c,x,f))
                {
                    case zfind::success:      break;
                    case zfind::failure:      return false;
                    case zfind::early_return: return true;
                }
                assert(s_a!=zfind::__zero__);
                assert(s_c!=zfind::__zero__);
                assert(s_a!=s_c);

                mutable_type width = fabs_of(x.c-x.a);
                while(true)
                {
                    const zfind::sign_type s_b = zfind::__sign( f.b = F( x.b=half*(x.a+x.c) ) );
                    if( zfind::__zero__ == s_b)
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
                        const T new_width = fabs_of(x.c-x.a);
                        if(new_width>=width)
                        {
                            return true;
                        }
                        width = new_width;
                    }
                }
                return false;
            }

            Y_ZROOT_API()


        private:
            Y_DISABLE_COPY_AND_ASSIGN(bisection);
        };

    }
}

#endif
