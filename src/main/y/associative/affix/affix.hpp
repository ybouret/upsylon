//! \file
#ifndef Y_AFFIX_INCLUDED
#define Y_AFFIX_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/object.hpp"
#include "y/ios/iosfwd.hpp"
#include "y/strfwd.hpp"
#include "y/memory/buffer.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace hashing
    {
        class function; //!< forward declaration
    }
    
    
    //__________________________________________________________________________
    //
    //
    //! basic operations on an affix tree
    //
    //__________________________________________________________________________
    class affix
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        class                                tree_node; //!< forward declaration
        typedef core::list_of_cpp<tree_node> tree_list; //!< alias
        
        //______________________________________________________________________
        //
        //! internal tree node
        //______________________________________________________________________
        class tree_node : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit tree_node(tree_node *, const uint8_t) throw(); //!< setup with parent and code
            virtual ~tree_node() throw();                           //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void   optimize()                     throw(); //!< locally optimize
            void   graphViz(ios::ostream &fp)       const; //!< for GraphViz
            void   leaves_to(tree_list &pool)     throw(); //!< return leaves to pool
            void   return_to(tree_list &pool)     throw(); //!< leaves and this to pool
            void   run(hashing::function &) const throw(); //!< in-order hashing of codes
            size_t children()               const throw(); //!< leaves.size
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            void         *addr;   //!< any data
            tree_node    *next;   //!< for list
            tree_node    *prev;   //!< for list
            tree_node    *parent; //!< parent node
            tree_list     leaves; //!< leaves nodes
            size_t        freq;   //!< frequency
            size_t        deep;   //!< from root
            const uint8_t code;   //!< code
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tree_node);
        };
        
        //______________________________________________________________________
        //
        //! insert address following a path
        /**
         on the fly nodes creation
         */
        //______________________________________________________________________
        template <typename ITERATOR>
        tree_node *insert_at_path(ITERATOR     curr,
                                  size_t       size,
                                  void        *addr)
        {
            assert(addr!=NULL);
            assert(root!=NULL);
            assert(root->code==0);
            assert(root->deep==0);
            //------------------------------------------------------------------
            // start from root
            //------------------------------------------------------------------
            tree_node *node = root;
            while(size-- > 0)
            {
                //----------------------------------------------------------
                // look for code
                //----------------------------------------------------------
                const uint8_t code = *(curr++);
                for(tree_node *chld=node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        //------------------------------------------------------
                        // take this path
                        //------------------------------------------------------
                        node = chld;
                        goto FOUND;
                    }
                }
                //--------------------------------------------------------------
                // need a new leaf
                //--------------------------------------------------------------
                node = node->leaves.push_back( new_tree_node(node,code) );
            FOUND:;
            }
            assert(node!=NULL);
            if(node->addr)
            {
                //--------------------------------------------------------------
                // busy
                //--------------------------------------------------------------
                return NULL;
            }
            else
            {
                //--------------------------------------------------------------
                // assign address and optimize
                //--------------------------------------------------------------
                node->addr = addr;
                increase_path_to(node);
                return node;
            }
        }
        
        //______________________________________________________________________
        //
        //! search a node by path
        //______________________________________________________________________
        template <typename ITERATOR>
        const tree_node *node_at_path(ITERATOR     curr,
                                      size_t       size) const throw()
        {
            const tree_node *node = root;
            while(size-- > 0)
            {
                const uint8_t code = *(curr++);
                for(tree_node *chld=node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        node = chld;
                        goto FOUND;
                    }
                }
                return NULL;
            FOUND:;
                assert(node);
            }
            
            assert(node);
            return node;
        }
        
        //______________________________________________________________________
        //
        //! remove a used node
        /**
         with pruning!
         */
        //______________________________________________________________________
        void remove_node(tree_node *) throw();
        
        
        //______________________________________________________________________
        //
        // helpers to insert
        //______________________________________________________________________
        
        
        //! insert using text[0..size-1] as path
        tree_node* insert_with(const char  *text,
                               const size_t size,
                               void *       addr);
        
        //! insert using text as path
        tree_node* insert_with(const char  *text,
                               void *       addr);
        
        //! insert using buffer as path
        tree_node* insert_with(const memory::ro_buffer &buff,
                               void *                   addr);
        
        //______________________________________________________________________
        //
        // helpers to find
        //______________________________________________________________________

        //! helper
        const tree_node *node_with(const char  *text,
                                   const size_t size) const throw();
        
        //! helper
        const tree_node *node_with(const char  *text) const throw();
        
        //! helper
        const tree_node *node_with(const memory::ro_buffer &buff) const throw();
        
        
        explicit affix();         //!< setup with root
        virtual ~affix() throw(); //!< cleanup
        void     clear() throw(); //!< return all nodes (but root) to pool
        
        void     graphViz(const upsylon::string &fileName) const; //!< render
        void     graphViz(const char            *fileName) const; //!< render
        void     hash_with(hashing::function &H)   const throw(); //!< H.set(); root->run(H)
        
        size_t   entries()   const throw(); //!< root->freq
        size_t   in_pool()   const throw(); //!< tree_pool.size
        void     gc(size_t nmax=0) throw(); //!< sort and keep no more than nmax in pool
        
    private:
        tree_node *root;
        tree_list  tree_pool;
        tree_node *new_tree_node(tree_node *, const uint8_t);
        void       increase_path_to(tree_node *node) throw();
        Y_DISABLE_COPY_AND_ASSIGN(affix);
    };
    
    
    
    
}

#endif
