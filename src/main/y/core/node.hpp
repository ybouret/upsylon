//! \file
#ifndef Y_CORE_NODE_INCLUDED
#define Y_CORE_NODE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace core
    {
        //! a generic node with data
        template <typename T>
        class node_of : public object
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            node_of *next;       //!< for list
            node_of *prev;       //!< for list
            type     data;       //!< the expected data

            //! no arg constructor
            inline explicit node_of() : next(0), prev(0), data() {}

            //! constructor, if T has a copy constructor
            inline explicit node_of( param_type args ) : next(0), prev(0), data(args) {}

            //! destructor
            inline virtual ~node_of() throw() {}

            //! copy, if T has a copy constructor
            inline node_of(const node_of &other) : next(0), prev(0), data(other.data) {}

        private:
            Y_DISABLE_ASSIGN(node_of);
        };
    }
}

#endif

