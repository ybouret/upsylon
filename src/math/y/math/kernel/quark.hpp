
//! \file
#ifndef Y_MK_QUARK_INCLUDED
#define Y_MK_QUARK_INCLUDED 1

#include "y/math/types.hpp"
#include "y/type/auto-cast.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/container/matrix.hpp"
#include "y/core/loop.hpp"

namespace upsylon {

    namespace math {

#define Y_QUARK_LOOP(N,FUNC,OFFSET) Y_LOOP_FUNC(N,Y_QUARK_##FUNC,OFFSET)

#define Y_QUARK_TASK_DECL() struct Task {

#define Y_QUARK_TASK_IMPL() \
static inline void run( void *args, parallel &ctx, lockable &access ) {\
assert(args); Task &task = *static_cast<Task *>(args); (void)access;

#define Y_QUARK_TASK_DATA() } };\
Task task = {

#define Y_QUARK_TASK_EXEC(LOOP) }; LOOP.run( task.run, &task )

        //! optimized argument conversion
#define Y_QUARK_CAST(TARGET,SOURCE,VALUE) (auto_cast<typename TARGET::type,typename SOURCE::type>::_(VALUE))

        //! binary methods
#define Y_QUARK_IMPL2(METHOD) \
template <typename TARGET, typename LHS, typename RHS> static inline \
void METHOD( TARGET &tgt, LHS &lhs, RHS &rhs ) {                      \
assert(tgt.size()<=lhs.size());                                        \
assert(tgt.size()<=rhs.size());                                         \
Y_QUARK_LOOP(tgt.size(),METHOD,1);                                       \
}                                                                         \
template <typename TARGET, typename LHS, typename RHS> static inline       \
void METHOD( TARGET &tgt, LHS &lhs, RHS &rhs, concurrent::for_each &loop ) {\
assert(tgt.size()<=lhs.size());                                            \
assert(tgt.size()<=rhs.size());                                           \
Y_QUARK_TASK_DECL()                                                      \
TARGET *tgt;                                                            \
LHS    *lhs;                                                           \
RHS    *rhs;                                                          \
Y_QUARK_TASK_IMPL()                                                  \
TARGET &tgt   = *task.tgt;                                          \
LHS    &lhs   = *task.lhs;                                         \
RHS    &rhs   = *task.rhs;                                        \
size_t offset = 1;                                               \
size_t length = tgt.size();                                     \
ctx.split(length,offset);                                      \
Y_QUARK_LOOP(length,METHOD,offset);                           \
Y_QUARK_TASK_DATA()                                          \
&tgt, &lhs, &rhs                                            \
Y_QUARK_TASK_EXEC(loop);                                   \
}

        struct quark {

#include "quark/ld.hpp"
#include "quark/set.hpp"
#include "quark/add.hpp"
#include "quark/sub.hpp"
        };

    }
}

#endif

