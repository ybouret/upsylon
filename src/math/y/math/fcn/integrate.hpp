//! \file
#ifndef Y_MATH_INTEGRATE_INCLUDED
#define Y_MATH_INTEGRATE_INCLUDED 1

#include "y/math/types.hpp"
#include "y/core/pool.hpp"
#include "y/ptr/auto.hpp"
#include "y/core/list.hpp"
#include <cstdlib>
#include "y/exception.hpp"

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

            //! trapezoidal integration, n>=2 for n=1 means full interval
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

            template <typename T>
            static inline int cmp_incr_abs( const void *lhs, const void *rhs)
            {
                assert(lhs); assert(rhs);
                const T L = abs_of( *static_cast<const T *>(lhs) );
                const T R = abs_of( *static_cast<const T *>(rhs) );
                return ( (L<R) ? -1 : ( (R<L) ? 1 : 0 ) );
            }

            template <typename T,typename FUNC,const size_t n>
            static inline T trapezes(const T s,
                                     const T a,
                                     const T w,
                                     FUNC  & F)
            {
                static const T      half  = T(0.5);
                static const size_t iter  = 1 << (n-2);
                T                   value[iter];
                const T             delta = w/iter;
                T                   x     = a + half*delta;
                for(size_t j=0;j<iter;++j,x+=delta)
                {
                    value[j] = F(x);
                }
                qsort(value,iter,sizeof(T),cmp_incr_abs<T>);
                T sum = 0;
                for(size_t j=0;j<iter;++j)
                {
                    sum += value[j];
                }
                return half*(s+(w*sum)/iter);
            }
        }



        //! functions for integration
        struct integrate
        {

            static const size_t min_level = 2;  // 2
            static const size_t max_level = 11; // 2^(max_level-1)-1 calls
            static const size_t max_iters = 1 << (max_level-2);
            static const size_t warmup    = (max_level+min_level)>>1-1;

            //! refine a value, initialized with 0.5*w*(F(a)+F(b))
            template <typename T,typename FUNC> static inline
            T trapezes(const T      s,
                       const T      a,
                       const T      w,
                       FUNC        &F,
                       const size_t level)
            {
                assert(level>=min_level);
                assert(level<=max_level);
                static const T      half  = T(0.5);
                const  size_t       iter  = 1 << (level-2);  // local number of iterations
                T                   value[max_iters];        // local stack

                {
                    const T             delta = w/iter;
                    T                   x     = a + half*delta;
                    for(size_t j=0;j<iter;++j,x+=delta)
                    {
                        value[j] = F(x);
                    }
                }
                qsort(value,iter,sizeof(T),kernel::cmp_incr_abs<T>);
                T sum = 0;
                for(size_t j=0;j<iter;++j)
                {
                    sum += value[j];
                }
                return half*(s+(w*sum)/iter);
            }

            template <typename T, typename FUNC> static inline
            bool try_quad(T &s, FUNC &F, const T a, const T b, const T ftol )
            {
                static const T four  = T(4);
                static const T three = T(3);

                // initialize at level 1
                const T w         = b-a;
                T       sum_trapz = (s=T(0.5) * w * (F(b)+F(a)));
                T       sum_accel = sum_trapz;
                T       old_trapz = sum_trapz;
                T       old_accel = sum_accel;

                for(size_t level=2;level<=max_level;++level)
                {
                    sum_trapz = trapezes(sum_trapz, a, w, F, level);
                    sum_accel = (s=(four*sum_trapz-old_trapz)/three);
                    if(level>warmup)
                    {
                        // test convergences
                    }
                    old_trapz = sum_trapz;
                    old_accel = sum_accel;
                }

                return false;

            }

            //! the integration routine
#define Y_INTG_KERNEL trpz
            //#define Y_INTG_KERNEL trapezoidal

            //! prolog of quad step
#define Y_INTG_PROLOG(N)                             \
st = kernel::Y_INTG_KERNEL<T,FUNC,N>(st,a,w,F);      \
s  = ( T(4.0) * st - old_st )/T(3.0);                \
ds = fabs_of(s-old_s)

            //! epilog for quad step
#define Y_INTG_EPILOG()                     \
old_st  = st;                               \
old_s   = s
            //! check convergence in quad step
#define Y_INTG_CHECK() \
if( ds <= fabs_of( ftol *old_s ) ) { \
return true;\
}


            //! warm up step
#define Y_INTG_FAST(N)                      \
Y_INTG_PROLOG(N);                           \
Y_INTG_EPILOG()
            
            //! convergence checking step
#define Y_INTG_TEST(N)                      \
Y_INTG_PROLOG(N);                           \
Y_INTG_CHECK()                              \
Y_INTG_EPILOG()

            //! Simpson's quadrature
            template <typename T,typename FUNC> static inline
            bool quad( T &s, FUNC &F, const T a, const T b, const T ftol )
            {
                static const T four  = T(4);
                static const T three = T(3);

                const T w       = b-a;
                // initialize sum with trapezoidal
                T       st      = T(0.5) * w * (F(b)+F(a));
                s               = st;
                T       ds      = 0;

                // previous values
                T       old_st  = st;
                T       old_s   = s;

                // while error increases
                size_t n = 2;
                while(true)
                {
                    st = trapezes(st,a,w,F,n);
                    s  = (four*st-old_st)/three;
                    const T    ds_new    = fabs_of(s-old_s);
                    const bool decreased = (ds_new <= ds);
                    //std::cerr << "s=" << old_s << "-> " << s << ", ds=" << ds << "->" << ds_new << std::endl;
                    ds     = ds_new;
                    old_st = st;
                    old_s  = s;
                    if(++n>max_level) return false;
                    if(n>5&&decreased)     break;
                }

                // while error decreases of tolerance reached
                while(true)
                {
                    st = trapezes(st,a,w,F,n);
                    s  = (four*st-old_st)/three;
                    const T    ds_new    = fabs_of(s-old_s);
                    //std::cerr << "s=" << old_s << "-> " << s << ", ds=" << ds << "->" << ds_new << std::endl;
                    if( (ds_new >= ds) ||  ds <= fabs_of( ftol * old_s ) )
                    {
                        return true;
                    }
                    if(++n>max_level) return false;
                    ds     = ds_new;
                    old_st = st;
                    old_s  = s;
                }

#if 0
                Y_INTG_FAST(2); // +1 : 3  evals
                Y_INTG_FAST(3); // +2 : 5  evals
                Y_INTG_FAST(4); // +4 : 9  evals
                Y_INTG_FAST(5); // +8 : 17 evals
                Y_INTG_FAST(6); // +16: 33 evals

                Y_INTG_TEST(7);  // +32:  65 evals
                Y_INTG_TEST(8);  // +64:  129 evals
                Y_INTG_TEST(9);  // +128: 257 evals
                Y_INTG_TEST(10); // +256: 513 evals
                Y_INTG_PROLOG(11);// +512: 1025 evals
                Y_INTG_CHECK();
#endif

#if 0
                {
                    const T err       = fabs_of(s-old_s);       \
                    const T threshold = fabs_of( ftol *old_s ); \
                    fprintf( stderr, "s=%.5e/old_s=%.5e: err=%.5e | thr=%.5e on [%.5e:%.5e]\n", s, old_s, err, threshold,a,b);
                }
#endif
                
                return false;
            }

            //! integration range for adaptive method
            template <typename T> class range : public object
            {
            public:
                const T ini;   //!< start point
                const T end;   //!< end point
                T       sum;   //!< local sum
                range  *next;  //!< for list/pool
                range  *prev;  //!< for list

                //! constructor
                inline explicit range(const T a,const T b) throw() :
                ini(a), end(b), sum(0), next(0), prev(0) {}

                //! destructor
                inline virtual ~range() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(range);
            };

            //! adaptive computation
            template <typename T,typename FUNC> static inline
            T compute( FUNC &F, const T a, const T b, const T ftol, size_t *user_count=0 )
            {
                core::pool_of_cpp< range<T> > todo;
                core::list_of_cpp< range<T> > done;
                size_t                        self_count = 0;
                size_t                       &count = (user_count!=NULL?*user_count:self_count);

                todo.store( new range<T>(a,b) );
                ++count;
                while(todo.size>0)
                {
                    if(count>1000) throw exception("fuck");
                    auto_ptr< range<T> > curr = todo.query();
                    if( quad(curr->sum,F,curr->ini,curr->end,ftol) )
                    {
                        //std::cerr << "[*] count=" << count << " for [" << curr->ini << ":" << curr->end << "]" << std::endl;
                        done.push_back( curr.yield() );
                    }
                    else
                    {
                        const T mid = (curr->ini+curr->end)/2;
                        {
                            const T end   = curr->end;
                            const T width = abs_of(end-mid);
                            if(width>numeric<T>::minimum)
                            {
                                todo.store( new range<T>(mid,end) ); ++count;
                            }
                        }

                        {
                            const T ini   = curr->ini;
                            const T width = abs_of(mid-ini);
                            if(width>numeric<T>::minimum)
                            {
                                todo.store( new range<T>(ini,mid) ); ++count;
                            }
                        }
                        //std::cerr << "[+] count=" << count << " for [" << curr->ini << ":" << curr->end << "]" << ", mid=" << mid << std::endl;
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

            //! binder first wrapper
            template <typename T, typename FUNCXY>
            struct fxy2fx
            {
                T       x;     //!< position
                FUNCXY *pfxy;  //!< pointer

                //! call
                inline T operator()( T y )
                {
                    assert(pfxy);
                    return (*pfxy)(x,y);
                }
            };

            //! integration between boundaries
            template <typename T, typename FUNCXY, typename BOUNDARY>
            struct fxy_call
            {
                FUNCXY   *pfxy;       //!< 2D function pointer
                BOUNDARY *plower_y;   //!< 1D lower boundary pointer
                BOUNDARY *pupper_y;   //!< 1D upper boundary pointer
                T         ftol;       //!< fractional tolerance
                size_t   *user_count; //!< user count for calls

                //! call
                inline T operator()( T x )
                {
                    assert(pfxy);
                    assert(plower_y);
                    assert(pupper_y);
                    fxy2fx<T,FUNCXY> fy  = { x, pfxy };
                    const T          ylo = (*plower_y)(x);
                    const T          yup = (*pupper_y)(x);
                    const T          tmp = compute(fy,ylo,yup,ftol,user_count);
                    //std::cerr << "\t @x=" << x << " -> [" << ylo << ":" << yup << "]" << " : " << tmp << std::endl;
                    return tmp;
                }
            };

            //! 2D integrator
            template <typename T, typename FUNCXY, typename BOUNDARY>
            static inline T compute2( FUNCXY &fxy, const T lower_x, const T upper_x, BOUNDARY &lower_y, BOUNDARY &upper_y, const T ftol )
            {
                size_t count = 0;
                fxy_call<T,FUNCXY,BOUNDARY> fx = { &fxy, &lower_y, &upper_y, ftol, &count };
                return compute(fx,lower_x,upper_x,ftol,&count);
            }

        };

    }
}

#endif

