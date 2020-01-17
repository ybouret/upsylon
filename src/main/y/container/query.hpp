//! \file

#ifndef Y_CONTAINER_QUERY_INCLUDED
#define Y_CONTAINER_QUERY_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/traits.hpp"

namespace upsylon {

    struct container_query
    {
        //----------------------------------------------------------------------
        //
        // unary
        //
        //----------------------------------------------------------------------
        template <typename T, typename ITERATOR, typename PROC> static inline
        T *first_( ITERATOR it, size_t n, PROC &proc )
        {
            while(n-- > 0)
            {
                T & data = *it;
                const typename type_traits<T>::mutable_type &temp  = data;
                if( proc(temp) ) return & data;
                ++it;
            }
            return NULL;
        }

        template <typename CONTAINER, typename PROC> static inline
        typename CONTAINER::type * first( CONTAINER &c, PROC &proc )
        {
            return first_< typename CONTAINER::type, typename CONTAINER::iterator, PROC>( c.begin(), c.size(), proc );
        }

        template <typename CONTAINER, typename PROC> static inline
        typename CONTAINER::const_type * first( const CONTAINER &c, PROC &proc )
        {
            return first_< typename CONTAINER::const_type, typename CONTAINER::const_iterator, PROC>( c.begin(), c.size(), proc );
        }


        template <typename CONTAINER, typename PROC> static inline
        typename CONTAINER::type * first_reverse( CONTAINER &c, PROC &proc )
        {
            return first_< typename CONTAINER::type, typename CONTAINER::reverse_iterator, PROC>( c.rbegin(), c.size(), proc );
        }

        template <typename CONTAINER, typename PROC> static inline
        typename CONTAINER::const_type * first_reverse( const CONTAINER &c, PROC &proc )
        {
            return first_< typename CONTAINER::const_type, typename CONTAINER::const_reverse_iterator, PROC>( c.rbegin(), c.size(), proc );
        }


        //----------------------------------------------------------------------
        //
        // binary
        //
        //----------------------------------------------------------------------

        

    };

}

#endif
