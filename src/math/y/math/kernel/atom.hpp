//! \file
#ifndef Y_MK_ATOM_INCLUDED
#define Y_MK_ATOM_INCLUDED 1

#include "y/math/types.hpp"
#include "y/type/auto-cast.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon {
    
    namespace math {
        
       

        //! optimized argument conversion
#define Y_MK_ATOM_CAST(TARGET,SOURCE,VALUE) (auto_cast<typename TARGET::type,typename SOURCE::type>::_(VALUE))
        
        //! Algebraic Templated Object Manipulation
        struct atom
        {

            //! utilities
            struct tool
            {
                //! direct copy
                template <typename LHS,typename RHS> static inline
                void copy1D( LHS &lhs, RHS &rhs )
                {
                    assert(lhs.size()<=rhs.size());
                    for(size_t i=lhs.size();i>0;--i)
                    {
                        lhs[i] = Y_MK_ATOM_CAST(LHS,RHS,rhs[i]);
                    }
                }

                //! direct copy
                template <typename LHS,typename RHS> static inline
                void copy2D( LHS &lhs, RHS &rhs )
                {
                    assert(lhs.rows<=rhs.rows);
                    assert(lhs.cols<=rhs.cols);
                    for(size_t i=lhs.rows;i>0;--i)
                    {
                        copy1D(lhs[i],rhs[i]);
                    }
                }

                //! check uniformity
                template <typename LHS, typename T> static inline
                typename LHS::const_type deltaSquaredTo( const LHS &lhs, const T &value )
                {
                    size_t n = lhs.size();
                    vector<typename LHS::mutable_type> deltaSquared(n,as_capacity);
                    while(n>0)
                    {
                        typename LHS::mutable_type d2 = lhs[n] - value;
                        d2 *= d2;
                        deltaSquared.push_back_(d2);
                        --n;
                    }
                    return sorted_sum(deltaSquared);
                }

                //! check differences 1D
                template <typename LHS, typename RHS> static inline
                typename LHS::const_type deltaSquared1D( const LHS &lhs, const RHS &rhs )
                {
                    assert(lhs.size()==rhs.size());
                    size_t n = lhs.size();
                    vector<typename LHS::mutable_type> deltaSquared(n,as_capacity);
                    while(n>0)
                    {
                        typename LHS::mutable_type d2 = lhs[n] - Y_MK_ATOM_CAST(LHS,RHS,rhs[n]);
                        d2 *= d2;
                        deltaSquared.push_back_(d2);
                        --n;
                    }
                    return sorted_sum(deltaSquared);
                }

                //! check differences 2D
                template <typename LHS, typename RHS> static inline
                typename LHS::const_type deltaSquared2D( const LHS &lhs, const RHS &rhs )
                {
                    assert(lhs.rows==rhs.rows);
                    assert(lhs.cols==rhs.cols);
                    assert(lhs.items==rhs.items);

                    const size_t nr = lhs.rows;
                    const size_t nc = lhs.cols;
                    vector<typename LHS::mutable_type> deltaSquared(lhs.items,as_capacity);
                    for(size_t i=nr;i>0;--i)
                    {
                        for(size_t j=nc;j>0;--j)
                        {
                            typename LHS::mutable_type d2 = lhs[i][j] - Y_MK_ATOM_CAST(LHS,RHS,rhs[i][j]);
                            d2 *= d2;
                            deltaSquared.push_back_(d2);
                        }
                    }
                    return sorted_sum(deltaSquared);
                }



            };

#include "atom/def.hpp"
            //==================================================================
            //
            //
            // level 1
            //
            //
            //==================================================================
            


#include "atom/ld.hpp"
#include "atom/set.hpp"
#include "atom/neg.hpp"
#include "atom/add.hpp"
#include "atom/sub.hpp"
#include "atom/mul1.hpp"
#include "atom/dot.hpp"
            
            //==================================================================
            //
            //
            // level 2
            //
            //
            //==================================================================
#include "atom/mul2.hpp"
            
            //==================================================================
            //
            //
            // level 3
            //
            //
            //==================================================================
#include "atom/mul3.hpp"
            
        };
        
        
    }
    
}

#endif

