
//! \file

#ifndef Y_MKL_FIT_TYPES_INCLUDED
#define Y_MKL_FIT_TYPES_INCLUDED 1

#include "y/mkl/types.hpp"
#include "y/mkl/fit/variables.hpp"
#include "y/ptr/arr.hpp"
#include "y/container/matrix.hpp"

namespace upsylon {
    
    namespace mkl {
        
        namespace Fit {
            
            
            
            //==================================================================
            //
            //
            //! default types definitions
            //
            //
            //==================================================================
            typedef vector<size_t>         Indices;    //!< global vector of indeices
            typedef memory::pooled         Allocator;  //!< memory for Variables-sized container
            typedef vector<bool,Allocator> bVector;    //!< a vector or boolean
            
            //! C++ fit function arguments
#define Y_ADJUST_FUNCTION_ARGS TL3(T,const accessible<T> &,const Variables&)
            
            
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
                typedef matrix<T>                             Matrix;     //!< matrix
                typedef vector<T,Allocator>                   Vector;     //!< for variables...
                typedef functor<T,Y_ADJUST_FUNCTION_ARGS>     Function;   //!< fit function  prototype
                
                //! reconstruct a symmetric matrix
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

