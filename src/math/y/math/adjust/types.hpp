
//! \file

#ifndef Y_MATH_ADJUST_TYPES_INCLUDED
#define Y_MATH_ADJUST_TYPES_INCLUDED 1

#include "y/math/types.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/arr.hpp"
#include "y/container/matrix.hpp"
#include "y/math/adjust/variables.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //! C++ fit function prototype
#define Y_ADJUST_FUNCTION_PROTO  TL3(T,const Array&,const Variables&)
            //! C fit function arguments
#define Y_ADJUST_CFUNCTION_ARGS      T,const Array&,const Variables&
            //! C++ validate prototype
#define Y_ADJUST_VALIDATE_PROTO  TL4(Array&,const array<bool>&,const Variables &,const size_t)
            //! C validate arguments
#define Y_ADJUST_CVALIDATE_ARGS      Array&,const array<bool>&,const Variables &,const size_t

            typedef vector<size_t>         Indices;
            typedef accessible<bool>       Flags;
            typedef memory::pooled         Allocator;
            typedef vector<bool,Allocator> bVector;

            template <typename T>
            struct Type
            {
                typedef sequence<T>                           Sequence;   //!< alias
                typedef arr_ptr<Sequence,arc_ptr>             Series;     //!< alias for shared data
                typedef addressable<T>                        Array;      //!< alias for interface
                typedef matrix<T>                             Matrix;     //!< matrix
                typedef vector<T,Allocator>                   Vector;     //!< for variables...
                typedef functor<T,Y_ADJUST_FUNCTION_PROTO>    Function;   //!< fit function  prototype
                typedef functor<bool,Y_ADJUST_VALIDATE_PROTO> Validate;   //!< do something with variables, return false to modify
                typedef T    (*CFunction)(Y_ADJUST_CFUNCTION_ARGS);       //!< fit CFunction
                typedef bool (*CValidate)(Y_ADJUST_CVALIDATE_ARGS);       //!< fit CValidate
                
                
                static inline void Regularize(Matrix &alpha) throw()
                {
                    assert(alpha.is_square);
                    const size_t n = alpha.rows;
                    for(size_t j=n;j>0;--j)
                    {
                        for(size_t k=n;k>j;--k)
                        {
                            alpha[j][k] = alpha[k][j];
                        }
                    }
                }
                

            };

        }

    }

}


#endif

