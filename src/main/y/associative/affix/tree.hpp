//! \file
#ifndef Y_AFFIX_TREE_INCLUDED
#define Y_AFFIX_TREE_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/object.hpp"

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
                void        *addr; //!< any data
                tree_node   *next; //!< for list
                tree_node   *prev; //!< for list
                size_t       freq; //!< frequency

                explicit tree_node() throw();
                virtual ~tree_node() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(tree_node);
            };

            explicit affix();

            virtual ~affix() throw();


        private:
            tree_node *root;
            Y_DISABLE_COPY_AND_ASSIGN(affix);
        };

    }
    
}

#endif
