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

        size_t apk:: gram_rank_of(const matrix<apz> &source)
        {

            const size_t nmax = source.rows;
            const size_t dims = source.cols;

            for(size_t n=nmax;n>0;--n)
            {
                combination               comb(nmax,n);
                const accessible<size_t> &indx = comb;
                matrix<apq>               G(n,n);


                for(comb.boot();comb.good();comb.next())
                {

                    // compute half Gram Matrix
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

                    // symetrize Gram Matrix
                    for(size_t i=n;i>0;--i)
                    {
                        for(size_t j=n;j>i;--j)
                        {
                            G[i][j] = G[j][i];
                        }
                    }

                    if( LU::build(G) ) return n;
#if 0
                    const apq d = __determinant(G);
                    assert(d.den.is(1));
                    if(d!=0) return n;
#endif

                }
            }
            return 0;
        }

    }
}

#include "y/exceptions.hpp"
#include "y/yap/gcd.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sort/index.hpp"
#include <cerrno>

namespace upsylon
{
    namespace mkl
    {
        const char apk:: complete_ortho_fn[] = "apk::complete_ortho";

        void apk:: reduce(addressable<apz> &arr)
        {
            (void)yap::compute_gcd::simplify(arr);
        }
        
        void apk:: complete_ortho_(const matrix<apz> &U,
                                   matrix<apz>       &V)
        {
            static const char * const fn = complete_ortho_fn;

            assert(U.cols==V.cols);
            assert(U.rows+V.rows==U.cols);

            const size_t primary = U.rows;
            const size_t replica = V.rows;
            const size_t dim     = U.cols;

            // special cases

            // projection on U_ortho
            matrix<apz> QT(dim,dim);
            {
                matrix<apz> Q(dim,dim);
                {
                    matrix<apz> P(dim,dim);
                    apz         dP = 0;
                    {
                        matrix<apz> aU2(primary,primary);
                        {
                            matrix<apz> U2(primary,primary);
                            tao::gram(U2,U);
                            dP = apk::determinant(U2); if(0==dP) throw libc::exception(EDOM,"%s with singular primary space",fn);
                            apk::adjoint(aU2,U2);
                        }
                        apk::simplify(aU2,dP,NULL);
                        matrix<apz> aU3(primary,dim);
                        tao::mmul(aU3,aU2,U);
                        tao::mmul_ltrn(P,U,aU3);
                    }
                    apk::simplify(P,dP,NULL);
                    const apz zero = 0;
                    Q.diag(dP,zero);
                    tao::msub(Q,P);
                }
                QT.assign_transpose(Q);

            }

            vector<apn,memory::dyadic>    Qn(dim,0);
            vector<size_t,memory::dyadic> iQ(dim,0);

            for(size_t i=dim;i>0;--i)
            {
                addressable<apz> &Qi = QT[i];
                yap::compute_gcd::simplify(Qi);
                apn qn = 0;
                for(size_t j=dim;j>0;--j) qn += Qi[j].n;
                Qn[i].xch(qn);
            }
            indexing::make(iQ,apn::compare,Qn);
            //std::cerr << "QT=" << QT << std::endl;
            //std::cerr << "Qn=" << Qn << std::endl;
            //std::cerr << "iQ=" << iQ << std::endl;

            // find a subspace
            bool        foundV = false;
            combination comb(dim,replica);

            for(comb.boot();comb.good();comb.next())
            {
                //std::cerr << "Try " << (counting &)comb << std::endl;
                for(size_t i=replica;i>0;--i)
                {
                    tao::set(V[i],QT[ iQ[comb[i]] ]);
                }
                if(has_maximal_rank(V))
                {
                    foundV = true;
                    break;
                }
            }

            if(!foundV) throw libc::exception(EDOM,"%s with singular replica space",fn);
        }

    }

}
