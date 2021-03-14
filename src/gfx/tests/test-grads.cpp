#include "y/yap.hpp"
#include "y/gfx/types.hpp"
#include "y/container/matrix.hpp"
#include "y/counting/mloop.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace yap;
using namespace graphic;

namespace
{
    typedef apq (*Lambda)(const accessible<coord> &coords, const size_t );
    
    apq toX2(const accessible<coord> &coords, const size_t k)
    {
        return square_of(coords[k].x);
    }
    
    apq toY2(const accessible<coord> &coords, const size_t k)
    {
        return square_of(coords[k].y);
    }
    
    apq toXY(const accessible<coord> &coords, const size_t k)
    {
        return  (coords[k].x*coords[k].y);
    }
    
    apq toX(const accessible<coord> &coords, const size_t k)
    {
        return  (coords[k].x);
    }
    
    apq toY(const accessible<coord> &coords, const size_t k)
    {
        return  (coords[k].y);
    }
    
    apq to1(const accessible<coord> &, const size_t)
    {
        return 1;
    }
    
    static const Lambda Lam[8] =
    {
        0,
        toX2,
        toY2,
        toXY,
        toX,
        toY,
        to1,
        0
    };
    
    
    static inline void compute(const accessible<coord> &coords,
                               const accessible<apq>   &weights)
    {
        assert(weights.size()==coords.size());
        
        const size_t N = coords.size();
        matrix<apq>  M(6,6);
        matrix<apq>  W(6,N);
        
        for(size_t i=1;i<=6;++i)
        {
            
            for(size_t j=1;j<=6;++j)
            {
                apq sum = 0;
                for(size_t k=1;k<=N;++k)
                {
                    sum += Lam[i](coords,k)*Lam[j](coords,k);
                }
                M[i][j] = sum;
            }
            
            for(size_t k=1;k<=N;++k)
            {
                W[i][k] = Lam[i](coords,k) * weights[k];
            }
        }
        std::cerr << "M=" << M << std::endl;
        std::cerr << "W=" << W << std::endl;
    }
}


Y_UTEST(grads)
{
    vector<coord> coords;
    vector<apq>   weights;
    
    {
        mloop<unit_t,coord> loop(coord(-1,-1),coord(1,1));
        std::cerr << "loop.count=" << loop.count << std::endl;
        
        coords.free();
        weights.free();
        
        for( loop.boot(); loop.good(); loop.next() )
        {
            coords.push_back( *loop );
            weights.push_back(1);
        }
        compute(coords,weights);
    }
    
}
Y_UTEST_DONE()

