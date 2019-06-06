//! \file

#ifndef Y_COMPARATOR_INCLUDED
#define Y_COMPARATOR_INCLUDED 1

#include "y/comparison.hpp"
#include "y/type/args.hpp"

namespace upsylon
{

    //! prototype comparator
    template <typename T>
    class comparator
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias

        inline   comparator() throw() {} //!< setup
        inline  ~comparator() throw() {} //!< cleanup

        //! return comparison
        inline   int operator()(const_type &lhs, const_type &rhs) { return comparison::increasing<T>(lhs,rhs); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(comparator);
    };
    
}

#endif

