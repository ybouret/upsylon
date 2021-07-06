//! \file
#ifndef Y_CORE_NODE_INCLUDED
#define Y_CORE_NODE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace core {

        //______________________________________________________________________
        //
        //
        //! a generic node with data
        //
        //______________________________________________________________________

        template <typename T>
        class cpp_node_of : public object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);  //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! no arg constructor
            inline explicit cpp_node_of() : next(0), prev(0), data() {}
            
            //! constructor, if T has a copy constructor
            inline explicit cpp_node_of( param_type args ) : next(0), prev(0), data(args) {}
            
            //! destructor
            inline virtual ~cpp_node_of() throw() { assert(0==next); assert(0==prev); }
            
            //! copy, if T has a copy constructor
            inline cpp_node_of(const cpp_node_of &other) : next(0), prev(0), data(other.data) {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! access
            inline type & operator*() throw() { return data; }
            
            //! const acces
            inline const_type & operator*() const throw() { return data; }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            cpp_node_of *next;    //!< for list
            cpp_node_of *prev;    //!< for list
            type         data;    //!< the expected data

        private:
            Y_DISABLE_ASSIGN(cpp_node_of);
        };
    }
    
}

#endif

