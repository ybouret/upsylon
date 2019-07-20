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

        //! functions for integration
        struct integrate
        {

            static const size_t min_level = 2;  // 2
            static const size_t max_level = 11; // 2^(max_level-1)-1 calls
            static const size_t max_iters = 1 << (max_level-2);
            static const size_t warmup    = ((max_level+min_level)>>1)-1;


            template <typename T>
            static inline int compare_by_increasing_abs( const void *lhs, const void *rhs)
            {
                assert(lhs); assert(rhs);
                const T L = abs_of( *static_cast<const T *>(lhs) );
                const T R = abs_of( *static_cast<const T *>(rhs) );
                return ( (L<R) ? -1 : ( (R<L) ? 1 : 0 ) );
            }


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

                // accumulate values
                {
                    const T             delta = w/iter;
                    T                   x     = a + half*delta;
                    for(size_t j=0;j<iter;++j,x+=delta)
                    {
                        value[j] = F(x);
                    }
                }
                // order to reduce roundoff errors
                qsort(value,iter,sizeof(T),compare_by_increasing_abs<T>);
                T sum = 0;
                for(size_t j=0;j<iter;++j)
                {
                    sum += value[j];
                }
                return half*(s+(w*sum)/iter);
            }

            template <typename T, typename FUNC> static inline
            bool quad(T &s, FUNC &F, const T a, const T b, const T ftol )
            {
                static const T four  = T(4);
                static const T three = T(3);

                // initialize at level 1
                const T w         = b-a;
                T       sum_trapz = (s=T(0.5) * w * (F(b)+F(a)));
                T       sum_accel = sum_trapz;
                T       old_trapz = 0;
                T       old_accel = 0;

                // iterate
                //std::cerr << "quad([" << a << ":" << b << "])" << std::endl;
                for(size_t level=2;level<=max_level;++level)
                {
                    sum_trapz = trapezes(sum_trapz, a, w, F, level);  // trapezes at this level
                    sum_accel = (s=(four*sum_trapz-old_trapz)/three); // Simpson's at this level
                    if(level>warmup)
                    {

                        // test convergences
                        {
                            const T delta_trapz = fabs_of( sum_trapz - old_trapz );
                            //std::cerr << "\ttrapz: " << old_trapz << "-> " << sum_trapz << ", delta=" << delta_trapz << std::endl;
                            if( delta_trapz <= fabs( ftol * old_trapz ) )
                            {
                                return true;
                            }
                        }

                        if(true)
                        {
                            const T delta_accel = fabs_of( sum_accel - old_accel );
                            //std::cerr << "\taccel: " << old_accel << "-> " << sum_accel << ", delta=" << delta_accel << std::endl;
                            if( delta_accel <= fabs( ftol * old_accel ) )
                            {
                                return true;
                            }
                        }
                    }
                    old_trapz = sum_trapz;
                    old_accel = sum_accel;
                }

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

                // initialize with full range
                todo.store( new range<T>(a,b) );
                ++count;
                while(todo.size>0)
                {
                    if(count>1000) throw exception("intg failure");
                    auto_ptr< range<T> > curr = todo.query();
                    if( quad(curr->sum,F,curr->ini,curr->end,ftol) )
                    {
                        // success
                        done.push_back( curr.yield() );
                    }
                    else
                    {
                        // failure => split
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
                //std::cerr << "sum of " << done.size << " ranges" << std::endl;
                T sum = 0;
                while(done.size>0)
                {
                    T tmp = 0; assert(quad(tmp, F, done.head->ini, done.head->end, ftol));
                    //std::cerr << "[" << done.head->ini << ":" << done.head->end << "]\t:\t" << done.head->sum << " /\t" << tmp << std::endl;
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

