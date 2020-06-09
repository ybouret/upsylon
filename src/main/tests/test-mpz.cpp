#include "y/mpl/integer.hpp"
#include "y/comparison.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define ITER (1<<14)

Y_UTEST(mpz)
{
    std::cerr << "-- mpz setup and compare" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = alea.partial<int64_t>() * alea.pm();
        const int64_t j   = alea.partial<int64_t>() * alea.pm();
        const int     cmp = comparison::increasing(i,j);

        const mpz I   = i; Y_ASSERT(I.lsi()==i);
        const mpz J   = j; Y_ASSERT(J.lsi()==j);
        const int CMP = mpz::compare(I,J);
        Y_ASSERT(cmp==CMP);
    }

    std::cerr << "-- mpz addition" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = alea.partial<int64_t>(61) * alea.pm();
        const int64_t j   = alea.partial<int64_t>(61) * alea.pm();
        const int64_t s   = i+j;
        const mpz I   = i;   Y_ASSERT(I.lsi()==i);
        const mpz J   = j;   Y_ASSERT(J.lsi()==j);
        const mpz S   = I+J; Y_ASSERT(S.lsi()==s);
    }
    std::cerr << "-- mpz loop++" << std::endl;
    for(mpz i=-10;i<=10;i++)
    {
        std::cerr << i << '/';
    }
    std::cerr << std::endl;
    std::cerr << "-- mpz ++loop" << std::endl;
    for(mpz i=-10;i<=10;++i)
    {
        std::cerr << i << '/';
    }
    std::cerr << std::endl;

    std::cerr << "-- mpz subtraction" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = alea.partial<int64_t>(61) * alea.pm();
        const int64_t j   = alea.partial<int64_t>(61) * alea.pm();
        const int64_t d   = i-j;
        const mpz I   = i;   Y_ASSERT(I.lsi()==i);
        const mpz J   = j;   Y_ASSERT(J.lsi()==j);
        const mpz S   = I-J; Y_ASSERT(S.lsi()==d);
    }
    std::cerr << std::endl;

    std::cerr << "-- mpz loop--" << std::endl;
    for(mpz i=10;i>=-10;i--)
    {
        std::cerr << i << '/';
    }
    std::cerr << std::endl;

    std::cerr << "-- mpz --loop" << std::endl;
    for(mpz i=10;i>=-10;--i)
    {
        std::cerr << i << '/';
    }
    std::cerr << std::endl;

    std::cerr << "-- conversion" << std::endl;

    std::cerr << "|_8bits" << std::endl;
    for(int i=limit_of<int8_t>::minimum;i<=limit_of<int8_t>::maximum;++i)
    {
        const int8_t I = int8_t(i);
        const mpz    Z = I;
        int8_t       J = 0;
        if(!Z.to(J)) throw exception("cannot recover %d", i);
        if(J!=I)     throw exception("found %d instead of %d",int(J),i);
    }

    std::cerr << "|_16bits" << std::endl;
    for(int i=limit_of<int16_t>::minimum;i<=limit_of<int16_t>::maximum;++i)
    {
        const int16_t I = int16_t(i);
        const mpz     Z = I;
        int16_t       J = 0;
        if(!Z.to(J)) throw exception("cannot recover %d", i);
        if(J!=I)     throw exception("found %d instead of %d",int(J),i);
    }

    std::cerr << "|_32bits" << std::endl;

    {
        mpz z = limit_of<int32_t>::minimum;
        Y_CHECK( z.cast_to<int32_t>() == limit_of<int32_t>::minimum);
        z = limit_of<int32_t>::maximum;
        Y_CHECK( z.cast_to<int32_t>() == limit_of<int32_t>::maximum);
    }

    for(int iter=0;iter<1000000;++iter)
    {
        const int32_t I = alea.full<int32_t>();
        const mpz     Z = I;
        int32_t       J = 0;
        if(!Z.to(J)) throw exception("cannot recover %ld", long(I));
        if(J!=I)     throw exception("found %ld instead of %ld",long(J),long(I));
    }

    std::cerr << "|_64bits" << std::endl;
    {
        mpz z = limit_of<int64_t>::minimum;
        Y_CHECK( z.cast_to<int64_t>() == limit_of<int64_t>::minimum);
        z = limit_of<int64_t>::maximum;
        Y_CHECK( z.cast_to<int64_t>() == limit_of<int64_t>::maximum);
    }

    for(int iter=0;iter<1000000;++iter)
    {
        const int64_t I = alea.full<int64_t>();
        const mpz     Z = I;
        int64_t       J = 0;
        if(!Z.to(J)) throw exception("cannot recover 64bits");
        if(J!=I)     throw exception("mismatch for 64bits");
    }

    std::cerr << std::endl;

    std::cerr << "-- mpz multiplication" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = alea.partial<int64_t>(31) * alea.pm();
        const int64_t j   = alea.partial<int64_t>(31) * alea.pm();
        const int64_t p   = i*j;
        const mpz I   = i;   Y_ASSERT(I.lsi()==i);
        const mpz J   = j;   Y_ASSERT(J.lsi()==j);
        const mpz P   = I*J; Y_ASSERT(P.lsi()==p);
    }
    std::cerr << std::endl;

    std::cerr << "-- mpz division" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = (1+alea.partial<int64_t>(61)) * alea.pm();
        const int64_t j   = (1+alea.partial<int64_t>(32)) * alea.pm();
        const int64_t q   = i/j;
        const mpz I   = i;   Y_ASSERT(I.lsi()==i);
        const mpz J   = j;   Y_ASSERT(J.lsi()==j);
        const mpz Q   = I/J;
        //std::cerr << "i=" << i << ", j=" << j << ", q=" << q << std::endl;
        //std::cerr << "I=" << I << ", J=" << J << ", Q=" << Q << std::endl;
        Y_ASSERT(Q.lsi()==q);
    }
    std::cerr << std::endl;

    std::cerr << "-- mpz square roots" << std::endl;
    std::cerr << "bits:";
    for(size_t iter=0;iter<100;++iter)
    {
        const mpz z( 10 + alea.leq(80), alea);
        const mpz z2 = mpz::square_of(z);
        const mpz s  = mkl::sqrt_of(z2);
        std::cerr << z2.n.bits() << "/";
        //std::cerr << z << "->" << z2 << "->" << s << std::endl;
        Y_ASSERT(s.n==z.n);
    }
    std::cerr << std::endl;



}
Y_UTEST_DONE()
