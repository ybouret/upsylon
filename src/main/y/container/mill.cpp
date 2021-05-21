
#include "y/container/mill.hpp"
#include "y/type/rtti.hpp"

namespace upsylon
{
    mill:: ~mill() throw()
    {}


    mill:: mill(const std::type_info &tid) : typeName( rtti::name_of(tid) )
    {
    }

    const at_exit::longevity mill::maximum_life_time = rtti::repo::life_time-1;

    const char mill:: CLID[] = "factory";

}
