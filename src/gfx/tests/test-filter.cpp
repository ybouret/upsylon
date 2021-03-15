
#include "y/gfx/filter.hpp"
#include "y/utest/run.hpp"

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
            filter<T> F3y( &simple[0][0],3,false);
            filter<T> F3x( &simple[0][0],3,true);

            std::cerr << "F3y:" << std::endl << F3y << std::endl;

            std::cerr << "F3x:" << std::endl << F3x << std::endl;
        }

        if(true)
        {
            std::cerr << "<dummy>" << std::endl;
            filter<T> F3y( &dummy[0][0],3,false);
            filter<T> F3x( &dummy[0][0],3,true);

            std::cerr << "F3y:" << std::endl << F3y << std::endl;

            std::cerr << "F3x:" << std::endl << F3x << std::endl;
        }
    }


}

Y_UTEST(filter)
{
    do_filter<float>();

}
Y_UTEST_DONE()

