#include "y/mkl/kernel/apk.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace
        {
            class apn_ref : public object
            {
            public:
                apn_ref    *next;
                apn        &data;

                inline explicit apn_ref(apn &r) throw() :
                next(0),
                data(r)
                {
                    assert(data>0);
                }

                inline virtual ~apn_ref() throw()
                {
                }

                typedef core::pool_of_cpp<apn_ref> pool;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(apn_ref);
            };
        }

        void apk:: simplify(addressable<apz> &num, apz &den, apz *ratio)
        {
            apn &ini = aliasing::_(den.n);
            if(ini>0)
            {
                apn_ref::pool  positive;
                positive.store( new apn_ref(ini) );
                std::cerr << "store ini=" << ini << std::endl;
                for(size_t i=num.size();i>0;--i)
                {
                    apn &ref = aliasing::_(num[i].n);
                    if(ref>0)
                    {
                        positive.store( new apn_ref(ref) );
                        std::cerr << "store ref=" << ref << std::endl;
                    }
                }

            }

        }

#if 0
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
#endif

        void apk::simplify(matrix<apz> &num, apz &den, apz *ratio)
        {
            lightweight_array<apz> data = num.as_array();
            simplify(data,den,ratio);
        }

    }

}
