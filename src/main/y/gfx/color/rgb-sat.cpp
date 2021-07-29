
#include "y/gfx/color/rgb-sat.hpp"
#include "y/sort/network/sort3.hpp"
#include "y/sort/network/index.hpp"
#include "y/comparison.hpp"


namespace upsylon
{
    namespace graphic
    {


        static
#include "saturated.inc"




        static inline int decreasing_chan(const uint8_t &lhs, const uint8_t &rhs) throw()
        {
            return int(rhs)-int(lhs);
        }

        rgb saturated::of(const rgb &src) throw()
        {
            const uint8_t *chan    = &src.r;
            size_t         indx[3] = {0,0,0};
            newtork_index<3>::make(indx,chan,decreasing_chan);
            assert( chan[ indx[0] ] >= chan[ indx[1] ]);
            assert( chan[ indx[1] ] >= chan[ indx[2] ]);
            const unsigned m = chan[ indx[0] ];
            rgb            c(0,0,0);
            if(m>0)
            {
                const uint8_t *sat = &saturated_table[ (m*(m+1))>>1 ];
                uint8_t       *tgt = &c.r;
                tgt[ indx[0] ]     = 0xff;
                tgt[ indx[1] ]     = sat[ chan[indx[1]] ];
                tgt[ indx[2] ]     = sat[ chan[indx[2]] ];
            }
            return c;

        }

        rgba saturated::of(const rgba &src) throw()
        {
            const rgb c = src;
            const rgb s = of(c);
            return rgba(s,src.a);
        }

    }

}
