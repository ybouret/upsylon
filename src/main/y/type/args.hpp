//! \file
#ifndef Y_TYPE_ARGS_INCLUDED
#define Y_TYPE_ARGS_INCLUDED 1

#include "y/type/traits.hpp"

namespace upsylon
{

    /** \brief container like arguments
     * streams, containers,..., can't contain references,
     * so these are the aliases used for in/out parameters passing
     */
#define    Y_DECL_ARGS(T,NAME)                                                \
typedef T                                        NAME;                        \
typedef typename type_traits<T>::mutable_type    mutable_##NAME;              \
typedef const mutable_##NAME                     const_##NAME;                \
typedef typename                                                              \
upsylon::select_type<                                                         \
type_traits<mutable_##NAME>::is_primitive, /* no references in containers */  \
const_##NAME  , /* pass primitive mutable types by const value            */  \
const_##NAME &  /* pass non primitive mutable types by const reference    */  \
>::result param_##NAME
    


}

#endif

