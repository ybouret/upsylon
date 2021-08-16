#include "y/mkl/kernel/apk.hpp"

namespace upsylon
{
    namespace mkl
    {

        const char apk:: det_fcn[] = "apk::determinant";
        const char apk:: adj_fcn[] = "apk::adjoint";

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

        void apk:: simplify(addressable<apz> &d, matrix<apz> &num, const apz &den)
        {
            assert(d.size()>=num.rows);
            const size_t nr = num.rows;
            for(size_t i=nr;i>0;--i)
            {
                simplify(num[i],d[i]=den,NULL);
            }
        }

    }

}


#include "y/counting/comb.hpp"

namespace upsylon
{
    namespace mkl
    {

        size_t apk:: rank_of(const matrix<apz> &source)
        {

            const size_t nmax = source.rows;
            const size_t dims = source.cols;

            size_t count = 0;
            for(size_t n=nmax;n>0;--n)
            {
                combination comb(nmax,n);
                count += comb.count;
                const accessible<size_t> &indx = comb;
                matrix<apq> G(n,n);
                for(comb.boot();comb.good();comb.next())
                {
                    for(size_t i=n;i>0;--i)
                    {
                        const size_t      lid = indx[i];
                        const array<apz> &lhs = source[lid];
                        array<apq>       &tgt = G[i];
                        for(size_t j=i;j>0;--j)
                        {
                            const size_t      rid = indx[j];
                            const array<apz> &rhs = source[rid];
                            apq res = 0;
                            for(size_t k=dims;k>0;--k)
                            {
                                const apz p = lhs[k] * rhs[k];
                                res += p;
                            }
                            tgt[j] = res;
                        }
                    }

                    for(size_t i=n;i>0;--i)
                    {
                        for(size_t j=n;j>i;--j)
                        {
                            G[i][j] = G[j][i];
                        }
                    }

                    //std::cerr << "    " << "@" << indx << " G=" << G << std::endl;
                    const apq d = __determinant(G);
                    assert(d.den.is(1));
                    if(d!=0) return n;
                }
            }
            return 0;
        }
    }

}
