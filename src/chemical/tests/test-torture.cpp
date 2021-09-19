

#include "y/chemical/system.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/mkl/tao.hpp"
#include "y/counting/comb.hpp"
#include "y/yap/gcd.hpp"


using namespace upsylon;
using namespace Chemical;
using namespace mkl;

Y_UTEST(torture)
{
    size_t N = 3;
    size_t M = 4;

    for(int i=1;i<argc;++i)
    {
        string arg = argv[i];
        char  *sep = strchr(*arg,'=');
        if(sep)
        {
            *sep = 0;
            string label = *arg;
            string value = sep+1;
            label.clean_with(" \t");
            value.clean_with(" \t");

            std::cerr << "label=<" << label << ">" << std::endl;
            std::cerr << "value=<" << value << ">" << std::endl;

            if("N"==label)
            {
                N = string_convert::to<size_t>(value,*label);
                continue;
            }

            if("M"==label)
            {
                M = string_convert::to<size_t>(value,*label);
                continue;
            }

            throw exception("unknow label '%s'", *label);
        }
    }

    if(M<N) throw exception("M<N");

    for(size_t iter=0;iter<4;++iter)
    {
        Library        lib;
        Lua::VM        lvm = new Lua::State();
        Equilibria     eqs;

        iMatrix Nu(N,M);
        std::cerr << "Building Nu:";
        do
        {
            (std::cerr << ".").flush();
            for(size_t i=1;i<=N;++i)
            {
                for(size_t j=1;j<=M;++j)
                {
                    Nu[i][j] = alea.range<unit_t>(-4,4);
                }
            }
        } while( apk::rank(Nu) < N);
        std::cerr << std::endl;
        std::cerr << "Nu=" << Nu << std::endl;

        if(M>N)
        {
            const size_t Nc = M-N;
            iMatrix      Om(Nc,M);
            apk::complete_ortho(Nu,Om);
            std::cerr  << "Om=" << Om << std::endl;
        }

       // continue;



        std::cerr << "Building Library" << std::endl;
        Strings names(M,as_capacity);
        for(size_t j=0;j<M;++j)
        {
            const string ID = 'A' + j;
            lib << ID;
            names << ID;
        }



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
        
        std::cerr << "lib=" << lib << std::endl;
        std::cerr << "eqs=" << eqs << std::endl;

        std::cerr << "Building System" << std::endl;
        System sys(lib,eqs,Equilibrium::Default);

        break;
    }

}
Y_UTEST_DONE()
