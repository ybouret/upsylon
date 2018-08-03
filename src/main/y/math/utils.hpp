#ifndef Y_MATH_UTILS_INCLUDED
#define Y_MATH_UTILS_INCLUDED 1

#include "y/math/types.hpp"

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
                type v = f(*i);
                while(++i!=last)
                {
                    const_type t = f(*i);
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
                type v = f(*i);
                while(++i!=last)
                {
                    const_type t = f(*i);
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
            template <typename SEQUENCE>
            size_t truncate( SEQUENCE &seq )
            {
                size_t n    = 0;
                type   vmax = 0;
                for( typename SEQUENCE::iterator i=seq.begin(); i != seq.end(); ++i, ++n )
                {
                    const_type tmp = __fabs(*i);
                    if(tmp>vmax) vmax=tmp;
                }
                const_type tol = numeric<type>::epsilon * n * vmax;
                size_t     ker = 0;
                for( typename SEQUENCE::iterator i=seq.begin(); i != seq.end(); ++i )
                {
                    type &v = *i;
                    if( __fabs(v) <= tol )
                    {
                        v = 0;
                        ++ker;
                    }
                }
                return ker;
            }
        };
        
    }
    
}


#endif

