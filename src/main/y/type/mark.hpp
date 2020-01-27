//!\file

#ifndef Y_TYPE_MARK_INCLUDED
#define Y_TYPE_MARL_INCLUDED 1

#include "y/type/spec.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    typedef arc_ptr<const type_spec> type_mark_;

    class type_mark : public type_mark_
    {
    public:
        type_mark( const type_spec &ts) throw();
        type_mark( const type_mark &other) throw();
        type_mark & operator=(const type_mark &other) throw();
        virtual ~type_mark() throw();
    };

}

#endif

