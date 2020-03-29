
#include "y/mpl/prime/factors-ratio.hpp"
#include "y/mpl/mpn.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string.hpp"

using namespace  upsylon;

Y_UTEST(mp_io)
{

    MPN &mp = MPN::instance();
    mp.createPrimes(100);

    for(size_t iter=1+alea.leq(100);iter>0;--iter)
    {
        const mpn n(100,alea);
        const mpz z(100,alea);
        const mpq q(z,1+n);
        const size_t num = alea.leq(10000);
        std::cerr << "decomposing " << num << std::endl;
        const mpF F = num;
        std::cerr << "F=" << F << std::endl;
        const size_t den = 1+alea.leq(10000);
        std::cerr << "decomposing " << den << std::endl;
        const mpF D = den;
        const mpQ Q(F,D);
        std::cerr << "Q=" << Q << std::endl;

        size_t       out = 0;
        {
            ios::ocstream fp("mp-io.dat");
            out += n.serialize_className(fp);
            out += n.serialize(fp);

            out += z.serialize_className(fp);
            out += z.serialize(fp);

            out += q.serialize_className(fp);
            out += q.serialize(fp);

            out += F.serialize_className(fp);
            out += F.serialize(fp);

            out += Q.serialize_className(fp);
            out += Q.serialize(fp);
        }


        {
            ios::icstream fp("mp-io.dat");
            size_t shift = 0;
            {
                const string s = string::read(fp,shift,"mpn className");
                if( s != mpn::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpn::CLASS_NAME );
                }
                const mpn nn = mpn::read(fp,shift,"mpn");
                Y_CHECK(nn==n);
            }

            {
                const string s = string::read(fp,shift,"mpz className");
                if( s != mpz::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpz::CLASS_NAME );
                }
                const mpz zz = mpz::read(fp,shift,"mpz");
                Y_CHECK(zz==z);
            }


            {
                const string s = string::read(fp,shift,"mpq className");
                if( s != mpq::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpq::CLASS_NAME );
                }
                const mpq qq = mpq::read(fp,shift,"mpq");
                Y_CHECK(qq==q);
            }

            {
                const string s = string::read(fp,shift,"mpF className");
                if( s != mpF::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpF::CLASS_NAME );
                }
                const mpF FF = mpF::read(fp,shift,"mpF");
                Y_CHECK(FF==F);
            }

            {
                const string s = string::read(fp,shift,"mpQ className");
                if( s != mpQ::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpQ::CLASS_NAME );
                }
                const mpQ QQ = mpQ::read(fp,shift,"QQ");
                std::cerr << "QQ=" << QQ << std::endl;
                Y_CHECK(QQ==Q);
            }

        }
    }

}
Y_UTEST_DONE()

