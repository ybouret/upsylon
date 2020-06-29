//! \file

//! LOOP wrapper
#define Y_QUARK_LOOP(N,FUNC,OFFSET) Y_LOOP_FUNC(N,Y_QUARK_##FUNC,OFFSET)

//! SEQUENTIAL LOOP wrapper
#define Y_QUARK_LOOP_SEQ(N,FUNC)    Y_QUARK_LOOP(N,FUNC,1)

//! PARALLEL LOOP wrapper
#define Y_QUARK_LOOP_PAR(N,FUNC)    \
size_t length = (N);                \
size_t offset = 1;                  \
ctx.split(length,offset);           \
Y_QUARK_LOOP(length,FUNC,offset)


//! Task declaration
#define Y_QUARK_TASK_DECL() struct Task {

//! Task implementation
#define Y_QUARK_TASK_IMPL() \
static inline void run( void *args, parallel &ctx, lockable &access ) {\
assert(args); Task &task = *static_cast<Task *>(args); (void)access;

//! Task Data
#define Y_QUARK_TASK_DATA() } };\
Task task = {

//! Execute Task
#define Y_QUARK_TASK_EXEC(LOOP) }; LOOP.run( task.run, &task )


//! optimized argument conversion
#define Y_QUARK_TO(TYPE,SOURCE,VALUE) (auto_cast<TYPE,typename SOURCE::mutable_type>::_(VALUE))

//! optimized argument conversion
#define Y_QUARK_CAST(TARGET,SOURCE,VALUE) (auto_cast<typename TARGET::mutable_type,typename SOURCE::mutable_type>::_(VALUE))

//! binary methods
#define Y_QUARK_IMPL2(METHOD) \
template <typename TARGET, typename LHS, typename RHS> static inline          \
void METHOD( TARGET &tgt, LHS &lhs, RHS &rhs ) {                              \
assert(tgt.size()<=lhs.size());                                               \
assert(tgt.size()<=rhs.size());                                               \
Y_QUARK_LOOP_SEQ(tgt.size(),METHOD);                                          \
}                                                                             \
template <typename TARGET, typename LHS, typename RHS> static inline          \
void METHOD( TARGET &tgt, LHS &lhs, RHS &rhs, concurrent::for_each &loop ) {  \
assert(tgt.size()<=lhs.size());                                               \
assert(tgt.size()<=rhs.size());                                               \
Y_QUARK_TASK_DECL()                                                           \
TARGET *tgt;                                                                  \
LHS    *lhs;                                                                  \
RHS    *rhs;                                                                  \
Y_QUARK_TASK_IMPL()                                                           \
TARGET &tgt   = *task.tgt;                                                    \
LHS    &lhs   = *task.lhs;                                                    \
RHS    &rhs   = *task.rhs;                                                    \
Y_QUARK_LOOP_PAR(tgt.size(),METHOD);                                          \
Y_QUARK_TASK_DATA()                                                           \
&tgt, &lhs, &rhs                                                              \
Y_QUARK_TASK_EXEC(loop);                                                      \
}


//! ternary methods
#define Y_QUARK_IMPL3(METHOD) \
template <typename TARGET, typename LHS, typename RHS> static inline        \
void METHOD( TARGET &tgt,                                                   \
LHS &lhs, typename TARGET::param_type x, RHS &rhs ) {                       \
assert(tgt.size()<=lhs.size());                                             \
assert(tgt.size()<=rhs.size());                                             \
Y_QUARK_LOOP(tgt.size(),METHOD,1);                                          \
}                                                                           \
template <typename TARGET, typename LHS, typename RHS> static inline        \
void METHOD( TARGET &tgt, LHS &lhs, typename TARGET::param_type x, RHS &rhs,\
concurrent::for_each &loop ) {                                              \
assert(tgt.size()<=lhs.size());                                             \
assert(tgt.size()<=rhs.size());                                             \
Y_QUARK_TASK_DECL()                                                         \
TARGET *tgt;                                                                \
LHS    *lhs;                                                                \
typename TARGET::const_type *x;                                             \
RHS    *rhs;                                                                \
Y_QUARK_TASK_IMPL()                                                         \
TARGET                      & tgt = *task.tgt;                              \
LHS                         & lhs = *task.lhs;                              \
typename TARGET::const_type   x   = *task.x;                                \
RHS                         & rhs = *task.rhs;                              \
Y_QUARK_LOOP_PAR(tgt.size(),METHOD);                                        \
{ Y_LOCK(access);  std::cerr << "@" << ctx.size << "." << ctx.rank << ":\t" << binary<typename TARGET::mutable_type>(x) << " :" << offset << "+" << length << std::endl;  }\
Y_QUARK_TASK_DATA()                                                         \
&tgt, &lhs, &x, &rhs                                                        \
Y_QUARK_TASK_EXEC(loop);                                                    \
}
