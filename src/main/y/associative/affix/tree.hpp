//! \file
#ifndef Y_AFFIX_TREE_INCLUDED
#define Y_AFFIX_TREE_INCLUDED 1

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
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit tree_node(tree_node *, const uint8_t) throw(); //!< setup with parent and code
                virtual ~tree_node() throw();                           //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void   optimize()                     throw(); //!< locally optimize
                void   graphViz(ios::ostream &fp)       const; //!< for GraphViz
                void   leaves_to(tree_list &pool)     throw(); //!< return leaves to pool
                void   return_to(tree_list &pool)     throw(); //!< leaves and this to pool
                void   run(hashing::function &) const throw(); //!< in-order hashing of codes
                size_t children()               const throw(); //!< leaves.size

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                void        *addr;   //!< any data
                tree_node   *next;   //!< for list
                tree_node   *prev;   //!< for list
                tree_node   *parent; //!< parent node
                tree_list    leaves; //!< leaves nodes
                size_t       freq;   //!< frequency
                size_t       deep;   //!< from root
                uint8_t      code;   //!< code



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
            tree_node *insert_at_path(ITERATOR     curr,
                                      size_t       size,
                                      void        *addr)
            {
                assert(addr!=NULL);
                assert(root!=NULL);
                assert(root->code==0);
                assert(root->deep==0);
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
                    return NULL;
                }
                else
                {
                    //----------------------------------------------------------
                    // assign address and optimize
                    //----------------------------------------------------------
                    node->addr = addr;
                    increase_path_to(node);
                    return node;
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

            //__________________________________________________________________
            //
            //! remove a used node
            /**
             with pruning!
             */
            //__________________________________________________________________
            void remove_node(tree_node *) throw();


            //__________________________________________________________________
            //
            // helpers to insert
            //__________________________________________________________________

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

            //__________________________________________________________________
            //
            // helpers to find
            //__________________________________________________________________

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


    template <typename T>
    class affix_tree
    {
    public:
        Y_DECL_ARGS(T,type);
        typedef core::affix            core_type;
        typedef core::affix::tree_node tree_node;

        class data_node
        {
        public:
            inline data_node(param_type args) :
            next(0),
            prev(0),
            hook(0),
            data(args)
            {
            }

            inline ~data_node() throw() {}


            data_node *next;
            data_node *prev;
            tree_node *hook;
            type       data;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_node);
        };

        typedef core::list_of<data_node> data_list;
        typedef core::pool_of<data_node> data_pool;


        inline explicit affix_tree() : dl(), dp(), db()
        {
        }

        inline virtual ~affix_tree() throw()
        {
            release_();
        }

        template <typename ITERATOR>
        bool insert_at(ITERATOR     iter,
                       const size_t size,
                       param_type   args)
        {
            data_node *dnode = create_data_node(args);
            try
            {
                tree_node *tnode = db.insert_at_path(iter,size,dnode);
                if(!tnode)
                {
                    delete_data_node(dnode);
                    return false;
                }
                else
                {
                    dl.push_back(dnode);
                    dnode->hook = tnode;
                    return true;
                }
            }
            catch(...)
            {
                delete_data_node(dnode);
                throw;
            }
        }

        inline void trim() throw()
        {
            while(dp.size)
            {
                data_node *node = dp.query();
                object::release1(node);
            }
        }

        inline void free() throw()
        {
            db.clear();
            while(dl.size)
            {
                delete_data_node(dl.pop_back());
            }
        }

    private:
        data_list   dl; //!< data list
        data_pool   dp; //!< data pool
        core::affix db; //!< data base

        inline void release_() throw()
        {
            db.clear();
            db.gc(0);
            trim();
            while(dl.size)
            {
                data_node *node = dl.pop_back();
                self_destruct(*node);
                object::release1(node);
            }
        }

        //! return a constructed data_node
        inline data_node  *create_data_node(const_type &args)
        {
            data_node *node = (dp.size>0) ? dp.query() : object::acquire1<data_node>();
            try {
                return new (node) data_node(args);
            } catch(...) {
                dp.store(node);
                throw;
            }
        }

        inline void delete_data_node(data_node *node) throw()
        {
            assert(node);
            self_destruct(*node);
            node->hook = NULL;
            dp.store(node);
        }

        Y_DISABLE_COPY_AND_ASSIGN(affix_tree);
    };
    
}

#endif
