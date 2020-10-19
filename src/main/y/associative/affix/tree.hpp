//! \file
#ifndef Y_AFFIX_TREE_INCLUDED
#define Y_AFFIX_TREE_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/object.hpp"
#include "y/ios/iosfwd.hpp"
#include "y/strfwd.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace hashing
    {
        class function; //!< forward declaration
    }

    namespace core
    {

        //______________________________________________________________________
        //
        //
        //! basic operations on an affix tree
        //
        //______________________________________________________________________
        class affix
        {
        public:
            //______________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            class                                tree_node; //!< forward declaration
            typedef core::list_of_cpp<tree_node> tree_list; //!< alias

            //__________________________________________________________________
            //
            //! internal tree node
            //__________________________________________________________________
            class tree_node : public object
            {
            public:

                explicit tree_node(tree_node *, const uint8_t) throw();
                virtual ~tree_node() throw();

                void     optimize()  throw(); //!< locally optimize

                void        *addr;   //!< any data
                tree_node   *next;   //!< for list
                tree_node   *prev;   //!< for list
                tree_node   *parent; //!< parent node
                tree_list    leaves; //!< leaves nodes
                size_t       freq;   //!< frequency
                uint8_t      code;   //!< code

                void         graphViz(ios::ostream &fp) const;
                void         leaves_to(tree_list &pool) throw();
                void         return_to(tree_list &pool) throw();
                void         run(hashing::function &) const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(tree_node);
            };

            //__________________________________________________________________
            //
            //! insert address following a path
            /**
             on the fly nodes creation
             */
            //__________________________________________________________________
            template <typename ITERATOR>
            bool insert_at_path(ITERATOR     curr,
                                size_t       size,
                                void        *addr)
            {
                assert(addr!=NULL);
                assert(root!=NULL);
                //--------------------------------------------------------------
                // start from root
                //--------------------------------------------------------------
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
                            //--------------------------------------------------
                            // take this path
                            //--------------------------------------------------
                            node = chld;
                            goto FOUND;
                        }
                    }
                    //----------------------------------------------------------
                    // need a new leaf
                    //----------------------------------------------------------
                    node = node->leaves.push_back( new_tree_node(node,code) );
                FOUND:;
                }
                assert(node!=NULL);
                if(node->addr)
                {
                    //----------------------------------------------------------
                    // busy
                    //----------------------------------------------------------
                    return false;
                }
                else
                {
                    //----------------------------------------------------------
                    // assign address and optimize
                    //----------------------------------------------------------
                    node->addr = addr;
                    increase_path_to(node);
                    return true;
                }
            }

            //__________________________________________________________________
            //
            //! search a node by path
            //__________________________________________________________________
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

            //! helper
            bool insert_with(const char  *text,
                             const size_t size,
                             void *       addr);

            //! helper
            bool insert_with(const char  *text,
                             void *       addr);

            //! helper
            bool insert_with(const memory::ro_buffer &buff,
                             void *                   addr);

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

            void     graphViz(const upsylon::string &fileName) const;
            void     graphViz(const char            *fileName) const;
            void     hash_with(hashing::function &H) const throw(); //!< H.set(); root->run(H)

            size_t   entries() const throw(); //!< root->freq

        private:
            tree_node *root;
            tree_list  tree_pool;
            tree_node *new_tree_node(tree_node *, const uint8_t);
            void       increase_path_to(tree_node *node) throw();
            Y_DISABLE_COPY_AND_ASSIGN(affix);
        };

    }
    
}

#endif
