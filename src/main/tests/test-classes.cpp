#include "y/sort/classes.hpp"
#include "y/sparse/matrix.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

#include "y/sort/sequence.hpp"
#include "y/mpl/rational.hpp"

using namespace upsylon;

namespace
{
    template <typename SEQUENCE>
    void count_classes( SEQUENCE &seq )
    {
        sort_sequence(seq, comparison::increasing<typename SEQUENCE::type> );
        const size_t nci = classes(seq);
        std::cerr << "seq=" << seq << std::endl;
        std::cerr << "#cls=" << nci << "/" << seq.size() << std::endl;

        sort_sequence(seq, comparison::decreasing<typename SEQUENCE::type> );
        const size_t ncd = classes(seq);
        std::cerr << "seq=" << seq << std::endl;
        std::cerr << "#cls=" << ncd << "/" << seq.size() << std::endl;
        Y_CHECK(nci==ncd);
        std::cerr << std::endl;
    }
}

Y_UTEST(classes)
{


    vector<size_t> v;
    list<size_t>   l;

    vector<mpn>    vn;
    list<mpn>      ln;

    vector<mpz>    vz;
    list<mpz>      lz;

    vector<mpq>    vq;
    list<mpq>      lq;

    for(size_t i=10+alea.leq(30);i>0;--i)
    {
        const size_t j = alea.leq(10);
        v.push_back( j );
        l.push_back( j );
        {
            const mpn n = j;
            vn.push_back(n);
            ln.push_back(n);
        }

        {
            unit_t k = unit_t(j);
            if( alea.choice() ) k = -k;
            const mpz z = k;
            vz.push_back(z);
            lz.push_back(z);

            const size_t d = 1+alea.leq(3);
            const mpq q(k,d);
            vq.push_back(q);
            lq.push_back(q);
        }

    }

    count_classes(v);
    count_classes(l);

    count_classes(vn);
    count_classes(ln);

    count_classes(vz);
    count_classes(lz);

    count_classes(vq);
    count_classes(lq);

}
Y_UTEST_DONE()


