
#include "y/graphic/parallel/tile.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {


        Tile:: ~Tile() throw()
        {
        }


        static const char fn[] = "Graphic::Tile: ";

        static inline size_t checkW(const Point &lo, const Point &up)
        {
            const unit_t w = 1+up.x-lo.x;
            if(w<0) throw exception("%snegative width",fn);
            return w;
        }

        static inline size_t checkH(const Point &lo, const Point &up)
        {
            const unit_t h = 1+up.y-lo.y;
            if(h<0) throw exception("%snegative height",fn);
            return h;
        }

        Tile:: Tile( const Point &lo, const Point &up ) :
        Area( checkW(lo,up), checkH(lo,up) ),
        lower(lo),
        upper(up),
        data(0),
        size(0)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Tile &tile)
        {
            std::cerr << "[ [" << tile.w << "x" << tile.h << "]=" << tile.items << "@" << tile.lower << "->" << tile.upper << "]";
            return os;
        }

    }

}


