#include "y/yap.hpp"
#include "y/gfx/types.hpp"
#include "y/container/matrix.hpp"
#include "y/counting/mloop.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "../../mkl/y/mkl/kernel/lu.hpp"

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
        return  coords[k].prod();
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
                               const accessible<apq>   &weights,
                               addressable<apq>        &gx,
                               addressable<apq>        &gy)
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
                    sum += Lam[i](coords,k)*Lam[j](coords,k) * weights[k];
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
        if(!mkl::LU::build(M))
        {
            throw exception("singular matrix");
        }
        mkl::LU::solve(M,W);
        for(size_t k=1;k<=N;++k)
        {
            gx[k] = W[4][k];
            gy[k] = W[5][k];
        }
        
        std::cerr << "gx=" << gx << std::endl;
        std::cerr << "gy=" << gy << std::endl;
    }
    
    typedef apq (*Weight)(const coord);
    
    static inline apq WeightOne(const coord) { return 1; }
    static inline apq WeightTwo(const coord) { return 2; }

    static inline apq WeightA(const coord p) {
        return apq(1,(1+p.norm1()));
    }

#if 0
    static inline void compute(const coord lower,
                               const coord upper,
                               Weight      wproc)
    {
        vector<coord> coords;
        vector<apq>   weights;
        vector<apq>   gx,gy;
        
        mloop<unit_t,coord> loop(lower,upper);
        for( loop.boot(); loop.good(); loop.next() )
        {
            coords.push_back( *loop );
            weights.push_back( wproc(*loop) );
            gx.push_back(0);
            gy.push_back(0);
        }
        compute(coords,weights,gx,gy);

    }
#endif
    

    static inline void compute(const unit_t delta,
                               Weight       wproc)
    {
        assert(delta>0);
        const unit_t  length = 1+delta*2;

        matrix<coord> m(length,length);
        matrix<apq>   w(length,length);
        matrix<apq>   gx(length,length);
        matrix<apq>   gy(length,length);

        const matrix<coord>::row coords = m.as_array();
        const matrix<apq>::row   weight = w.as_array();
        matrix<apq>::row         grad_x = gx.as_array();
        matrix<apq>::row         grad_y = gy.as_array();

        for(unit_t y=-delta;y<=delta;++y)
        {
            const unit_t j=y+delta+1;
            for(unit_t x=-delta;x<=delta;++x)
            {
                const unit_t i=x+delta+1;
                const coord  p(x,y);
                m[i][j] = p;
                w[i][j] = wproc(p);
            }
        }
        m.tableau( std::cerr << "coords:" << std::endl );
        w.tableau( std::cerr << "weight:" << std::endl );

        compute(coords,weight,grad_x,grad_y);

        gx.tableau( std::cerr << "grad_x:" << std::endl );
        gy.tableau( std::cerr << "grad_y:" << std::endl );

    }
    
}




Y_UTEST(grads)
{
#if 0
    compute( coord(-1,-1), coord(1,1), WeightOne );
    compute( coord(-1,-1), coord(1,1), WeightTwo );
    compute( coord(-1,-1), coord(1,1), WeightA   );
#endif

    compute(1,WeightOne);

}
Y_UTEST_DONE()

