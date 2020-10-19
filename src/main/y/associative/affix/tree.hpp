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
            class node : public object
            {
            public:
                void   *addr; //!< any data
                node   *next; //!< for list
                node   *prev; //!< for list
                size_t  freq; //!< frequency
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(node);
            };

            explicit affix();

            virtual ~affix() throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(affix);
        };

    }
    
}

#endif
