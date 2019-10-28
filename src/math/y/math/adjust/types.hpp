
//! \file

#ifndef Y_MATH_ADJUST_TYPES_INCLUDED
#define Y_MATH_ADJUST_TYPES_INCLUDED 1

#include "y/math/types.hpp"
#include "y/ptr/arr.hpp"
#include "y/container/matrix.hpp"
#include "y/math/adjust/variables.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {



            //==================================================================
            //
            //
            //! default types definitions
            //
            //
            //==================================================================
            typedef vector<size_t>         Indices;    //!< global vector of indeices
            typedef accessible<bool>       Flags;      //!< interface to untouched flags
            typedef memory::pooled         Allocator;  //!< memory for Variables-sized container
            typedef vector<bool,Allocator> bVector;    //!< a vector or boolean

            //! C++ fit function arguments
#define Y_ADJUST_FUNCTION_ARGS TL3(T,const Parameters&,const Variables&)

            ////! C++ validate prototype
//#define Y_ADJUST_VALIDATE_ARGS  TL4(Array&,const Flags &,const Variables &,const size_t)

            //==================================================================
            //
            //
            //! template types definitions
            //
            //
            //==================================================================
            template <typename T>
            struct Type
            {
                typedef sequence<T>                           Sequence;   //!< alias
                typedef arr_ptr<Sequence,arc_ptr>             Series;     //!< alias for shared data
                typedef addressable<T>                        Array;      //!< alias for interface
                typedef accessible<T>                         Parameters; //!< alias for interface
                typedef matrix<T>                             Matrix;     //!< matrix
                typedef vector<T,Allocator>                   Vector;     //!< for variables...
                typedef functor<T,Y_ADJUST_FUNCTION_ARGS>     Function;   //!< fit function  prototype
                //typedef functor<bool,Y_ADJUST_VALIDATE_ARGS>  Validate;   //!< do something with variables, return false to modify
                
                //! reconstruct a symetric matrix
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

