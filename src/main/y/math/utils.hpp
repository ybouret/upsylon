//! \file
#ifndef Y_MATH_UTILS_INCLUDED
#define Y_MATH_UTILS_INCLUDED 1

#include "y/math/types.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/index.hpp"

namespace upsylon
{
    
    namespace math
    {
        //! find with return type T
        template <typename T>
        struct __find
        {
            Y_DECL_ARGS(T,type); //!< aliases
            
            //! max_of(i,i<last,FUNC(*i)),
            template <typename ITERATOR, typename FUNC> static inline
            type max_within( ITERATOR i, const ITERATOR last, FUNC &f)
            {
                type v = type(f(*i));
                while(++i!=last)
                {
                    const_type t = type(f(*i));
                    if(t>v)
                    {
                        v = t;
                    }
                }
                return v;
            }
            
            //! max(f(seq))
            template <typename SEQUENCE,typename FUNC> static inline
            type max_within( const SEQUENCE &seq, FUNC &f )
            {
                return max_within( seq.begin(), seq.end(), f);
            }
            
            //! max(seq)
            template <typename SEQUENCE> static inline
            type max_of( const SEQUENCE &seq )
            {
                return max_within( seq, __id<type> );
            }
            
            //! max(|seq|)
            template <typename SEQUENCE> static inline
            type abs_max_of( const SEQUENCE &seq )
            {
                return max_within( seq, __abs<typename SEQUENCE::type> );
            }
            
            //! min_of(i,i<last,FUNC(*i)),
            template <typename ITERATOR, typename FUNC> static inline
            type min_within( ITERATOR i, const ITERATOR last, FUNC &f)
            {
                type v = type(f(*i));
                while(++i!=last)
                {
                    const_type t = type(f(*i));
                    if(t<v)
                    {
                        v = t;
                    }
                }
                return v;
            }
            
            //! min(f(seq))
            template <typename SEQUENCE,typename FUNC> static inline
            type min_within( const SEQUENCE &seq, FUNC &f )
            {
                return min_within( seq.begin(), seq.end(), f);
            }
            
            //! min(seq)
            template <typename SEQUENCE> static inline
            type min_of( const SEQUENCE &seq )
            {
                return min_within( seq, __id<type> );
            }
            
            //! min(|seq|)
            template <typename SEQUENCE> static inline
            type abs_min_of( const SEQUENCE &seq )
            {
                return min_within( seq, __abs<typename SEQUENCE::type> );
            }
            
            //! truncate and return number of 'zero' values
            /**
             - compute tol = epsilon * n * max(|seq|).
             - set seq[i] to to 0 if |seq[i]|<=tol
             \return the number of almost zero, a.k.a the numeric kernel size.
             */
            template <typename SEQUENCE> static inline
            size_t truncate( SEQUENCE &seq )
            {
                size_t       n    = 0;
                mutable_type vmax = 0;
                for( typename SEQUENCE::iterator i=seq.begin(); i != seq.end(); ++i, ++n )
                {
                    const_type tmp = __fabs(*i);
                    if(tmp>vmax) vmax=tmp;
                }
                const_type tol = numeric<type>::epsilon * type(n) * vmax;
                size_t     ker = 0;
                for( typename SEQUENCE::iterator i=seq.begin(); i != seq.end(); ++i )
                {
                    type &v = *i;
                    if( __fabs(v) <= tol )
                    {
                        (mutable_type&)v = 0;
                        ++ker;
                    }
                }
                return ker;
            }
            
            //! truncate and return number of 'zero' values
            /**
             - compute tol = epsilon * n * max(|seq|).
             - set seq[i] to to 0 if |seq[i]|<=tol, 1/seq[i] otherwise
             \return the number of almost zero, a.k.a the numeric kernel size.
             */
            template <typename SEQUENCE> static inline
            size_t inverse( SEQUENCE &seq )
            {
                size_t       n    = 0;
                mutable_type vmax = 0;
                for( typename SEQUENCE::iterator i=seq.begin(); i != seq.end(); ++i, ++n )
                {
                    const_type tmp = __fabs(*i);
                    if(tmp>vmax) vmax=tmp;
                }
                const_type tol = numeric<type>::epsilon * type(n) * vmax;
                size_t     ker = 0;
                for( typename SEQUENCE::iterator i=seq.begin(); i != seq.end(); ++i )
                {
                    type &v = *i;
                    if( __fabs(v) <= tol )
                    {
                        (mutable_type&)v = 0;
                        ++ker;
                    }
                    else
                    {
                        (mutable_type&)v = T(1)/v;
                    }
                }
                return ker;
            }
            
            
            
            //! truncate matrix
            /**
             - compute tol = epsilon * sqrt(rows*cols) * max(|A|)
             */
            static inline
            void truncate( matrix<T> &A )
            {
                T        Amax =  0;
                {
                    const T *a    = *A;
                    for(size_t i=A.items;i>0;--i)
                    {
                        const T Atmp = __fabs(*(a++));
                        if(Atmp>Amax)
                        {
                            Amax=Atmp;
                        }
                    }
                }
                const T tol = sqrt_of( T(A.items) ) * numeric<T>::epsilon * Amax;
                {
                    T *a = *A;
                    for(size_t i=A.items;i>0;--i)
                    {
                        T &v = *(a++);
                        if( __fabs(v) <= tol )
                        {
                            v=0;
                        }
                    }
                }
            }
            
            //! keep sz largest absolute values
            template <typename SEQUENCE> static inline
            void __set_size( SEQUENCE &seq, const size_t sz)
            {
                assert(seq.size()>=sz);
                const size_t n = seq.size();
                vector<mutable_type> v(n,as_capacity);
                vector<size_t>       I(n,as_capacity);
                mutable_type         vmax = 0;
                {
                    size_t j=1;
                    for( typename SEQUENCE::iterator i=seq.begin(); i != seq.end(); ++i,++j)
                    {
                        const_type temp = *i;
                        const_type vtmp = __fabs(temp);
                        if(vtmp>vmax) vmax=vtmp;
                        v.push_back(vtmp);
                        I.push_back(j);
                    }
                }
                indexing::make(I,comparison::increasing<type>,v);
                //std::cerr << "I=" << I << std::endl;
                for(size_t i=n-sz;i>0;--i)
                {
                    const size_t j = I[i];
                    //std::cerr << "set 0@" << j << std::endl;
                    typename SEQUENCE::iterator k = seq.begin() + (j-1);
                    ((mutable_type &) *k) = 0;
                }
            }
            
            //! keep img_size largest absolute values
            template <typename SEQUENCE> static inline
            void set_image_size( SEQUENCE &seq, const size_t img_size )
            {
                assert(seq.size()>=img_size);
                __set_size(seq,img_size);
            }
            
            //! keep img_size largest absolute values
            template <typename SEQUENCE> static inline
            void set_kernel_size( SEQUENCE &seq, const size_t ker_size )
            {
                assert(seq.size()>=ker_size);
                __set_size(seq,seq.size()-ker_size);
            }
        };
        
    }
    
}


#endif

