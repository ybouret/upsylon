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

        //! low level average computation
        template <typename T, typename ITERATOR>
        T __average_of( ITERATOR it, const size_t n, T *pSdev)
        {
            if(pSdev)
            {
                *pSdev=0;
            }

            if(n>0)
            {
                vector<T> tmp(n);
                for(size_t i=1;i<=n;++i,++it)
                {
                    tmp[i] = *it;
                }
                hsort(tmp,comparison::decreasing_abs<T>);
                T ave = 0;
                for(size_t i=n;i>0;--i)
                {
                    ave += tmp[i];
                }
                ave /= n;
                if(pSdev)
                {
                    if(n>1)
                    {
                        for(size_t i=n;i>0;--i)
                        {
                            tmp[i] = square_of(tmp[i]-ave);
                        }
                        hsort(tmp,comparison::decreasing<T>);
                        T sig = 0;
                        for(size_t i=n;i>0;--i)
                        {
                            sig += tmp[i];
                        }
                        sig    = sqrt_of(sig)/(n-1);
                        *pSdev = sig;
                    }
                }
                return ave;
            }
            else
            {
                return 0;
            }
        }

        //! average for sequence with iterator
        template <typename SEQ> inline
        typename SEQ::type average_of( const SEQ &seq, typename SEQ::type *pSig=0)
        {
            return __average_of(seq.begin(), seq.size(), pSig);
        }

        //! average for array
        template <typename T> inline
        T average_of( const array<T> &arr, T *pSig=0)
        {
            return __average_of( arr(), arr.size(), pSig);
        }


        //! low level median computation
        template <typename T, typename ITERATOR>
        T __median_of( ITERATOR it, const size_t n, T *pAdev)
        {
            if(pAdev)
            {
                *pAdev=0;
            }
            if(n>0)
            {
                vector<T> tmp(n);
                for(size_t i=1;i<=n;++i,++it)
                {
                    tmp[i] = *it;
                }
                hsort(tmp,comparison::increasing<T>);
                const size_t nh = (n>>1);
                const T      m  = ( (n&1) != 0 ) ? tmp[nh+1] : (tmp[nh]+tmp[nh+1])/T(2);

                if(pAdev)
                {
                    for(size_t i=n;i>0;--i)
                    {
                        tmp[i] = abs_of(tmp[i]-m);
                    }
                    hsort(tmp,comparison::decreasing<T>);
                    T adev = 0;
                    for(size_t i=n;i>0;--i)
                    {
                        adev += tmp[i];
                    }
                    *pAdev = adev/n;
                }
                return m;
            }
            else
            {
                return 0;
            }
        }

        //! median computation for sequence with iterator
        template <typename SEQ> inline
        typename SEQ::type median_of( const SEQ &seq, typename SEQ::type *pAdev=0)
        {
            return __median_of(seq.begin(), seq.size(), pAdev);
        }

        //! median for array
        template <typename T> inline
        T median_of( const array<T> &arr, T *pAdev=0)
        {
            return __median_of( arr(), arr.size(), pAdev);
        }

        //! compute Pearson's correlation coefficient
        template <typename T>
        class correlation : public list< point2d<T> >
        {
        public:
            typedef point2d<T>    point_t;   //!< alias for (x,y)
            typedef list<point_t> list_type; //!< alias for base class

            inline explicit correlation() : list_type() {} //!< initialize
            inline virtual ~correlation() throw()       {} //!< destructor

            //! add a new point to internal format
            template <typename U,typename V>
            inline void add( U u, V v)
            {
                const point_t p(u,v);
                this->push_back(p);
            }

            //! compute the coefficient
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

