
//! \file
#ifndef Y_CORE_INODE_INCLUDED
#define Y_CORE_INODE_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon {
    
    
    //! minimal interface class to give node properties to another class
    template <typename T>
    class inode
    {
    public:
        Y_DECL_ARGS_(T,type); //!< alias
        mutable_type *next;  //!< mutable object for a list
        mutable_type *prev;  //!< mutable object for a list
        
        inline explicit inode() throw() : next(0), prev(0) {}                        //!< initialize
        inline virtual ~inode() throw() {assert(0==next); assert(0==prev);}          //!< destructor
        inline bool     is_linked() const throw() { return (0!=next) || (0!=prev); } //!< linked to another
        inline bool     is_single() const throw() { return (0==next) && (0==prev); } //!< not in list
    private:
        Y_DISABLE_COPY_AND_ASSIGN(inode);
    };
    
    
}

#endif

