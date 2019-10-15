//! \file
#ifndef Y_MK_ATOM_INCLUDED
#define Y_MK_ATOM_INCLUDED 1

#include "y/math/types.hpp"
#include "y/type/auto-cast.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon {
    
    namespace math {
        
        //! followed by members
#define Y_MK_ATOM_OPS_API() \
struct ops {
        
        //! followed by getting variables
#define Y_MK_ATOM_OPS_GET(SELF) \
static inline void call(void *_, parallel &ctx, lockable &access) {\
(void)access;\
ops & SELF = *static_cast<ops*>(_)
        
        
        //! compute the dedicated indices and register variables
#define Y_MK_ATOM_OPS_USE_(LENGTH,CODE) \
size_t __offset = 1;         \
size_t __length = (LENGTH);  \
ctx.split(__length,__offset); \
while(__length--) { const size_t __indx = __offset++; Y_MK_ATOM_##CODE(__indx); }\
} }; \
ops __self ={
        
        //! compute the dedicated indices
#define Y_MK_ATOM_OPS_USE(LENGTH,CODE) Y_MK_ATOM_OPS_USE_(LENGTH,CODE)
        
        //! compute the dedicated indices with some prolog code
#define Y_MK_ATOM_OPS_USE_WITH(PROLOG,LENGTH,CODE) \
PROLOG; Y_MK_ATOM_OPS_USE_(LENGTH,CODE)
        
        //! execute SIMD instructions
#define Y_MK_ATOM_OPS_RUN(LOOP) }; (LOOP).run( ops::call, & __self )

        //! optimized argument conversion
#define Y_MK_ATOM_CAST(TARGET,SOURCE,VALUE) (auto_cast<typename TARGET::type,typename SOURCE::type>::_(VALUE))
        
        //! Algebraic Templated Object Manipulation
        struct atom
        {
            //==================================================================
            //
            //
            // level 1
            //
            //
            //==================================================================
            
            //! direct copy
            template <typename LHS,typename RHS> static inline
            void copy1D( LHS &lhs, const RHS &rhs )
            {
                assert(lhs.size()<=rhs.size);
                for(size_t i=lhs.size;i>0;--i)
                {
                    lhs[i] = Y_MK_ATOM_CAST(LHS,RHS,rhs[i]);
                }
            }
            
            //! direct copy
            template <typename LHS,typename RHS> static inline
            void copy2D( LHS &lhs, const RHS &rhs )
            {
                assert(lhs.rows<=rhs.rows);
                assert(lhs.cols<=rhs.cols);
                for(size_t i=lhs.rows;i>0;--i)
                {
                    copy1D(lhs[i],rhs[i]);
                }
            }

            //! check differences
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

