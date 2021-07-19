
#include "y/gfx/color/size-to-rgba.hpp"
#include "y/gfx/color/named.hpp"

namespace upsylon
{
    namespace graphic
    {
        size_to_rgba:: ~size_to_rgba() throw()
        {
        }

        size_to_rgba:: size_to_rgba(const size_t s) :
        type_to_rgba(),
        shift(s)
        {
        }

        size_to_rgba:: size_to_rgba(const size_to_rgba &other) throw() :
        type_to_rgba(),
        shift(other.shift)
        {
        }
        

        unit_t size_to_rgba:: depth() const throw()
        {
            return sizeof(size_t);
        }

        rgba  size_to_rgba:: operator()(const void *addr) const throw()
        {
            assert(addr);
            const size_t value = *static_cast<const size_t *>(addr);
            return named_color::get(value+shift);
        }

    }

}

