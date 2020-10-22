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

            static inline apn gcd_of(const apn_ref::pool &positive)
            {
                const apn_ref *node = positive.head;
                apn            gcd  = node->data;
                for(node=node->next;node;node=node->next)
                {
                    gcd = apn::gcd(gcd,node->data);
                    if(1==gcd) return gcd;
                }
                return gcd;
            }
        }

        void apk:: simplify(addressable<apz> &num, apz &den, apz *ratio)
        {
            apn &ini = aliasing::_(den.n);
            if(ini>0)
            {
                apn_ref::pool  positive;
                positive.store( new apn_ref(ini) );
                for(size_t i=num.size();i>0;--i)
                {
                    apn &ref = aliasing::_(num[i].n);
                    if(ref>0)
                    {
                        positive.store( new apn_ref(ref) );
                    }
                }
                const apn gcd = gcd_of(positive);
                if(gcd>1)
                {
                    while(positive.size)
                    {
                        assert(positive.head->data.is_divisible_by(gcd));
                        positive.head->data /= gcd;
                        positive.pop();
                    }
                }
                if(ratio) *ratio = gcd;
            }

        }


        void apk::simplify(matrix<apz> &num, apz &den, apz *ratio)
        {
            lightweight_array<apz> data = num.as_array();
            simplify(data,den,ratio);
        }

    }

}
