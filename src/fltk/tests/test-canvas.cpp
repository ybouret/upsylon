#include "y/utest/run.hpp"
#include "canvas.h"
#include "y/ptr/auto.hpp"
#include "y/math/types.hpp"

using namespace upsylon;
using namespace FLTK;

Y_UTEST(canvas)
{
    auto_ptr<Fl_Window> win( MakeCanvasWindow() );
    
    Curve &crv = Ca->curves["data"];
    
    Ca->xaxis.set_range(-1, 1);
    Ca->yaxis.set_range(-1, 1);
    
    for( size_t i=0; i <= 100; ++i )
    {
        const double angle = (20.0*i)/100;
        const double radius = 0.1 + 0.1 * angle;
        crv.push_back( Point(radius*sin(angle),radius*cos(angle) ) );
    }
    
    win->show(argc,argv);

    (void)Fl::run();
}
Y_UTEST_DONE()

