
//! \file

#ifndef Y_HASH_TABLE_INCLUDED
#define Y_HASH_TABLE_INCLUDED 1

#include "y/associative/hash/zpairs.hpp"
#include "y/associative/hash/buckets.hpp"


namespace upsylon
{
    
    template <typename NODE>
    class hash_table
    {
    public:
        typedef core::list_of<NODE> list_type;
        
        inline explicit hash_table(const size_t num_buckets) :
        nodes(),
        pails( num_buckets ),
        cache()
        {
        }
        
        inline virtual ~hash_table() throw()
        {
        }
        
        
        
        list_type         nodes;   //!< live nodes
        hash_buckets      pails;   //!< buckets of handles to nodes
        hash_zpairs<NODE> cache;   //!< cache
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_table);
    };

}

#endif
