
#include "y/mpl/prime/factors-ratio.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/io.hpp"

using namespace  upsylon;

Y_UTEST(mp_io)
{
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
            {
                const string s = string_io::load_binary(fp);
                if( s != mpn::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpn::CLASS_NAME );
                }
                const mpn nn = mpn::read(fp);
                Y_CHECK(nn==n);
            }

            {
                const string s = string_io::load_binary(fp);
                if( s != mpz::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpz::CLASS_NAME );
                }
                const mpz zz = mpz::read(fp);
                Y_CHECK(zz==z);
            }


            {
                const string s = string_io::load_binary(fp);
                if( s != mpq::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpq::CLASS_NAME );
                }
                const mpq qq = mpq::read(fp);
                Y_CHECK(qq==q);
            }

            {
                const string s = string_io::load_binary(fp);
                if( s != mpF::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpF::CLASS_NAME );
                }
                const mpF FF = mpF::read(fp);
                Y_CHECK(FF==F);
            }

            {
                const string s = string_io::load_binary(fp);
                if( s != mpQ::CLASS_NAME )
                {
                    throw exception("Couldn't read %s", mpQ::CLASS_NAME );
                }
                const mpQ QQ = mpQ::read(fp);
                std::cerr << "QQ=" << QQ << std::endl;
                Y_CHECK(QQ==Q);
            }

        }
    }

}
Y_UTEST_DONE()

