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
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef prefix_stem<CODE,null_type>   stem_type; //!< alias
        typedef typename stem_type::node_type node_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! setup
        inline explicit prefix_depot() : db()
        {
        }
        
        //! cleanup
        inline virtual ~prefix_depot() throw()
        {
        }
    
        //! copy
        inline prefix_depot(const prefix_depot &other) :
        db()
        {
            db.duplicate(other.db);
        }
        
        //! assign
        inline prefix_depot & operator=(const prefix_depot &other)
        {
            prefix_depot temp(other);
            swap_with(temp);
            return *this;
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! insert
        inline bool insert(const CODE  *code,
                           const size_t size)
        {
            assert(!(NULL==code&&size>0));
            node_type *mark = 0;
            node_type *node = db.grow(code,size,&mark);
            return NULL!=node;
        }
        
        //! test
        inline bool  has(const CODE *code, const size_t size) const throw()
        {
            return db.has(code,size);
        }
        
        //! remove
        inline bool remove(const CODE *code, const size_t size) throw()
        {
            const node_type *node = db.find(code,size);
            if(node&&node->used)
            {
                db.pull((node_type*)node);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        //! test equality
        inline friend bool operator==(const prefix_depot &lhs,
                                      const prefix_depot &rhs) throw()
        {
            return lhs.db.similar_to(rhs.db);
        }
        
        //! no-throw exchange
        inline void swap_with(prefix_depot &other) throw()
        {
            db.xch(other.db);
        }
        
        //! inserted items
        inline size_t size() const throw()
        {
            return db.tell();
        }
        
        //! prune internal tree
        inline void crop() throw()
        {
            db.cache_prune();
        }
        
    private:
        stem_type db;
    };
    
}

#endif
