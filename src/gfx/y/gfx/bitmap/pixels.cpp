

#include "y/gfx/bitmap/pixels.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace GFX
    {
        static inline
        void * make_pixels( size_t &bytes )
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            return mgr.acquire(bytes);
        }

        Pixels_:: Pixels_(const size_t n) :
        bytes(n),
        entry( make_pixels( aliasing::_(bytes) ) )
        {
            
        }

        Pixels_:: ~Pixels_() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.release( aliasing::_(entry),aliasing::_(bytes));
        }

        std::ostream & operator<<(std::ostream &os, const Pixels_ &px)
        {
            os << "(@" << px.entry << "+" << px.bytes << ")";
            return os;
        }
        
    }
}
