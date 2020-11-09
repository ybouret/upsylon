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
        Y_DECL_ARGS(T,type);

        typedef suffix_graph<char,T,BASE_CLASS> graph_type;

        inline virtual ~suffix_storage() throw() {}

        inline explicit suffix_storage() : graph_type()
        {
        }

        inline suffix_storage(const suffix_storage &other) :
        graph_type(other)
        {
            
        }

        inline suffix_storage & operator=( const suffix_storage &other )
        {
            { graph_type &self = *this; self = other; }
            return *this;
        }

        inline bool insert(const string &key, param_type args)
        {
            return this->insert_by(*key,key.size(),args);
        }

        inline bool insert(const char *key, param_type args)
        {
            return this->insert_by(key,length_of(key),args);
        }
        

    };

}

#endif
