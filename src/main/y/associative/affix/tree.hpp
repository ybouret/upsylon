//! \file
#ifndef Y_AFFIX_TREE_INCLUDED
#define Y_AFFIX_TREE_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/object.hpp"
#include "y/ios/iosfwd.hpp"
#include "y/strfwd.hpp"

namespace upsylon
{

    namespace core
    {

        class affix
        {
        public:
            class                                tree_node;
            typedef core::list_of_cpp<tree_node> tree_list;

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

            private:
                Y_DISABLE_COPY_AND_ASSIGN(tree_node);
            };

            class data_node : public object
            {
            public:
                explicit data_node(tree_node *node) throw();
                data_node *next;
                data_node *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(data_node);
            };
            typedef core::list_of_cpp<data_node> data_list;

            template <typename ITERATOR>
            tree_node *insert_with(ITERATOR     curr,
                                   size_t       size,
                                   void        *addr)
            {
                assert(addr!=NULL);
                assert(root!=NULL);
                tree_node *node = root;
                while(size-- > 0)
                {
                    const uint8_t code = *(curr++);
                    // look for code
                    for(tree_node *chld=node->leaves.head;chld;chld=chld->next)
                    {
                        if(code==chld->code)
                        {
                            // take this path
                            node = chld;
                            goto FOUND;
                        }
                    }
                    // need a new leaf
                    node = node->leaves.push_back( new_tree_node(node,code) );
                FOUND:;
                }
                assert(node!=NULL);
                if(node->addr)
                {
                    // busy
                    return NULL;
                }
                else
                {
                    // update frequencies...
                    node->addr = addr;
                    return node;
                }
            }

            explicit affix();
            virtual ~affix() throw();

            void     graphViz(const upsylon::string &fileName) const;
            void     graphViz(const char            *fileName) const;

        private:
            tree_node *root;
            data_list  data;
            tree_list  tree_pool;
            data_list  data_pool;
            tree_node *new_tree_node(tree_node *, const uint8_t);

            Y_DISABLE_COPY_AND_ASSIGN(affix);
        };

    }
    
}

#endif
