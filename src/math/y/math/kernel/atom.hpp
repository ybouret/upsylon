//! \file
#ifndef Y_MK_ATOM_INCLUDED
#define Y_MK_ATOM_INCLUDED 1

#include "y/math/types.hpp"
#include "y/concurrent/scheme/simd.hpp"

namespace upsylon {

    namespace math {

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

            //==================================================================
            //
            // loading
            //
            //==================================================================
            template <typename ARRAY> static inline
            void ld( ARRAY &arr, typename ARRAY::param_type value)
            {
                for(size_t i=arr.size();i>0;--i)
                {
                    arr[i] = value;
                }
            }

            template <typename ARRAY> static inline
            void ld( ARRAY &arr, typename ARRAY::param_type value, concurrent::for_each &loop)
            {
                struct ops
                {
                    ARRAY                      *array_;
                    typename ARRAY::const_type *value_;
                    
                    static inline void call(void *args, parallel &ctx, lockable &)
                    {
                        ops   &params = *static_cast<ops*>(args);
                        ARRAY &a      = *params.array_;
                        size_t offset = 1;
                        size_t length = a.size();
                        ctx.split(length,offset);
                        while(length--)
                        {
                            a[offset++] = *params.value_;
                        }
                    }
                };
                ops params = { &arr, &value };
                loop.run( ops::call, &params );
            }

            


        };

    }

}

#endif

