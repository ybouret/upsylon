//! \file

#ifndef Y_MKL_ROOT_ZSOLVER_INCLUDED
#define Y_MKL_ROOT_ZSOLVER_INCLUDED

#include "y/mkl/types.hpp"
#include "y/mkl/triplet.hpp"

namespace upsylon {

    namespace mkl {

        //______________________________________________________________________
        //
        //
        //! base class for zroot
        //
        //______________________________________________________________________
        class zfind
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! named sign
            enum sign_type
            {
                is_negative=-1, //!< f<0
                is_zero    =0,  //!< f==0
                is_positive=1   //!< f>0
            };

            //! setup result
            enum result_type
            {
                success,       //!< bracketed
                failure,       //!< ill-bracketed
                early_return   //!< one point is zero
            };

            //! generic sign computation
            template <typename T> static inline
            sign_type sign_of( const T x ) throw()
            {
                return (x<0) ? is_negative : ( (0<x) ? is_positive : is_zero );
            }

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~zfind() throw();  //!< cleanup

        protected:
            explicit zfind() throw();  //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zfind);
        };


        //______________________________________________________________________
        //
        //
        //! base class for generic solvers
        //
        //______________________________________________________________________
        template <typename T>
        class zroot : public zfind
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                        //!< aliases
            typedef triplet<mutable_type> triplet_type; //!< alias


            //__________________________________________________________________
            //
            // static methods
            //__________________________________________________________________

            //! prepare result for an exact zero return
            static inline
            void exactly(param_type z, triplet_type &x, triplet_type &f)
            {
                x.ld(z);
                f.ld(0);
            }

            //! prepare signs and configuration
            static inline
            result_type setup(sign_type    &s_a,
                              sign_type    &s_c,
                              triplet_type &x,
                              triplet_type &f)
            {
                s_a = sign_of(f.a);
                s_c = sign_of(f.c);
                switch(s_a)
                {
                    case is_zero:
                        switch(s_c)
                        {
                            case is_zero: return failure;
                            default     : break;
                        }
                        exactly(x.a, x, f);
                        return early_return;

                    case is_positive:
                        switch(s_c)
                        {
                            case is_zero:     exactly(x.c, x, f); return early_return;
                            case is_positive: return failure;
                            case is_negative: return success;
                        }

                    case is_negative:
                        switch(s_c)
                        {
                            case is_zero:     exactly(x.c, x, f); return early_return;
                            case is_positive: return success;
                            case is_negative: return failure;
                        }
                }
                // never get here...
                return failure;
             }

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~zroot() throw() {}

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            mutable_type  toto; //!< convergence criterion

            

        protected:
            inline explicit zroot() throw() : zfind(), toto(0) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zroot);
        };
    }
}

#endif
