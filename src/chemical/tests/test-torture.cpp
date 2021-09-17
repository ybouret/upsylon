

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
            matrix<apz>  Om(Nc,M);
            apk::complete_ortho(Nu,Om);
        }

        return 0;

        matrix<apz> Nu2(N,N);
        matrix<apz> NuT(M,N);
        NuT.assign_transpose(Nu);
        tao::gram(Nu2,Nu);
        std::cerr << "Nu2=" << Nu2 << std::endl;
        const apz dNu2 = apk::determinant(Nu2);
        std::cerr << "dNu2=" << dNu2 << std::endl;
        matrix<apz> aNu2(N,N);
        apk::adjoint(aNu2,Nu2);
        std::cerr << "aNu2=" << aNu2 << std::endl;

        matrix<apz> P(M,M);
        {
            matrix<apz> aNu3(N,M);
            tao::mmul(aNu3,aNu2,Nu);
            tao::mmul(P,NuT,aNu3);
        }
        apz dP = dNu2, dQ=dNu2;
        matrix<apz> Q(M,M);
        Q.diag(dNu2,0);
        tao::msub(Q,P);

        apk::simplify(P,dP,NULL);
        apk::simplify(Q,dQ,NULL);
        std::cerr << "P=" << P << "/" << dP << std::endl;
        std::cerr << "Q=" << Q << "/" << dQ << std::endl;

        {
            matrix<apz> PQ(M,M);
            tao::mmul_rtrn(PQ,P,Q);
            Y_ASSERT( tao::mmod2<apz>::of(PQ) == 0 );
        }



        if(M>N)
        {
            const size_t Nc=M-N;
            Y_ASSERT(apk::rank(Q)==Nc);
            // extracting
            matrix<apz> Omega(Nc,M);
            combination comb(M,Nc);

            bool        foundOmega = false;
            for(comb.boot();comb.good();comb.next())
            {
                //std::cerr << "Trying " << (counting &)comb << std::endl;
                for(size_t i=Nc;i>0;--i)
                {
                    const size_t ii = comb[i];
                    for(size_t j=M;j>0;--j)
                    {
                        Omega[i][j] = Q[j][ii];
                    }
                }
                if( apk::has_maximal_rank(Omega) )
                {
                    foundOmega = true;
                    break;
                }
            }
            Y_ASSERT(foundOmega);
            std::cerr << "Omega0=" << Omega << std::endl;

            for(size_t i=1;i<=Nc;++i)
            {
                yap::compute_gcd::simplify(Omega[i]);
            }
            std::cerr << "Omega=" << Omega << std::endl;

        }


        continue;

        std::cerr << "Building Library" << std::endl;
        Strings names(M,as_capacity);
        for(size_t j=0;j<M;++j)
        {
            const string ID = 'A' + j;
            lib << ID;
            names << ID;
        }

        std::cerr << "lib=" << lib << std::endl;


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

        std::cerr << "eqs=" << eqs << std::endl;

        std::cerr << "Building System" << std::endl;
        System sys(lib,eqs,Equilibrium::Default);

    }

}
Y_UTEST_DONE()
