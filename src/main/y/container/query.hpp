//! \file

#ifndef Y_CONTAINER_QUERY_INCLUDED
#define Y_CONTAINER_QUERY_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/traits.hpp"

namespace upsylon {

    //! simple queries on containers
    struct container_query
    {
        //----------------------------------------------------------------------
        //
        // unary first
        //
        //----------------------------------------------------------------------

        //! first among iterators
        template <typename T, typename ITERATOR, typename PROC> static inline
        T *first_( ITERATOR it, size_t n, PROC &proc )
        {
            while(n-- > 0)
            {
                T                                           & data = *it;
                const typename type_traits<T>::mutable_type & temp = data;
                if( proc(temp) ) return & data;
                ++it;
            }
            return NULL;
        }

        //! first in query
        template <typename CONTAINER, typename PROC> static inline
        typename CONTAINER::type * first( CONTAINER &c, PROC &proc )
        {
            return first_< typename CONTAINER::type, typename CONTAINER::iterator, PROC>( c.begin(), c.size(), proc );
        }

        //! const first in query
        template <typename CONTAINER, typename PROC> static inline
        typename CONTAINER::const_type * first( const CONTAINER &c, PROC &proc )
        {
            return first_< typename CONTAINER::const_type, typename CONTAINER::const_iterator, PROC>( c.begin(), c.size(), proc );
        }

        //! first reverse
        template <typename CONTAINER, typename PROC> static inline
        typename CONTAINER::type * first_reverse( CONTAINER &c, PROC &proc )
        {
            return first_< typename CONTAINER::type, typename CONTAINER::reverse_iterator, PROC>( c.rbegin(), c.size(), proc );
        }

        //! const first reverse
        template <typename CONTAINER, typename PROC> static inline
        typename CONTAINER::const_type * first_reverse( const CONTAINER &c, PROC &proc )
        {
            return first_< typename CONTAINER::const_type, typename CONTAINER::const_reverse_iterator, PROC>( c.rbegin(), c.size(), proc );
        }

        //----------------------------------------------------------------------
        //
        // unary all
        //
        //----------------------------------------------------------------------
        //! all matching among iterators
        template <typename SEQUENCE, typename T, typename ITERATOR, typename PROC> static inline
        size_t all_( SEQUENCE &seq, ITERATOR it, size_t n, PROC &proc )
        {
            size_t count = 0;
            while(n-- > 0)
            {
                T                                           & data = *it;
                const typename type_traits<T>::mutable_type & temp = data;
                if( proc(temp) )
                {
                    seq.push_back(temp);
                    ++count;
                }
                ++it;
            }
            return count;
        }

        //! all matching in sequence
        template <typename SEQUENCE, typename CONTAINER, typename PROC> static inline
        size_t all( SEQUENCE &seq, const CONTAINER &c, PROC &proc )
        {
            return all_<SEQUENCE,typename CONTAINER::const_type,typename CONTAINER::const_iterator,PROC>( seq, c.begin(), c.size(), proc );
        }




        //----------------------------------------------------------------------
        //
        // binary
        //
        //----------------------------------------------------------------------
        //! first among iterators
        template <typename T, typename ITERATOR, typename PROC, typename ARGS> static inline
        T *first_( ITERATOR it, size_t n, PROC &proc, ARGS &args )
        {
            while(n-- > 0)
            {
                T                                           & data = *it;
                const typename type_traits<T>::mutable_type & temp = data;
                if( proc(temp,args) ) return & data;
                ++it;
            }
            return NULL;
        }

        //! first in container
        template <typename CONTAINER, typename PROC, typename ARGS> static inline
        typename CONTAINER::type * first( CONTAINER &c, PROC &proc, ARGS &args )
        {
            return first_< typename CONTAINER::type, typename CONTAINER::iterator, PROC, ARGS>(c.begin(),c.size(),proc,args);
        }

        //! const first in container
        template <typename CONTAINER, typename PROC, typename ARGS> static inline
        typename CONTAINER::const_type * first( const CONTAINER &c, PROC &proc, ARGS &args )
        {
            return first_< typename CONTAINER::const_type, typename CONTAINER::const_iterator, PROC, ARGS>(c.begin(),c.size(),proc,args);
        }

        //! first reverse in container
        template <typename CONTAINER, typename PROC, typename ARGS> static inline
        typename CONTAINER::type * first_reverse( CONTAINER &c, PROC &proc, ARGS &args )
        {
            return first_<typename CONTAINER::type, typename CONTAINER::reverse_iterator,PROC,ARGS>(c.rbegin(),c.size(),proc,args);
        }

        //! const first reverse in container
        template <typename CONTAINER, typename PROC, typename ARGS> static inline
        typename CONTAINER::const_type * first_reverse( const CONTAINER &c, PROC &proc, ARGS &args )
        {
            return first_< typename CONTAINER::const_type, typename CONTAINER::const_reverse_iterator, PROC, ARGS>(c.rbegin(),c.size(),proc,args);
        }

        //----------------------------------------------------------------------
        //
        // binary all
        //
        //----------------------------------------------------------------------

        //! all matching among iterators
        template <typename SEQUENCE, typename T, typename ITERATOR, typename PROC, typename ARGS> static inline
        size_t all_( SEQUENCE &seq, ITERATOR it, size_t n, PROC &proc, ARGS &args)
        {
            size_t count = 0;
            while(n-- > 0)
            {
                T                                           & data = *it;
                const typename type_traits<T>::mutable_type & temp = data;
                if( proc(temp) )
                {
                    seq.push_back(temp,args);
                    ++count;
                }
                ++it;
            }
            return count;
        }

        //! all matching among container
        template <typename SEQUENCE, typename CONTAINER, typename PROC, typename ARGS> static inline
        size_t all( SEQUENCE &seq, const CONTAINER &c, PROC &proc, ARGS &args )
        {
            return all_<SEQUENCE,typename CONTAINER::const_type,typename CONTAINER::const_iterator,PROC,ARGS>( seq, c.begin(), c.size(), proc, args);
        }



    };

}

#endif
