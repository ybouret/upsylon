#include "y/yap/rational.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;

namespace {

   
    template <typename T, typename U, typename V>
    void doDOT( concurrent::for_each *loop )
    {
        std::cerr << "<DOT/MOD " << type_name_of<T>() << "," << type_name_of<U>()<< "," << type_name_of<V>() << ">" << std::endl;

        const U zu = 0;
        const V zv = 0;
        for(size_t iter=0;iter<16;++iter)
        {
            const size_t n = 1000 + alea.leq(1000);
            vector<U> u(n,zu);
            vector<V> v(n,zv);

            {
                support::fill1D(u);
                support::fill1D(v);
                const T seq = quark::dot<T>::of(u,v);
                {
                    const T seqb = quark::dot<T>::of(v,u);
                    std::cerr << "\tdot   : " << seq << "," << seqb;
                    Y_ASSERT( mod2_of(seq-seqb) <= 0 );
                }
                if(loop)
                {
                    const T par = quark::dot<T>::of(u,v,*loop);
                    std::cerr << "," << par << std::endl;
                }
                else
                {
                    std::cerr << std::endl;
                }
            }

            {
                support::fill1D(u);
                const T seq = quark::mod2<T>::of(u);
                std::cerr << "\tmodv1 : " << seq;
                if(loop)
                {
                    const T par = quark::mod2<T>::of(u,*loop);
                    std::cerr << "," << par << std::endl;
                }
                else
                {
                    std::cerr << std::endl;
                }
            }

            {
                support::fill1D(u);
                support::fill1D(v);
                const T seq = quark::mod2<T>::of(u,v);
                std::cerr << "\tmodv2 : " << seq;
                if(loop)
                {
                    const T par = quark::mod2<T>::of(u,v,*loop);
                    std::cerr << "," << par << std::endl;
                }
                else
                {
                    std::cerr << std::endl;
                }
            }



        }
        std::cerr << "<DOT/MOD/>" << std::endl;

    }

}

Y_UTEST(quark1_dot)
{
    concurrent::simd loop;
    doDOT<float,float,float>( &loop );
    doDOT<double,unit_t,unit_t>( &loop );
    doDOT<apz,apz,apz>( NULL );
}
Y_UTEST_DONE()

