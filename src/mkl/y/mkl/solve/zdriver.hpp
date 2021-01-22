//! \file

#ifndef Y_MKL_SOLVE_ZDRIVER_INCLUDED
#define Y_MKL_SOLVE_ZDRIVER_INCLUDED 1

#include "y/mkl/solve/zalgo.hpp"

namespace upsylon
{
    namespace mkl
    {
        //______________________________________________________________________
        //
        //
        //! driver for different algorithms
        //
        //______________________________________________________________________
        template <typename T,
        template <class> class ZALGO>
        class zdriver : public ZALGO<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                                   //!< aliases
            typedef ZALGO<T> algo_type;                            //!< alias
            typedef typename algo_type::triplet_type triplet_type; //!< alias

            //! wrapper to solve F(x) = y
            template <typename FUNC>
            struct zcall
            {
                FUNC      &F; //!< function reference
                const_type y; //!< value to match

                //! call wrapper
                inline mutable_type operator()(param_type x)
                {
                    return F(x)-y;
                }
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            inline explicit zdriver() : algo_type()
            {
            }

            inline virtual ~zdriver() throw()
            {
            }
            
            //__________________________________________________________________
            //
            // wrappers
            //__________________________________________________________________

            //! solve F(x) = 0 between x1 and x2
            template <typename FUNC> inline
            mutable_type operator()(FUNC &F, param_type x1, param_type x2)
            {
                algo_type   &z  = *this;
                triplet_type x  = { x1,0,x2};
                triplet_type f  = { F(x.a), 0, F(x.c) };
                if(!z(F,x,f)) this->error_not_bracketed();
                return x.b;
            }

            //! solve F(x) = y between x1 and x2
            template <typename FUNC> inline
            mutable_type operator()(param_type y, FUNC &F, param_type x1, param_type x2)
            {
                zdriver    &z = *this;
                zcall<FUNC> f = { F, y };
                return z(f,x1,x2);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(zdriver);


        };

    }
}

#endif
