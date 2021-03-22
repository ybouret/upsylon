
#include "y/gfx/filter.hpp"
#include "y/gfx/image/io.hpp"
#include "y/gfx/ops/extrema.hpp"

#include "y/concurrent/loop/solo.hpp"
#include "y/concurrent/loop/simt.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

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






    template <typename T>
    static inline void do_filter()
    {
        crux::filter::patch<T> p3y( &simple[0][0], coord(-1,-1),coord(1,1 ));
        p3y.display(std::cerr << "p3y:" << std::endl);
        
        return ;
        {
            std::cerr << "<simple>" << std::endl;
            filter<T> F3y( "simple.y", &simple[0][0],3,false);
            filter<T> F3x( "simple.x", &simple[0][0],3,true);

            std::cerr << "F3y:" << std::endl << F3y << std::endl;

            std::cerr << "F3x:" << std::endl << F3x << std::endl;
        }

        if(true)
        {
            std::cerr << "<dummy>" << std::endl;
            filter<T> F3y( "dummy.y", &dummy[0][0],3,false);
            filter<T> F3x( "dummy.x", &dummy[0][0],3,true);

            std::cerr << "F3y:" << std::endl << F3y << std::endl;

            std::cerr << "F3x:" << std::endl << F3x << std::endl;
        }
    }

    static inline
    void apply_filter(const filter<float> &f,
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
    Y_UTEST_SIZEOF( filter<float>   );
    Y_UTEST_SIZEOF( filter<int>   );

    if(argc>1)
    {
        const string      filename = argv[1];
        image::io        &IMG      = Y_IMAGE();
        const pixmap<rgb> img      = IMG.load<rgb>(filename);
        broker            seq(seqEngine,img);
        broker            par(parEngine,img);
        pixmap<float>     source(img,par,convert<float,rgb>::from );

        IMG.save(source, "source.png" );

        filter<float> f3y( "simple-y", &simple[0][0],3,false );
        filter<float> f3x( "simple-x", &simple[0][0],3,true  );

        filter<float> d3y( "dummy-y", &dummy[0][0],3,false);
        filter<float> d3x( "dummy-x", &dummy[0][0],3,true);

        pixmap<float> target(source.w,source.h);

        apply_filter(f3y, target, source, par, IMG);
        apply_filter(f3x, target, source, par, IMG);
        apply_filter(d3y, target, source, par, IMG);
        apply_filter(d3x, target, source, par, IMG);

    }

}
Y_UTEST_DONE()

