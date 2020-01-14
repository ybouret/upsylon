
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
static inline void run( void *args, parallel &ctx, lockable & ) {\
assert(args); Task &task = *static_cast<Task *>(args);\

#define Y_QUARK_TASK_DATA() } };\
Task task = {

#define Y_QUARK_TASK_EXEC(LOOP) }; LOOP.run( task.run, &task )

        //! optimized argument conversion
#define Y_QUARK_CAST(TARGET,SOURCE,VALUE) (auto_cast<typename TARGET::type,typename SOURCE::type>::_(VALUE))

        struct quark {

#include "quark/ld.hpp"

        };

    }
}

#endif

