//! \file
#ifndef Y_MATH_INTEGRATE_INCLUDED
#define Y_MATH_INTEGRATE_INCLUDED 1

#include "y/math/types.hpp"
#include "y/core/pool.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace math
    {
        namespace kernel
        {
            //! summation for trapeze rule
            template <typename T, typename FUNC, const size_t n>
            struct dyadic_sum
            {
                //! evaluate
                static inline T compute(FUNC &F, const T X, const T D)
                {
                    assert(is_a_power_of_two(n));
                    assert(n>1);
                    static const size_t h=n>>1;
                    return dyadic_sum<T,FUNC,h>::compute(F, X, D) + dyadic_sum<T,FUNC,h>::compute(F, X + (h*D), D);
                }
            };

            //! special case
            template <typename T, typename FUNC>
            struct dyadic_sum<T,FUNC,1>
            {
                //! evaluate
                static inline T compute(FUNC &F, const T X, const T )
                {
                    return F(X);
                }
            };

            //! trapezoidal integration
            template <typename T,typename FUNC,const size_t n>
            static inline T  trpz(const T  s,
                                  const T  a,
                                  const T  w,
                                  FUNC    &F)
            {
                static const size_t iter  = 1 << (n-2);
                const T             delta = w/iter;
                const T             start = a + T(0.5) * delta;
                const T             sum   = kernel::dyadic_sum<T,FUNC,iter>::compute(F, start,delta);
                return T(0.5)*(s+(w*sum)/iter);
            }

        }

        //! functions for integration
        struct integrate
        {


            //! prolog of quad step
#define Y_INTG_PROLOG(N)                    \
st = kernel::trpz<T,FUNC,N>(st,a,w,F);      \
s = ( T(4.0) * st - old_st )/T(3.0)

            //! epilog for quad step
#define Y_INTG_EPILOG()                     \
old_st  = st;                               \
old_s   = s
            //! check convergence in quad step
#define Y_INTG_CHECK()                      \
if( __fabs(s-old_s) <= __fabs( ftol * s ) ) \
return true
            //! warm up step
#define Y_INTG_FAST(N)                      \
Y_INTG_PROLOG(N);                           \
Y_INTG_EPILOG()
            
            //! convergence checking step
#define Y_INTG_TEST(N)                      \
Y_INTG_PROLOG(N);                           \
Y_INTG_CHECK();                             \
Y_INTG_EPILOG()

            //! Simpson's quadrature
            template <typename T,typename FUNC> static inline
            bool quad( T &s, FUNC &F, const T a, const T b, const T ftol )
            {
                const T w       = b-a;
                T       st      = T(0.5) * w * (F(b)+F(a));
                T       old_st  = st;
                s               = st;
                T       old_s   = s;


                Y_INTG_FAST(2); // +1 : 3  evals
                Y_INTG_FAST(3); // +2 : 5  evals
                Y_INTG_FAST(4); // +4 : 9  evals
                Y_INTG_FAST(5); // +8 : 17 evals
                Y_INTG_FAST(6); // +16: 33 evals

                Y_INTG_TEST(7);  // +32:  65 evals
                Y_INTG_TEST(8);  // +64:  129 evals
                Y_INTG_TEST(9);  // +128: 257 evals
                Y_INTG_TEST(10); // +256: 513 evals
                Y_INTG_TEST(11); // +512: 1025 evals

                return false;
            }

            template <typename T> class range : public object
            {
            public:
                const T ini;
                const T end;
                T       sum;
                range  *next;
                range  *prev;
                inline explicit range(const T a,const T b) throw() :
                ini(a), end(b), sum(0), next(0), prev(0) {}
                inline virtual ~range() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(range);
            };

            template <typename T,typename FUNC> static inline
            T compute( FUNC &F, const T a, const T b, const T ftol )
            {
                core::pool_of_cpp< range<T> > todo;
                core::list_of_cpp< range<T> > done;
                todo.store( new range<T>(a,b) );
                while(todo.size>0)
                {
                    auto_ptr< range<T> > curr = todo.query();
                    if( quad(curr->sum,F,curr->ini,curr->end,ftol) )
                    {
                        done.push_back( curr.yield() );
                    }
                    else
                    {
                        const T mid = (curr->ini+curr->end)/2;
                        todo.store( new range<T>(mid,curr->end) );
                        todo.store( new range<T>(curr->ini,mid) );
                    }
                }
                assert(done.size>0);
                T sum = 0;
                while(done.size>0)
                {
                    sum += done.head->sum;
                    delete done.pop_front();
                }
                return sum;
            }

        };

    }
}

#endif

