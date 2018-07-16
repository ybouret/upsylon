//! \file
#ifndef Y_CORE_LOOP_INCLUDED
#define Y_CORE_LOOP_INCLUDED 1

#include "y/os/platform.hpp"

#if 0
#if defined(__ICC) && defined(NDEBUG)
// selector expression is constant for optimized version
#pragma warning(disable:280)
#endif
#endif

/** N > 0 */
#define Y_LOOP_(N,CODE) do                                 \
/*    */    {                                              \
/*    */        const size_t iter_ = (N);                  \
/*    */        assert(iter_>0);                           \
/*    */        {                                          \
/*    */            size_t loop_ = (iter_+7)>>3;           \
/*    */            switch (iter_&7) {                     \
/*    */            case 0: do { { CODE; } /* FALLTHRU */  \
/*    */            case 7:      { CODE; } /* FALLTHRU */  \
/*    */            case 6:      { CODE; } /* FALLTHRU */  \
/*    */            case 5:      { CODE; } /* FALLTHRU */  \
/*    */            case 4:      { CODE; } /* FALLTHRU */  \
/*    */            case 3:      { CODE; } /* FALLTHRU */  \
/*    */            case 2:      { CODE; } /* FALLTHRU */  \
/*    */            case 1:      { CODE; } /* FALLTHRU */  \
/*    */                        } while (--loop_ > 0);     \
/*    */            }                                      \
/*    */        }                                          \
/*    */    } while(0)


/**  N >= 0 */
#define Y_LOOP(N,CODE) do                                 \
/*    */    {                                             \
/*    */        const size_t iter_ = (N);                 \
/*    */        if(iter_>0)                               \
/*    */        {                                         \
/*    */            size_t loop_ = (iter_+7)>>3;          \
/*    */            switch (iter_&7) {                    \
/*    */            case 0: do { { CODE; } /* FALLTHRU */ \
/*    */            case 7:      { CODE; } /* FALLTHRU */ \
/*    */            case 6:      { CODE; } /* FALLTHRU */ \
/*    */            case 5:      { CODE; } /* FALLTHRU */ \
/*    */            case 4:      { CODE; } /* FALLTHRU */ \
/*    */            case 3:      { CODE; } /* FALLTHRU */ \
/*    */            case 2:      { CODE; } /* FALLTHRU */ \
/*    */            case 1:      { CODE; } /* FALLTHRU */ \
/*    */                        } while (--loop_ > 0);    \
/*    */            }                                     \
/*    */        }                                         \
/*    */    } while(0)


/** loop from SHIFT to (N-1+SHIFT), N>=0 */
#define Y_LOOP_FUNC(N,FUNC,SHIFT) do                                     \
{                                                                         \
const size_t iter_ = (N);                                                  \
if(iter_>0)                                                                 \
{                                                                            \
size_t       loop_ = (iter_+0x7)>>0x3;                                        \
const size_t jump_ = iter_&0x7;                                                \
size_t       indx_ = jump_-(0x8-SHIFT);                                         \
switch (jump_) {                                                                 \
case 0: do { { const size_t i0_ = (indx_ +=0x8); { FUNC(i0_); } }  /* FALLTHRU */ \
case 7:      { const size_t i1_ = indx_+0x1;     { FUNC(i1_); } }  /* FALLTHRU */ \
case 6:      { const size_t i2_ = indx_+0x2;     { FUNC(i2_); } }  /* FALLTHRU */ \
case 5:      { const size_t i3_ = indx_+0x3;     { FUNC(i3_); } }  /* FALLTHRU */ \
case 4:      { const size_t i4_ = indx_+0x4;     { FUNC(i4_); } }  /* FALLTHRU */ \
case 3:      { const size_t i5_ = indx_+0x5;     { FUNC(i5_); } }  /* FALLTHRU */ \
case 2:      { const size_t i6_ = indx_+0x6;     { FUNC(i6_); } }  /* FALLTHRU */ \
case 1:      { const size_t i7_ = indx_+0x7;     { FUNC(i7_); } }  /* FALLTHRU */ \
} while (--loop_ > 0);                                                           \
}                                                                               \
}                                                                              \
} while(0)

/** loop from SHIFT to (N-1+SHIFT), N>0 */
#define Y_LOOP_FUNC_(N,FUNC,SHIFT) do                                    \
{                                                                         \
const size_t iter_ = (N);                                                  \
assert(iter_>0);                                                            \
{                                                                            \
size_t       loop_ = (iter_+0x7)>>0x3;                                        \
const size_t jump_ = iter_&0x7;                                                \
size_t       indx_ = jump_-(0x8-SHIFT);                                         \
switch (jump_) {                                                                 \
case 0: do { { const size_t i0_ = (indx_ +=0x8); { FUNC(i0_); } }  /* FALLTHRU */ \
case 7:      { const size_t i1_ = indx_+0x1;     { FUNC(i1_); } }  /* FALLTHRU */ \
case 6:      { const size_t i2_ = indx_+0x2;     { FUNC(i2_); } }  /* FALLTHRU */ \
case 5:      { const size_t i3_ = indx_+0x3;     { FUNC(i3_); } }  /* FALLTHRU */ \
case 4:      { const size_t i4_ = indx_+0x4;     { FUNC(i4_); } }  /* FALLTHRU */ \
case 3:      { const size_t i5_ = indx_+0x5;     { FUNC(i5_); } }  /* FALLTHRU */ \
case 2:      { const size_t i6_ = indx_+0x6;     { FUNC(i6_); } }  /* FALLTHRU */ \
case 1:      { const size_t i7_ = indx_+0x7;     { FUNC(i7_); } }  /* FALLTHRU */ \
} while (--loop_ > 0);                                                           \
}                                                                               \
}                                                                              \
} while(0)


#endif

