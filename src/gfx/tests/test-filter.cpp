
#include "y/gfx/filter.hpp"
#include "y/gfx/image/io.hpp"

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
        {0,1,0},
        {0,0,0},
        {1,2,3}
    };



    template <typename T>
    static inline void do_filter()
    {

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


}

Y_UTEST(filter)
{
    engine seqEngine = new concurrent::solo();
    engine parEngine = new concurrent::simt();

    do_filter<float>();
    do_filter<int>();

    Y_UTEST_SIZEOF( crux::filter_weights<int>   );
    Y_UTEST_SIZEOF( crux::filter_weights<float> );

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

        pixmap<float> target(source.w,source.h);

        f3x(target,source,seq);




    }

}
Y_UTEST_DONE()

