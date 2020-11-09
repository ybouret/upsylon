//! \file
#ifndef Y_SUFFIX_BATCH_INCLUDED
#define Y_SUFFIX_BATCH_INCLUDED 1

#include "y/suffix/graph.hpp"
#include "y/associative/catalog.hpp"
#include "y/suffix/key-to-path.hpp"

namespace upsylon
{


    //__________________________________________________________________________
    //
    //
    //! catalog<KEY,TYPE> interface
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class suffix_batch : public suffix_graph<char,T, catalog<KEY,T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);
        Y_DECL_ARGS(KEY,key_type);
        typedef key_to_path<KEY>                key_get;
        typedef catalog<KEY,T>                  base_class;
        typedef suffix_graph<char,T,base_class> graph_type;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit suffix_batch() : graph_type() {}

        //! setup with capacity
        inline explicit suffix_batch(const size_t n, const as_capacity_t &_) : graph_type(n,_) {}

        //! cleanup
        inline virtual ~suffix_batch() throw() {}

        //! copy
        inline suffix_batch(const suffix_batch &other) : collection(), graph_type(other)
        {
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

