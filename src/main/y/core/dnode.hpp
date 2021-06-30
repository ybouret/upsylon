
//! \file
#ifndef Y_CORE_DNODE_INCLUDED
#define Y_CORE_DNODE_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon {
    
    
    //! minimal interface class to give double linked node properties to another class
    template <typename T>
    class dnode
    {
    public:
        Y_DECL_ARGS_(T,type); //!< alias
        mutable_type *next;   //!< mutable object for a list
        mutable_type *prev;   //!< mutable object for a list
        
        inline explicit dnode() throw() : next(0), prev(0) {}                        //!< initialize
        inline virtual ~dnode() throw() {assert(0==next); assert(0==prev);}          //!< destructor
        inline bool     is_linked() const throw() { return (0!=next) || (0!=prev); } //!< linked to another
        inline bool     is_single() const throw() { return (0==next) && (0==prev); } //!< not in list
    private:
        Y_DISABLE_COPY_AND_ASSIGN(dnode);
    };
    
    
}

#endif

