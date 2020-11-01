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
        inline bool insert_path(const CODE  *code,
                                const size_t size)
        {
            assert(!(NULL==code&&size>0));
            node_type *mark = 0;
            node_type *node = db.tick(code,size,&mark);
            return NULL!=node;
        }
        
        //! insert assuming string like code
        inline bool insert(const CODE *code)
        {
            return insert_path(code,codelen(code));
        }
        
        //! insert using compatible sequence
        template <typename SEQUENCE>
        inline bool insert(SEQUENCE &seq)
        {
            node_type *mark = 0;
            node_type *node = db.tick(seq,&mark);
            return NULL!=node;
        }
        
        //! test
        inline bool  has_path(const CODE *code, const size_t size) const throw()
        {
            return db.has(code,size);
        }
        
        //! test assuming string like code
        inline bool has(const CODE *code) const throw()
        {
            return has_path(code,codelen(code));
        }
        
        //! test using compatible sequence
        template <typename SEQUENCE>
        inline bool has(SEQUENCE &seq) const throw()
        {
            return db.has(seq);
        }
        
        //! remove
        inline bool remove_path(const CODE *code, const size_t size) throw()
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
        
        //! remove assuming string like code
        inline bool remove(const CODE *code) throw()
        {
            return remove(code,codelen(code));
        }
        
        //! test using compatible sequence
        template <typename SEQUENCE>
        inline bool remove(SEQUENCE &seq) throw()
        {
            const node_type *node = db.find(seq);
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
        inline bool eq(const prefix_depot &rhs) const throw()
        {
            return db.similar_to(rhs.db);
        }
        
        
        //! test equality
        inline friend bool operator==(const prefix_depot &lhs,
                                      const prefix_depot &rhs) throw()
        {
            return lhs.eq(rhs);
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
        
        //! inline 'strlen' like implementation
        static inline size_t codelen(const CODE *code) throw()
        {
            if(code)
            {
                const CODE *init = code;
                while(*code!=0)  ++code;
                return static_cast<size_t>(code-init);
            }
            else
            {
                return 0;
            }
        }
        
    private:
        stem_type db;
       
    };
    
}

#endif
