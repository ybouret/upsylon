//! \file
#ifndef Y_SUFFIX_SET_INCLUDED
#define Y_SUFFIX_SET_INCLUDED 1

#include "y/associative/suffix/graph.hpp"
#include "y/associative/catalog.hpp"
#include "y/associative/suffix/key-to-path.hpp"

namespace upsylon
{


    //__________________________________________________________________________
    //
    //
    //! catalog<KEY,TYPE> interface
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class suffix_set : public suffix_graph<char,T, catalog<KEY,T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                                 //!< aliases
        Y_DECL_ARGS(KEY,key_type);                           //!< aliases
        typedef key_to_path<KEY>                key_get;     //!< alias
        typedef catalog<KEY,T>                  base_class;  //!< alias
        typedef suffix_graph<char,T,base_class> graph_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit suffix_set() : graph_type() {}

        //! setup with capacity
        inline explicit suffix_set(const size_t n, const as_capacity_t &_) : graph_type(n,_) {}

        //! cleanup
        inline virtual ~suffix_set() throw() {}

        //! copy
        inline suffix_set(const suffix_set &other) : collection(), graph_type(other)
        {
        }

        //! assign
        inline suffix_set & operator=( const suffix_set &other )
        {
            graph_type &self = *this;
            self = other;
            return *this;
        }


        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________

        //! catalog insert
        inline virtual bool insert(param_type args)
        {
            const_key_type & key = args.key();
            return this->insert_by( key_get::addr_of(key), key_get::size_of(key),args);
        }

        //! catalog search
        inline virtual const_type *search(param_key_type key) const throw()
        {
            return this->search_by( key_get::addr_of(key), key_get::size_of(key) );
        }

        //! catalog search
        inline virtual type *search(param_key_type key)  throw()
        {
            return (type *)(this->search_by( key_get::addr_of(key), key_get::size_of(key) ));
        }

        //! catalog remove
        inline virtual bool remove(param_key_type key) throw()
        {
            return this->remove_by( key_get::addr_of(key), key_get::size_of(key) );
        }



    };


}

#endif

