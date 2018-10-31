#include "y/code/property.hpp"
#include "y/exceptions.hpp"

namespace upsylon
{
    ppty:: ~ppty() throw() {}

    ppty:: ppty(const char *id, const bool rw) :
    name(id),
    writable(rw)
    {
    }

    bool ppty:: is_writable() const throw() { return writable; }

    void ppty:: check_writable() const
    {
        if(!writable) throw imported::exception("property write failue","[%s] is read-only", *name);
    }
}
