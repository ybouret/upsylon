#include "y/utest/run.hpp"
#include "y/mpl/rational.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/adjoint.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
#include "y/core/ipower.hpp"
#include "y/math/kernel/quark.hpp"
#include "y/mpl/mpn.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(polint)
{
    MPN &mp = MPN::instance();
    mp.createPrimes(100);
    std::cerr << "mp.probe=" << mp.probe << std::endl;
    if(argc>1)
    {
        vector<mpq>  x;
        vector<mpq>  y;
        {
            const string fileName = argv[1];
            {
                ios::icstream  fp(fileName);
                string         line;
                vector<string> words;
                while(fp.gets(line))
                {
                    tokenizer<char>::split_with(words, line, " \t");
                    if(words.size()>=2)
                    {
                        const mpq X = string_convert::to<unit_t>( words[1], "x" );
                        const mpq Y = string_convert::to<unit_t>( words[2], "y" );
                        x.push_back(X);
                        y.push_back(Y);
                    }
                }
            }
        }
        std::cerr << "x=" << x << std::endl;
        std::cerr << "y=" << y << std::endl;
        const size_t n = x.size();
        matrix<mpq> m(n,n);
        matrix<mpq> am(n,n);
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                m[i][j] = ipower<mpq>(x[i], j-1);
            }
        }
        //std::cerr << "m=" << m << std::endl;
        const mpq dm = determinant(m);
        //std::cerr << "dm=" << dm << std::endl;
        adjoint(am,m);
        //std::cerr << "am=" << am << std::endl;
        vector<mpq> coef(n,0);
        quark::mul(coef,am,y);
        //std::cerr << "coef=" << coef << std::endl;

        vector<mpz> u(n+1,0);
        for(size_t i=n;i>0;--i)
        {
            u[i] = coef[i].num;
        }
        u[n+1] = dm.num;
        //std::cerr << "u=" << u << " = ";
        const mpn g = MPN::instance().simplify(u);
        //std::cerr << g << "*" << u << std::endl;
        const mpz den = u.back();
        u.pop_back();
        std::cerr << "u=" << u << "/" << den << std::endl;
        for(size_t i=1;i<=n;++i)
        {
            
        }
        
    }
    
    
}
Y_UTEST_DONE()

