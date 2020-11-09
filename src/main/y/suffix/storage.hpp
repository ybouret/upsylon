//! \file

#ifndef Y_SUFFIX_STORAGE_INCLUDED
#define Y_SUFFIX_STORAGE_INCLUDED 1

#include "y/suffix/graph.hpp"
#include "y/string.hpp"

namespace upsylon
{


    //__________________________________________________________________________
    //
    //
    //! a storage is a graph indexed by strings with utility functions
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename BASE_CLASS = suffix_collection
    >
    class suffix_storage : public suffix_graph<char,T,BASE_CLASS>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                                 //!< aliases
        typedef suffix_graph<char,T,BASE_CLASS> graph_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~suffix_storage() throw() {}

        //! setup
        inline explicit suffix_storage() : graph_type()
        {
        }

        //! copy
        inline suffix_storage(const suffix_storage &other) :
        graph_type(other)
        {
            
        }

        //! assign
        inline suffix_storage & operator=( const suffix_storage &other )
        {
            { graph_type &self = *this; self = other; }
            return *this;
        }

        //! insert by string
        inline bool insert(const string &key, param_type args)
        {
            return this->insert_by(*key,key.size(),args);
        }

        //! insert by text
        inline bool insert(const char *key, param_type args)
        {
            return this->insert_by(key,length_of(key),args);
        }

        //! search by string
        inline const_type *search(const string &key) const throw()
        {
            return this->search_by(*key,key.size());
        }

        //! search by text
        inline const_type *search(const char *key) const throw()
        {
            return this->search_by(key,length_of(key));
        }

        //! remove by string
        inline bool remove(const string &key) throw()
        {
            return this->remove_by(*key,key.size());
        }

        //! remove by text
        inline bool remove(const char *key) throw()
        {
            return this->remove_by(key,length_of(key));
        }

    };

}

#endif
