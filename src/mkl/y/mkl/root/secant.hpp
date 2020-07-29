

//! \file

#ifndef Y_MKL_ROOT_SECANT_INCLUDED
#define Y_MKL_ROOT_SECANT_INCLUDED 1

#include "y/mkl/root/zfind.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    namespace mkl {

        namespace kernel {

            //! common content
            struct secant {
                static const char name[]; //!< "secant"
            };
        }

        //______________________________________________________________________
        //
        //
        //! secant/false position
        //
        //______________________________________________________________________
        template <typename T>
        class secant : public zroot<T>
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
            inline virtual ~secant() throw() {}

            //! setup
            inline explicit secant() : zroot<T>() {}

            //! name
            inline virtual const char * method() const throw() { return kernel::secant::name; }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve f.b=F(x.b)=0
            template <typename FUNC> inline
            bool operator()( FUNC &F, triplet_type &x, triplet_type &f )
            {
                // reordering for clamping
                if(x.c<x.a)
                {
                    cswap(x.a,x.c);
                    cswap(f.a,f.c);
                }
                // setup
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
                mutable_type width = x.c - x.a;
                while(true)
                {
                    x.b = clamp(x.a,x.a-f.a/(f.c-f.a)*width,x.c);
                    const zfind::sign_type s_b = zfind::__sign( f.b = F(x.b) );
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
                        const T new_width =  x.c-x.a;
                        if(new_width>=width)
                        {
                            return true;
                        }
                        width = new_width;
                    }
                }
            }

            Y_ZROOT_API()



        private:
            Y_DISABLE_COPY_AND_ASSIGN(secant);
        };

    }
}

#endif
