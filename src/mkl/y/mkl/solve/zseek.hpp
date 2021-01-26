//! \file

#ifndef Y_MKL_SOLVE_ZSEEK_INCLUDED
#define Y_MKL_SOLVE_ZSEEK_INCLUDED 1

#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace mkl
    {

        //______________________________________________________________________
        //
        //
        //! base class for algorithms
        //
        //______________________________________________________________________
        class zseek
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! named sign
            enum sign_t
            {
                negative=-1, //!< f<0
                __zero__=0,  //!< f==0
                positive=1   //!< f>0
            };

            //! setup result
            enum result
            {
                success,       //!< bracketed
                failure,       //!< ill-bracketed
                early_return   //!< one point is zero
            };

            //! true if opposite signs
            static bool are_opposite(const sign_t lhs, const sign_t rhs) throw();

            //! generic sign computation
            template <typename T> static inline
            sign_t sign_of( const T x ) throw()
            {
                return (x<0) ? negative : ( (0<x) ? positive : __zero__ );
            }

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~zseek() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            //! name of the method
            virtual const char *method() const throw() = 0;

        protected:
            explicit zseek() throw();             //!< setup
            void     error_not_bracketed() const; //!< raise exception

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zseek);
        };

    }
}

#endif


