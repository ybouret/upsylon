

//! \file

#ifndef Y_MKL_ROOT_SECANT_INCLUDED
#define Y_MKL_ROOT_SECANT_INCLUDED 1

#include "y/mkl/root/zfind.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    namespace mkl {

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

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve f.b=F(x.b)=0
            template <typename FUNC> inline
            bool operator()( FUNC &F, triplet_type &x, triplet_type &f )
            {
                if(x.c<x.a)
                {
                    cswap(x.a,x.c);
                    cswap(f.a,f.c);
                }
                zfind::sign_type s_a = zfind::is_zero;
                zfind::sign_type s_c = zfind::is_zero;
                switch(this->setup(s_a,s_c,x,f))
                {
                    case zfind::success:      break;
                    case zfind::failure:      return false;
                    case zfind::early_return: return true;
                }
                assert(s_a!=zfind::is_zero);
                assert(s_c!=zfind::is_zero);
                assert(s_a!=s_c);
                mutable_type width = x.c - x.a;
                while(true)
                {
                    x.b = clamp(x.a,x.a-f.a/(f.c-f.a)*width,x.c);
                    const zfind::sign_type s_b = zfind::sign_of( f.b = F(x.b) );
                    if( zfind::is_zero == s_b)
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
                return false;
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(secant);
        };

    }
}

#endif
