#include "y/type/bzset.hpp"
#include "y/os/rt-clock.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{

    template <size_t N> static inline
    void _bzset(const double D)
    {
        std::cerr << std::endl;
        std::cerr << "BZSET<" << N << ">" << std::endl;
        rt_clock clk;
        char     blk[128];
        uint64_t sum = 0;
        size_t   cycles = 0;

        do
        {
            ++cycles;
            uint64_t mark = rt_clock::ticks();
            core::bzset<N>(blk);
            sum += rt_clock::ticks() - mark;
        } while( clk(sum) <= D );

        memset(blk,1,N);
        core::bzset<N>(blk);
        for(size_t i=0;i<N;++i)
        {
            std::cerr << "  "; Y_CHECK(0==blk[i]);
        }
        const double speed = (cycles/clk(sum))/1e6;
        std::cerr << "speed" << N << "\t= " << speed << std::endl;
        FILE *fp = fopen("bzset.dat","ab");
        if(fp)
        {
            fprintf(fp, "%u %.15g\n", unsigned(N), speed);
            fclose(fp);
        }
    }


    static inline void do_test(const double D)
    {
        FILE *fp = fopen("bzset.dat","wb"); if(fp) fclose(fp);
        _bzset<1>(D);  _bzset<2>(D);  _bzset<3>(D);  _bzset<4>(D);  _bzset<5>(D);  _bzset<6>(D);  _bzset<7>(D);  _bzset<8>(D);  _bzset<9>(D);  _bzset<10>(D);
        _bzset<11>(D); _bzset<12>(D); _bzset<13>(D); _bzset<14>(D); _bzset<15>(D); _bzset<16>(D); _bzset<17>(D); _bzset<18>(D); _bzset<19>(D); _bzset<20>(D);
        _bzset<21>(D); _bzset<22>(D); _bzset<23>(D); _bzset<24>(D); _bzset<25>(D); _bzset<26>(D); _bzset<27>(D); _bzset<28>(D); _bzset<29>(D); _bzset<30>(D);
        _bzset<31>(D); _bzset<32>(D); _bzset<33>(D); _bzset<34>(D); 

    }
}

Y_UTEST(zset)
{
    double D = 0.05;
    if(argc>1) D=atof(argv[1]);
    do_test(D);
}
Y_UTEST_DONE()

