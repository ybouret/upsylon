#include "y/utest/run.hpp"
#include "y/mpl/rational.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
#include "y/core/ipower.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(polint)
{
    
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
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                m[i][j] = ipower<mpq>(x[i], j-1);
            }
        }
        std::cerr << "m=" << m << std::endl;
        if(!LU::build(m))
        {
            throw exception("singular system");
        }
        vector<mpq> a = y;
        LU::solve(m,a);
        std::cerr << "a=" << a << std::endl;
        
    }
    
    
}
Y_UTEST_DONE()

