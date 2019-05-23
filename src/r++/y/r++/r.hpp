//! \file
#ifndef Y_R_INCLUDED
#define Y_R_INCLUDED 1

#include "y/sequence/array.hpp"

#include <R.h>
#include <Rinternals.h>

namespace upsylon
{
    struct R
    {
        template <typename T> struct Info;

        template <> struct Info<double> {
            enum { Value = REALSXP };
            static inline double *Get(SEXP Rvec) throw() { return REAL(Rvec); }
        };

        template <> struct Info<int> {
            enum { Value = INTSXP  };
            static inline int *Get(SEXP Rvec) throw() { return INTEGER(Rvec); }
        };

        template <typename T>
        class Vector : public lightweight_array<T>
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef lightweight_array<T> ArrayType;
            typedef Info<mutable_type>   InfoType;

            inline explicit Vector(SEXP Rvec) throw() :
            ArrayType( InfoType::Get(Rvec), length(Rvec) )
            {
            }

            inline virtual ~Vector() throw()
            {
            }


        private:

        };

    };

}

#endif

