#include "y/yap/gcd.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

Y_UTEST(yap_gcd)
{
    const int tab[] = { 4, 8, 32, 1024 };

    list<apn>    ln;
    vector<apn>  vn;
    list<apz>    lz;
    vector<apz>  vz;

    unsigned i=0;
    do
    {
        std::cerr << ln << vn << lz << vz << std::endl;


        const apn gln = yap::compute_gcd::of(ln);
        const apn gvn = yap::compute_gcd::of(vn);
        
        const apz glz = yap::compute_gcd::of(lz);
        const apz gvz = yap::compute_gcd::of(vz);

        std::cerr << gln << " " << gvn << " " << glz << " " << gvz << std::endl;

        const int t = tab[i];
        ln << t;
        vn << t;
        {
            lz << (alea.choice()?t:-t);
            vz << (alea.choice()?t:-t);
        }

    } while(++i<=sizeof(tab)/sizeof(tab[0]));



}
Y_UTEST_DONE()

