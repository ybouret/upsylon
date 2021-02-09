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

        const unsigned Position :: At[9] =
        {
            Core,

            Top,
            Bottom,
            Left,
            Right,

            Top|Left,
            Top|Right,
            Bottom|Left,
            Bottom|Right
        };

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

#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace GFX
    {
        Point Parse::WxH(const string &s)
        {
            static const char blanks[] = " \t";
            static const char fn[]     = "Parse::WxH: ";

            tokenizer<char> tkn(s);
            if(!tkn.next_with(":x"))  throw exception("%smissing w",fn);
            string ws(tkn.token(),tkn.units()); ws.clean_with(blanks);

            if(!tkn.next_with(blanks)) throw exception("%smissing h",fn);
            string hs(tkn.token(),tkn.units()); hs.clean_with(blanks);


            const unit_t w = string_convert::to<unit_t>(ws,"w");
            if(w<0) throw exception("%sw<0",fn);

            const unit_t h = string_convert::to<unit_t>(hs,"h");
            if(h<0) throw exception("%sh<0",fn);

            return Point(w,h);
        }

        Point Parse::WxH(const char *s)
        {
            const string _(s);
            return WxH(_);
        }
    }
}
