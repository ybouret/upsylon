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


    }

}

#if 0
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"

#include "y/parops.hpp"
#include "y/type/standard.hpp"

namespace upsylon
{

    namespace GFX
    {
        
        Stretch:: Stretch(const Area  &area,
                          const size_t size,
                          const size_t rank) throw() :
        work(0),
        begin(),
        end()
        {
            typedef core::standard<unit_t> ustd;

            unit_t length = area.n;
            unit_t offset = 0;
            parops::split_any(length,offset,size,rank);
            if(length>0)
            {
                aliasing::_(work) = length;
                {
                    const ustd::div_type l  = ustd::div_call(offset,area.w);
                    aliasing::_(begin.x) = l.rem;
                    aliasing::_(begin.y) = l.quot;
                }
                offset += length;
                {
                    const ustd::div_type l  = ustd::div_call(--offset,area.w);
                    aliasing::_(end.x) = l.rem;
                    aliasing::_(end.y) = l.quot;
                }
            }

        }

        Stretch:: Stretch(const Stretch &other) throw() :
        work(other.work),
        begin(other.begin),
        end(other.end)
        {
        }


        Stretch:: ~Stretch() throw()
        {
            _bzset(begin);
            _bzset(end);
        }


        Manager:: ~Manager() throw()
        {
        }
        
    }

}
#endif
