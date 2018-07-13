//! \file
#ifndef Y_CORE_NODE_INCLUDED
#define Y_CORE_NODE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace core
    {
        template <typename T>
        class node_of : public object
        {
        public:
            Y_DECL_ARGS(T,type);
            node_of *next;
            node_of *prev;
            type     data;

            //! no arg constructor
            inline explicit node_of() : next(0), prev(0), data() {}

            //! copy data in constructor
            inline explicit node_of( param_type args ) : next(0), prev(0), data(args) {}

            inline virtual ~node_of() throw() {}
            inline node_of(const node_of &other) : next(0), prev(0), data(other.data) {}



        private:
            Y_DISABLE_ASSIGN(node_of);
        };
    }
}

#endif

