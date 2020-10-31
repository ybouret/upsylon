//! file
#ifndef Y_PREFIX_DEPOT_INCLUDED
#define Y_PREFIX_DEPOT_INCLUDED 1

#include "y/associative/prefix/stem.hpp"

namespace upsylon {
 
    template <typename CODE>
    class prefix_depot
    {
    public:
        typedef prefix_stem<CODE,null_type>   stem_type;
        typedef typename stem_type::node_type node_type;
        
        inline explicit prefix_depot() : db()
        {
        }
        
        inline virtual ~prefix_depot() throw()
        {
        }
        
        template <typename ITERATOR> inline
        bool insert_path(ITERATOR     curr,
                         const size_t size)
        {
            node_type *mark = 0;
            node_type *node = db.grow(curr,size,&mark);
            return NULL!=node;
        }
        
        inline bool insert(const CODE *code,
                           const size_t size)
        {
            assert(!(NULL==code&&size>0));
            return insert_path(code,size);
        }
        
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_depot);
        stem_type db;
    };
    
}

#endif
