//! \file
#ifndef Y_CORE_NODE_INCLUDED
#define Y_CORE_NODE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace core {
        
        //! a generic node with data
        template <typename T>
        class cpp_node_of : public object
        {
        public:
            Y_DECL_ARGS(T,type);  //!< alias
            cpp_node_of *next;       //!< for list
            cpp_node_of *prev;       //!< for list
            type         data;       //!< the expected data
            
            //! no arg constructor
            inline explicit cpp_node_of() : next(0), prev(0), data() {}
            
            //! constructor, if T has a copy constructor
            inline explicit cpp_node_of( param_type args ) : next(0), prev(0), data(args) {}
            
            //! destructor
            inline virtual ~cpp_node_of() throw() { assert(0==next); assert(0==prev); }
            
            //! copy, if T has a copy constructor
            inline cpp_node_of(const cpp_node_of &other) : next(0), prev(0), data(other.data) {}
            
            //! access
            inline type & operator*() throw() { return data; }
            
            //! const acces
            inline const_type & operator*() const throw() { return data; }
            
            
        private:
            Y_DISABLE_ASSIGN(cpp_node_of);
        };
    }
    
}

#endif

