

#include "y/chemical/system.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/kernel/apk.hpp"

using namespace upsylon;
using namespace Chemical;
using namespace mkl;

Y_UTEST(torture)
{

    Library        lib;
    Lua::VM        lvm = new Lua::State;
    Equilibria     eqs;

    const size_t M = 4;
    const size_t N = 3;

    std::cerr << "Building Library" << std::endl;
    Strings names(M,as_capacity);
    for(size_t j=0;j<M;++j)
    {
        const string ID = 'A' + j;
        lib << ID;
        names << ID;
    }

    std::cerr << "lib=" << lib << std::endl;

    // choosing a matrix
    iMatrix Nu(N,M);
    std::cerr << "Building Nu:";
    do
    {
        (std::cerr << ".").flush();
        for(size_t i=1;i<=N;++i)
        {
            for(size_t j=1;j<=M;++j)
            {
                Nu[i][j] = alea.range<unit_t>(-5,5);
            }
        }
    } while( apk::rank(Nu) < N);
    std::cerr << std::endl;
    std::cerr << "Nu=" << Nu << std::endl;

    std::cerr << "Building Equilbria" << std::endl;
    for(size_t i=1;i<=N;++i)
    {
        const string ID = vformat("eq%u",unsigned(i));
        Equilibrium &eq = eqs.use( new ConstEquilibrium(ID,double(i)) );

        for(size_t j=1;j<=M;++j)
        {
            const unit_t nu = Nu[i][j];
            if(nu)
            {
                const Species &sp = lib[ names[j] ];
                eq(nu,sp);
            }
        }
    }

    std::cerr << "Building System" << std::endl;
    System sys(lib,eqs,Equilibrium::Default);


}
Y_UTEST_DONE()
