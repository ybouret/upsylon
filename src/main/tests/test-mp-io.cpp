
#include "y/mpl/rational.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/io.hpp"

using namespace  upsylon;

Y_UTEST(mp_io)
{
    for(size_t iter=1+alea.leq(100);iter>0;--iter)
    {
        mpn n(100,alea);
        mpz z(100,alea);
        mpq q(z,1+n);

        size_t       out = 0;
        {
            ios::ocstream fp("mp-io.dat");
            out += n.serialize_className(fp);
            out += n.serialize(fp);

            out += z.serialize_className(fp);
            out += z.serialize(fp);

            out += q.serialize_className(fp);
            out += q.serialize(fp);
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
        }
    }

}
Y_UTEST_DONE()

