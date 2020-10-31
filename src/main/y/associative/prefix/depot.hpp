//! file
#ifndef Y_PREFIX_DEPOT_INCLUDED
#define Y_PREFIX_DEPOT_INCLUDED 1

#include "y/associative/prefix/stem.hpp"

namespace upsylon {
 
    //__________________________________________________________________________
    //
    //
    //! depot to store keys, without any data
    //
    //__________________________________________________________________________

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
    
        inline prefix_depot(const prefix_depot &other) :
        db()
        {
            db.duplicate(other.db);
        }
        
        inline prefix_depot & operator=(const prefix_depot &other)
        {
            prefix_depot temp(other);
            swap_with(temp);
            return *this;
        }
        
        
        inline bool insert(const CODE  *code,
                           const size_t size)
        {
            assert(!(NULL==code&&size>0));
            node_type *mark = 0;
            node_type *node = db.grow(code,size,&mark);
            return NULL!=node;
        }
        
        inline bool  has(const CODE *code, const size_t size) const throw()
        {
            return db.has(code,size);
        }
        
        inline friend bool operator==(const prefix_depot &lhs,
                                      const prefix_depot &rhs) throw()
        {
            return stem_type::have_same_layout(lhs.db,rhs.db);
        }
        
        inline void swap_with(prefix_depot &other) throw()
        {
            db.xch(other.db);
        }
        
    private:
        stem_type db;
    };
    
}

#endif
