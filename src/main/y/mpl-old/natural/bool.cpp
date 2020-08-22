
#include "y/mpl/natural.hpp"

namespace upsylon
{
    namespace mpl
    {
        natural natural::__bool(const uint8_t *l, const size_t nl,
                                const uint8_t *r, const size_t nr,
                                booleanOp      proc)
        {
            assert(l);
            assert(r);
            assert(proc);
            if(nl>=nr)
            {
                natural ans(nl,as_capacity);
                for(size_t i=0;i<nr;++i)
                {
                    ans.byte[i] = proc(l[i],r[i]);
                }
                for(size_t i=nr;i<nl;++i)
                {
                    ans.byte[i] = proc(l[i],0);
                }
                ans.bytes=nl;
                ans.update();
                return ans;
            }
            else
            {
                assert(nr>nl);
                natural ans(nr,as_capacity);
                for(size_t i=0;i<nl;++i)
                {
                    ans.byte[i] = proc(l[i],r[i]);
                }
                for(size_t i=nl;i<nr;++i)
                {
                    ans.byte[i] = proc(0,r[i]);
                }
                ans.bytes=nr;
                ans.update();
                return ans;
            }
        }

    }

}

