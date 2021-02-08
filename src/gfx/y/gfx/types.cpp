#include "y/gfx/types.hpp"
#include "y/exceptions.hpp"

namespace upsylon
{

    namespace GFX
    {
        unit_t Check:: GTZ(const unit_t w, const char *id, const char *fn)
        {
            assert(id); assert(fn);
            if(w<=0) throw exception("in %s: %s=%ld<=0", fn, id, long(w) );
            return w;
        }

        unit_t Check:: GEQZ(const unit_t w, const char *id, const char *fn)
        {
            assert(id); assert(fn);
            if(w<0) throw exception("in %s: %s=%ld<0", fn, id, long(w) );
            return w;
        }

        const char Check:: Width[] = "width";
        const char Check:: Height[] = "height";

#define Y_GFX_POS(NAME) case NAME : return #NAME
        const char * Position::Text(const unsigned p) throw()
        {
            switch(p)
            {
                    Y_GFX_POS(Core);
                    
                    Y_GFX_POS(Top);
                    Y_GFX_POS(Bottom);
                    Y_GFX_POS(Left);
                    Y_GFX_POS(Right);

                    Y_GFX_POS(Top|Left);
                    Y_GFX_POS(Top|Right);

                    Y_GFX_POS(Bottom|Left);
                    Y_GFX_POS(Bottom|Right);

                default:
                    break;
            }
            return "Invalid";
        }

        Y_GFX_POSITION_IMPL(InCore);
        Y_GFX_POSITION_IMPL(AtTop);
        Y_GFX_POSITION_IMPL(AtBottom);
        Y_GFX_POSITION_IMPL(AtLeft);
        Y_GFX_POSITION_IMPL(AtRight);

    }

}


