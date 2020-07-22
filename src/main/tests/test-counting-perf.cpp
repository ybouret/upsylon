
#include "y/counting/comb.hpp"
#include "y/counting/perm.hpp"
#include "y/utest/run.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/sequence/vector.hpp"
#include <iomanip>
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace {

    static inline double save_average(const char          *filename,
                                    const size_t         num,
                                    addressable<double> &spd )
    {
        const double ave = (sorted_sum(spd) / spd.size());
        ios::ocstream::echo(filename, "%u %g\n", unsigned(num), ave);
        return ave;
    }

    static inline
    void comb_perf(const size_t N)
    {
        vector<double> spd(N,as_capacity);
        std::cerr << "comb" << std::endl;

        const char filename[] ="comb-perf.dat";
        ios::ocstream::overwrite(filename);

        for(size_t n=1;n<=N;++n)
        {
            std::cerr << "\tn=" << std::setw(2) << n << std::endl;
            spd.free();
            for(size_t k=1;k<=n;++k)
            {
                combination comb(n,k);
                const double speed = comb.loops_per_second(1)/1e6;
                std::cerr << "\t\t" << std::setw(2) << k << "  : " << speed << std::endl;
                spd.push_back(speed);
            }
            const double ave = save_average(filename,n,spd);
            std::cerr << "\t\tave : " << ave << std::endl;
        }
        std::cerr << std::endl;
    }

    static inline
    void perm_perf(const size_t N)
    {
        vector<double> spd(N,as_capacity);
        std::cerr << "perm" << std::endl;

        const char filename[] ="perm-perf.dat";
        ios::ocstream::overwrite(filename);

        for(size_t n=1;n<=N;++n)
        {
            std::cerr << "\t" << std::setw(2) << n << "  : " << std::endl;
            spd.free();
            permutation perm(n);
            spd.push_back( perm.loops_per_second(1)/1e6 );
            const double ave = save_average(filename,n,spd);
            std::cerr << "\tave : " << ave << std::endl;
        }

        std::cerr << std::endl;
    }

}

Y_UTEST(counting_perf)
{
    const size_t   N=7;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string args = argv[iarg];
        if("comb"==args) comb_perf(N);
        if("perm"==args) perm_perf(N);
    }


}
Y_UTEST_DONE()
