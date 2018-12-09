//! \file

#ifndef Y_MATH_STAT_METRICS_INCLUDED
#define Y_MATH_STAT_METRICS_INCLUDED 1

#include "y/math/types.hpp"
#include "y/type/point2d.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    namespace math
    {

        template <typename T>
        class correlation : public list< point2d<T> >
        {
        public:
            typedef point2d<T>    point_t;
            typedef list<point_t> list_type;

            inline explicit correlation() : list_type() {} //!< initialize
            inline virtual ~correlation() throw()       {} //!< destructor

            template <typename U,typename V>
            inline void add( U u, V v)
            {
                const point_t p(u,v);
                this->push_back(p);
            }

            inline T compute() const
            {
                const size_t n = this->size();
                if(n<=1)
                {
                    return 1;
                }
                else
                {
                    // first pass: compute averages
                    vector<T> x(n);
                    vector<T> y(n);
                    point_t   bar;
                    {
                        size_t j=1;
                        for(typename list_type::const_iterator i=this->begin(); j<=n; ++i,++j)
                        {
                            x[j] = i->x;
                            y[j] = i->y;
                        }
                        hsort(x, comparison::decreasing_abs<T> );
                        hsort(y, comparison::decreasing_abs<T> );

                        for(size_t i=n;i>0;--i)
                        {
                            bar.x += x[i];
                            bar.y += y[i];
                        }
                        bar /= n;
                    }
                    // second pass, compute individual terms
                    vector<T> xy(n);
                    {
                        size_t j=1;
                        for(typename list_type::const_iterator i=this->begin(); j<=n; ++i,++j)
                        {
                            const T dx = i->x-bar.x;
                            const T dy = i->y-bar.y;
                            x[j]  = square_of(dx);
                            y[j]  = square_of(dy);
                            xy[j] = dx*dy;
                        }
                    }
                    hsort(x, comparison::decreasing<T>);
                    hsort(x, comparison::decreasing<T>);
                    hsort(xy,comparison::decreasing_abs<T>);
                    T sum_x2 = 0, sum_y2=0, sum_xy = 0;
                    for(size_t i=n;i>0;--i)
                    {
                        sum_x2 += x[i];
                        sum_y2 += y[i];
                        sum_xy += xy[i];
                    }
                    return sum_xy/( sqrt_of(sum_x2)*sqrt_of(sum_y2)+numeric<T>::tiny);
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(correlation);
        };

    }
}

#endif

