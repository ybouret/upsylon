
#include "y/container/factory.hpp"
#include "y/exception.hpp"
#include "y/type/rtti.hpp"


namespace upsylon {

#if 0
    void factory_::throw_multiple_keys( ) const
    {
        throw exception("multiple key for [%s] creator",*typeName);
    }

    void factory_:: throw_no_creator() const
    {
        throw exception("no key for creator from [%s]",*typeName);
    }

    factory_:: ~factory_() throw()
    {
    }

    factory_:: factory_(const std::type_info &tid) :
    typeName( rtti::name_of(tid) )
    {

    }

    const at_exit::longevity factory_::minimum_life_time = rtti::repo::life_time-1;
#endif
}
