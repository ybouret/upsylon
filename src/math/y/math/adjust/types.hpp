
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

            typedef vector<size_t>   Indices;
            typedef accessible<bool> Flags;
            
            template <typename T>
            struct Type
            {

                typedef sequence<T>                           Sequence;   //!< alias
                typedef arr_ptr<Sequence,arc_ptr>             Series;     //!< alias for shared data
                typedef addressable<T>                        Array;      //!< alias for interface
                typedef matrix<T>                             Matrix;     //!< matrix
                typedef functor<T,Y_ADJUST_FUNCTION_PROTO>    Function;   //!< fit function  prototype
                typedef functor<bool,Y_ADJUST_VALIDATE_PROTO> Validate;   //!< do something with variables, return false to modify
                typedef T    (*CFunction)(Y_ADJUST_CFUNCTION_ARGS);       //!< fit CFunction
                typedef bool (*CValidate)(Y_ADJUST_CVALIDATE_ARGS);       //!< fit CValidate

                static inline
                void Regularize( Array &beta, Matrix &alpha, const Flags &used )
                {
                    assert( alpha.rows == alpha.cols );
                    assert( used.size() == alpha.rows );
                    assert( used.size() == beta.size() );

                    const size_t n = alpha.rows;
                    for(size_t j=n;j>0;--j)
                    {
                        Array &alpha_j = alpha[j];
                        if(used[j])
                        {

                        }
                        else
                        {
                            beta[j] = 0;
                            size_t k=n;
                            for(;k>j;--k)
                            {
                                alpha_j[k] = 0;
                            }
                            alpha_j[j] = 1;
                            for(--k;k>0;--k)
                            {
                                alpha_j[k] = 0;
                            }
                        }
                    }
                }

            };

        }

    }

}


#endif

