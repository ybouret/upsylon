//! \file

//! V2 METHOD, parallel on rows
#define Y_QUARK_V2(METHOD,CHECK) \
template <typename T, typename LHS, typename RHS> static inline \
void METHOD( LHS &lhs, const matrix<T> &M, RHS &rhs )           \
{ CHECK; Y_QUARK_LOOP_SEQ(M.rows,METHOD);  }                    \
\
template <typename T, typename LHS, typename RHS> static inline                  \
void METHOD( LHS &lhs, const matrix<T> &M, RHS &rhs, concurrent::for_each &loop) \
{ CHECK;                            \
Y_QUARK_TASK_DECL()                 \
LHS             *lhs;               \
const matrix<T> *M;                 \
RHS             *rhs;               \
Y_QUARK_TASK_IMPL()                 \
LHS               &lhs = *task.lhs; \
const matrix<T>   &M   = *task.M;   \
RHS               &rhs = *task.rhs; \
Y_QUARK_LOOP_PAR(M.rows,METHOD);    \
Y_QUARK_TASK_DATA()                 \
&lhs, &M, &rhs                      \
Y_QUARK_TASK_EXEC(loop);            \
}

//! V2 METHOD, parallel on cols for transposed
#define Y_QUARK_V2_TRN(METHOD,CHECK)                            \
template <typename T, typename LHS, typename RHS> static inline \
void METHOD( LHS &lhs, const matrix<T> &M, RHS &rhs )           \
{ CHECK; Y_QUARK_LOOP_SEQ(M.cols,METHOD);  }                    \
\
template <typename T, typename LHS, typename RHS> static inline                 \
void METHOD(LHS &lhs, const matrix<T> &M, RHS &rhs, concurrent::for_each &loop) \
{ CHECK;                            \
Y_QUARK_TASK_DECL()                 \
LHS             *lhs;               \
const matrix<T> *M;                 \
RHS             *rhs;               \
Y_QUARK_TASK_IMPL()                 \
LHS               &lhs = *task.lhs; \
const matrix<T>   &M   = *task.M;   \
RHS               &rhs = *task.rhs; \
Y_QUARK_LOOP_PAR(M.cols,METHOD);    \
Y_QUARK_TASK_DATA()                 \
&lhs, &M, &rhs                      \
Y_QUARK_TASK_EXEC(loop);            \
}
