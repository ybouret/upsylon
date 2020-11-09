//! \file

#ifndef Y_SUFFIX_STORAGE_INCLUDED
#define Y_SUFFIX_STORAGE_INCLUDED 1

#include "y/suffix/graph.hpp"
#include <cstring>

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
        collection(), graph_type(other)
        {
            
        }

        //! assign
        inline suffix_storage & operator=( const suffix_storage &other )
        {
            { graph_type &self = *this; self = other; }
            return *this;
        }

        //! insert by buffer
        inline bool insert(const memory::ro_buffer &key, param_type args)
        {
            return this->insert_by(key.as<char>(),key.length(),args);
        }

        //! insert by text
        inline bool insert(const char *key, param_type args)
        {
            return this->insert_by(key,key?strlen(key):0,args);
        }

        //! search by string
        inline const_type *search(const memory::ro_buffer &key) const throw()
        {
            return this->search_by(key.as<char>(),key.length());
        }

        //! search by text
        inline const_type *search(const char *key) const throw()
        {
            return this->search_by(key,key?strlen(key):0);
        }

        //! remove by string
        inline bool remove(const memory::ro_buffer &key) throw()
        {
            return this->remove_by(key.as<char>(),key.length());
        }

        //! remove by text
        inline bool remove(const char *key) throw()
        {
            return this->remove_by(key,key?strlen(key):0);
        }

    };

}

#endif
