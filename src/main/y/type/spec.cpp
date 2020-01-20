#include "y/type/spec.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    type_spec::alias:: ~alias() throw()
    {
    }

    type_spec:: alias:: alias( const string &id ) :
    next(0), prev(0), name(id) {}
    

    type_spec:: ~type_spec() throw()
    {

    }

#define Y_TYPE_SPEC_CTOR() \
info( tid ),               \
uuid( tid.name() ),        \
user()

    type_spec:: type_spec( const std::type_info &tid ) :
    Y_TYPE_SPEC_CTOR()
    {

    }

    type_spec:: type_spec(const std::type_info &tid, const string &known) :
    Y_TYPE_SPEC_CTOR()
    {
        aka(known);
    }

    type_spec:: type_spec(const std::type_info &tid, const char *known) :
    Y_TYPE_SPEC_CTOR()
    {
        aka(known);
    }


    void type_spec:: aka( const string &usr )
    {
        for(const alias *scan=user.head; scan; scan=scan->next)
        {
            if(usr==scan->name) return;
        }
        aliasing::_(user).push_back( new alias(usr) );
    }

    void type_spec:: aka(const char *usr)
    {
        const string _(usr); aka(_);
    }

    const string & type_spec:: name() const throw()
    {
        return uuid;
    }

}
