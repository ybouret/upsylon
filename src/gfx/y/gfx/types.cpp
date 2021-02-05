
#include "y/gfx/types.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{

    namespace GFX
    {
        Area:: ~Area() throw()
        {
            _bzset(x);
            _bzset(y);
            _bzset(w);
            _bzset(h);
            _bzset(n);
        }

        Area::  Area(const unit_t W, const unit_t H):
        x(0),
        y(0),
        w(W), h(H), n(w*h)
        {
            assert(w>=0);
            assert(h>=0);
        }

        Area:: Area(const unit_t X, const unit_t Y, const unit_t W, const unit_t H) :
        x(X),
        y(Y),
        w(W),
        h(H),
        n(w*h)
        {
            assert(w>=0);
            assert(h>=0);
        }
    }

}

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
