
#include "y/gfx/edges/dvertex-to-rgba.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {
            dvertex_to_rgba::  dvertex_to_rgba() throw() : type_to_rgba()
            {
            }

            dvertex_to_rgba:: ~dvertex_to_rgba() throw()
            {
            }


            unit_t dvertex_to_rgba:: depth() const throw()
            {
                return sizeof(dvertex::type);
            }

            rgba dvertex_to_rgba:: operator()(const void *addr) const throw()
            {
                assert(addr);
                const dvertex::type &vtx = *static_cast<const dvertex::type *>(addr);
                const size_t         idx = dvertex::index_of(vtx);
                assert(idx<directions);
                return colors[idx];
            }


        }

    }

}

#include "y/gfx/color/named.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {
            const rgba dvertex_to_rgba:: colors[directions] =
            {
                Y_BLACK,

                Y_RED,
                Y_CYAN,
                Y_MAGENTA,
                Y_GREEN,

                Y_YELLOW,
                Y_BLUE,
                Y_ORANGE,
                Y_PURPLE

            };

        }
    }
}

