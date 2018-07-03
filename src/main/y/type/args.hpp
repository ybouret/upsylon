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
#define    Y_DECL_ARGS(T)                                                   \
typedef T                                        type;                      \
typedef typename type_traits<T>::mutable_type    mutable_type;              \
typedef const mutable_type                       const_type;                \
typedef typename                                                            \
upsylon::select_type<                                                       \
type_traits<mutable_type>::is_primitive, /* no references in containers */  \
const_type  , /* pass primitive mutable types by const value            */  \
const_type &  /* pass non primitive mutable types by const reference    */  \
>::result param_type


}

#endif

