#include "y/mkl/kernel/apk.hpp"

namespace upsylon
{
    namespace mkl
    {
        apz apk:: simplify( addressable<apz> &num, apz &den )
        {
            apz        r      = 1;
            const apn &ref    = den.n;
            apn        lower  = ref;
            if(lower>0)
            {
                const size_t count = num.size();
                apn          upper = lower;

                // get lower/upper bound
                for(size_t i=count;i>0;--i)
                {
                    const apn &a = num[i].n;
                    if(a>0)
                    {
                        if(a<lower)
                        {
                            lower = a;
                        }
                        else
                        {
                            if(a>upper)
                            {
                                upper=a;
                            }
                        }
                    }
                }

                // find divider
                if(upper>0)
                {
                    for(apn d=lower;d>1;--d)
                    {
                        if( !ref.is_divisible_by(d) ) continue;
                        bool ok = true;
                        for(size_t i=count;i>0;--i)
                        {
                            const apn &a = num[i].n;
                            if(a>0&&!a.is_divisible_by(d))
                            {
                                ok=false;
                                break;
                            }
                        }
                        if(ok)
                        {
                            r=d;
                            break;
                        }
                    }

                    if(r>1)
                    {
                        den /= r;
                        for(size_t i=count;i>0;--i)
                        {
                            num[i] /= r;
                        }
                    }
                }

            }
            return r;
        }

        apz apk::simplify(matrix<apz> &num, apz &den)
        {
            lightweight_array<apz> data = num.as_array();
            return simplify(data,den);
        }

    }

}
