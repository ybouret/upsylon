#include "y/ipso/field.hpp"

namespace upsylon
{
    namespace ipso
    {
        field_info:: ~field_info() throw()
        {
        }

        field_info:: field_info(const string &id,const size_t sz) :
        name(id),
        item_size(sz),
        bytes(0)
        {
            assert(item_size>0);
        }

        field_info:: field_info(const char *id,const size_t sz) :
        name(id),
        item_size(sz),
        bytes(0)
        {
            assert(item_size>0);
        }

    }
}
