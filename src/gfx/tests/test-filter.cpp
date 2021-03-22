
#include "y/gfx/filter.hpp"
#include "y/gfx/image/io.hpp"
#include "y/gfx/ops/extrema.hpp"

#include "y/concurrent/loop/solo.hpp"
#include "y/concurrent/loop/simt.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace graphic;

namespace {

    static const short simple[3][3] =
    {
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };

    

    static const int dummy[3][3] =
    {
        {0,-1,0},
        {0,0,0},
        {1,2,3}
    };


    static const short rect[3][5] =
    {
        {1,2,3,4,5},
        {6,7,8,9,10},
        {11,12,13,14,15}
    };




    template <typename T>
    static inline void do_filter()
    {
        std::cerr << "do_filter<" << type_name_of<T>() << ">" << std::endl;
        {
            crux::filter::patch<T> p3y( &rect[0][0], coord(-2,-1), coord(2,1) );
            p3y.display(std::cerr << "p3y:" << std::endl);

            crux::filter::patch<T> p3x( &rect[0][0], coord(-2,-1), coord(2,1), area::transpose);
            p3x.display(std::cerr << "p3x:" << std::endl);

            T rms=0;
            for(unit_t y=p3y.lower.y;y<=p3y.upper.y;++y)
            {
                for(unit_t x=p3y.lower.x;x<=p3y.upper.y;++x)
                {
                    rms += square_of( p3y[y][x] - p3x[x][y] );
                }
            }
            Y_CHECK(rms<=0);

        }

        std::cerr << std::endl;

        {
            std::cerr << "<simple>" << std::endl;
            filter<T,int> F3y( "simple", &simple[0][0],3,3,".y");
            filter<T,int> F3x( "simple", &simple[0][0],3,3,area::transpose,".x");

            std::cerr << "F3y:" << std::endl << F3y << std::endl;
            std::cerr << "F3x:" << std::endl << F3x << std::endl;
        }

         if(true)
        {
            std::cerr << "<dummy>" << std::endl;
            filter<T,long> F3y( "dummy.y", &dummy[0][0],3,3);
            filter<T,long> F3x( "dummy.x", &dummy[0][0],3,3,area::transpose);

            std::cerr << "F3y:" << std::endl << F3y << std::endl;

            std::cerr << "F3x:" << std::endl << F3x << std::endl;
        }
    }

    template <typename Z>
    static inline
    void apply_filter(const filter<float,Z> &f,
                      pixmap<float>       &target,
                      const pixmap<float> &source,
                      broker              &apply,
                      image::io           &IMG)
    {
        f(target,source,apply);
        float vmin=0, vmax=0;
        {
            const float *arr = extrema::minmax(target,apply);
            vmin = arr[0];
            vmax = arr[1];
        }
        extrema::normalize(target,target,apply,vmin,vmax);

        IMG.save(target,f.name+".png");
    }

}

Y_UTEST(filter)
{
    engine seqEngine = new concurrent::solo();
    engine parEngine = new concurrent::simt();

    do_filter<float>();
    do_filter<int>();

    Y_UTEST_SIZEOF( crux::filter::weights<int>   );
    Y_UTEST_SIZEOF( crux::filter::weights<float> );


    if(argc>1)
    {
        const string      filename = argv[1];
        image::io        &IMG      = Y_IMAGE();
        const pixmap<rgb> img      = IMG.load<rgb>(filename);
        broker            seq(seqEngine,img);
        broker            par(parEngine,img);
        pixmap<float>     source(img,par,convert<float,rgb>::from );

        IMG.save(source, "source.png" );

        filter<float,short> f3y( "simple-y", &simple[0][0],3,false );
        filter<float,short> f3x( "simple-x", &simple[0][0],3,true  );

        filter<float,int> d3y( "dummy-y", &dummy[0][0],3,false);
        filter<float,int> d3x( "dummy-x", &dummy[0][0],3,true);

        pixmap<float> target(source.w,source.h);

        apply_filter(f3y, target, source, par, IMG);
        apply_filter(f3x, target, source, par, IMG);
        apply_filter(d3y, target, source, par, IMG);
        apply_filter(d3x, target, source, par, IMG);

    }

}
Y_UTEST_DONE()

