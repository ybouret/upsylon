//!\file
#ifndef Y_TYPE_MARK_INCLUDED
#define Y_TYPE_MARK_INCLUDED 1

#include "y/type/spec.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    //! base type for system wide type_mark
    typedef arc_ptr<const type_spec> type_mark_;

    //! system wide type mark, shared pointer on unique type_spec
    class type_mark : public type_mark_
    {
    public:
        type_mark(const type_spec &) throw();             //!< setup from existing type_spec
        type_mark(const type_mark &) throw();             //!< no-throw copy
        type_mark & operator=(const type_mark &) throw(); //!< no-throw assign
        virtual ~type_mark() throw();                     //!< cleanup
        type_mark( const std::type_info &tid );           //!< setup using type_spec::declare

    };

}

#endif

