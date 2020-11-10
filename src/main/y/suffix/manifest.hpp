//! \file

#ifndef Y_SUFFIX_MANIFEST_INCLUDED
#define Y_SUFFIX_MANIFEST_INCLUDED 1

#include "y/suffix/tree.hpp"
#include "y/suffix/collection.hpp"

namespace upsylon
{
    
    //__________________________________________________________________________
    //
    //
    //! simple manifest of CODE sequences, no assiocated data
    /**
     will derived:
     - suffix_strings for string/buffer manifests.
     - suffix_inventory for accessible keys
     */
    //
    //__________________________________________________________________________
    template <typename CODE, typename BASE_CLASS = suffix_collection>
    class suffix_manifest : public BASE_CLASS
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef suffix_tree<CODE>             tree_type; //!< alias
        typedef typename tree_type::tree_node tree_node; //!< alias
        
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit suffix_manifest() : BASE_CLASS(), tree() {} //!< setup
        inline virtual ~suffix_manifest() throw() {}                //!< cleanup
        
        //! copy by tree cloning
        inline suffix_manifest(const suffix_manifest &other) : collection(), BASE_CLASS(), tree()
        {
            tree.clone(other.tree);
        }
        
        //! assign by copy/swap
        inline suffix_manifest & operator=(const suffix_manifest &other)
        {
            suffix_manifest temp(other);
            swap_with(temp);
            return *this;
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! load extra nodes
        virtual void reserve(size_t n)
        {
            tree.cache_load(n);
        }
        
        
        //! no-throw swap
        inline void swap_with(suffix_manifest &other) throw()
        {
            tree.swap_with(other.tree);
        }
        
        //! equality by layouts
        static inline bool are_equal(const suffix_manifest &lhs, const suffix_manifest &rhs) throw()
        {
            return tree_type::have_same_layout(lhs.tree,rhs.tree);
        }
        
        //! operator ==
        inline friend bool operator==(const suffix_manifest &lhs, const suffix_manifest &rhs) throw()
        {
            return are_equal(lhs,rhs);
        }
        
        //! operator !=
        inline friend bool operator!=(const suffix_manifest &lhs, const suffix_manifest &rhs) throw()
        {
            return !are_equal(lhs,rhs);
        }
        
        
        //! number of entries
        inline virtual size_t size() const throw() { return tree.size(); }
        
        //! erase, keep memory
        inline virtual void   free()       throw() { tree.erase(); }
        
        //! erase, ditch memory
        inline virtual void   release()    throw() { tree.ditch(); }
        
        
        //______________________________________________________________________
        //
        // insertion
        //______________________________________________________________________
        
        //! insertion by range
        template <typename ITERATOR> inline
        bool insert_by(ITERATOR     curr,
                       const size_t size)
        {
            static void *used = suffix::in_use();
            return tree.insert_by(curr,size,used);
        }
        
        //! insertion by sequence
        template <typename SEQUENCE> inline
        bool insert_by(SEQUENCE &seq)
        {
            static void *used = suffix::in_use();
            return tree.insert_by(seq,used);
        }
        
        //! insertion by path
        template <typename U> inline
        bool insert_at(const accessible<U> &path)
        {
            static void *used = suffix::in_use();
            return tree.insert_at(path,used);
        }
        
        
        //______________________________________________________________________
        //
        // search
        //______________________________________________________________________
        
        //! search by range
        template <typename ITERATOR> inline
        bool search_by(ITERATOR     curr,
                       const size_t size) const throw()
        {
            return is_used( tree.find_by(curr,size) );
        }
        
        //! search by sequence
        template <typename SEQUENCE> inline
        bool search_by(SEQUENCE &seq) const throw()
        {
            return is_used( tree.find_by(seq) );
        }
        
        
        //! search by path
        template <typename U> inline
        bool search_at(const accessible<U> &path) const throw()
        {
            return is_used( tree.find_at(path) );
        }
        
        
        //______________________________________________________________________
        //
        // remove
        //______________________________________________________________________
        //! remove by range
        template <typename ITERATOR> inline
        bool remove_by(ITERATOR     curr,
                       const size_t size) throw()
        {
            return NULL != tree.pull_by(curr,size);
        }
        
        //! remove by sequence
        template <typename SEQUENCE> inline
        bool remove_by(SEQUENCE &seq) throw()
        {
            return NULL != tree.pull_by(seq);
        }
        
        //! remove by path
        template <typename U> inline
        bool remove_at(const accessible<U> &path) throw()
        {
            return NULL != tree.pull_at(path);
        }
        
        
    private:
        tree_type tree;
        static inline bool is_used(const tree_node *node) throw()
        {
            return node && node->addr;
        }
    };
}

#endif

