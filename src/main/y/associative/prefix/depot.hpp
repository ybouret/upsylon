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
    class prefix_depot : public prefix_stem<CODE,null_type>
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
        inline explicit prefix_depot() : stem_type()
        {
        }
        
        //! cleanup
        inline virtual ~prefix_depot() throw()
        {
        }
    
        //! copy
        inline prefix_depot(const prefix_depot &other) : stem_type()
        {
            this->duplicate(other);
        }
        
        //! assign
        inline prefix_depot & operator=(const prefix_depot &other)
        {
            prefix_depot temp(other);
            swap_with(temp);
            return *this;
        }
        
        //! insert 'used' using path
        template <typename ITERATOR> inline
        bool insert(ITERATOR &curr, const size_t size)
        {
            return this->tick(curr,size);
        }
        
        //! insert 'used' using sequence
        template <typename SEQUENCE> inline
        bool insert(SEQUENCE &seq)
        {
            return this->tick(seq);
        }
        
        //! insert 'used' using C-style array
        inline bool insert(const CODE *code)
        {
            return this->tick(code);
        }
        
        
        //! remove value at path
        template <typename ITERATOR> inline
        bool remove(ITERATOR curr, const size_t size) throw()
        {
            const node_type *node = this->find(curr,size);
            if(node&&node->used)
            {
                this->pull( (node_type*) node);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        //! remove value using sequnece
        template <typename SEQUENCE> inline
        bool remove(SEQUENCE &seq) throw()
        {
            return remove(seq.begin(),seq.size());
        }
        
        //! remove value using C-style array
        inline bool remove(const CODE *code)
        {
            return remove(code,stem_type::codelen(code));
        }
        
        //! content
        inline size_t size() const throw()
        {
            return this->tell();
        }
        
        //! clear structure, keep memory
        inline void free() throw()
        {
            this->reset();
        }
        
        //! clear structure, ditch memory
        inline void release() throw()
        {
            this->ditch();
        }
        
        //! test equality
        inline friend bool operator==(const prefix_depot &lhs,
                                      const prefix_depot &rhs) throw()
        {
            return lhs.similar_to(rhs);
        }
        
        //! no-throw exchange
        inline void swap_with(prefix_depot &other) throw()
        {
            xch(other);
        }
        
        
        
        
       
    };
    
}

#endif
