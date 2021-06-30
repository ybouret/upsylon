

//! \file
#ifndef Y_CORE_SNODE_INCLUDED
#define Y_CORE_SNODE_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon {


    //! minimal interface class to give single linked node properties to another class
    template <typename T>
    class snode
    {
    public:
        Y_DECL_ARGS_(T,type); //!< alias
        mutable_type *next;  //!< mutable object for a list

        inline explicit snode() throw() : next(0) {}                    //!< initialize
        inline virtual ~snode() throw() {assert(0==next);}              //!< destructor
        inline bool     is_linked() const throw() { return (0!=next); } //!< linked to another
        inline bool     is_single() const throw() { return (0==next); } //!< not in list
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(snode);
    };


}

#endif

