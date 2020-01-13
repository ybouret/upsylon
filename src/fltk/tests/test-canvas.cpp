#include "y/utest/run.hpp"
#include "canvas.h"
#include "y/ptr/auto.hpp"
#include "y/math/types.hpp"

using namespace upsylon;
using namespace FLTK;


Y_UTEST(canvas)
{
    auto_ptr<Fl_Window> win( MakeCanvasWindow() );

    {
        Curve &crv = Ca->curves["data"];

        Ca->xaxis.set_range(-1, 1);
        Ca->yaxis.set_range(-1, 1);

        for( size_t i=0; i <= 100; ++i )
        {
            const double angle = (20.0*i)/100;
            const double radius = 0.1 + 0.1 * angle;
            crv.push_back( Point(radius*sin(angle),radius*cos(angle) ) );
        }
    }

    {
        Curve &crv = Ca->curves["data2"];

        crv.color      = FL_RED;
        crv.style      = Curve::WithPoints;
        crv.pointSize  = 8;
        crv.pointShape = Curve::Circle;
        crv.pointStyle = Curve::Filled;

        Ca->xaxis.set_range(-1, 1);
        Ca->yaxis.set_range(-1, 1);

        for( size_t i=0; i <= 100; ++i )
        {
            const double angle  = (20.0*i)/100;
            const double radius = 0.08 + 0.1 * angle;
            crv.push_back( Point(radius*sin(angle),radius*cos(angle) ) );
        }
    }

    {
        Curve &crv = Ca->curves["data3"];

        crv.color      = FL_GREEN;
        crv.style      = Curve::WithPoints;
        crv.pointSize  = 12;
        crv.pointShape = Curve::Circle;
        crv.pointStyle = Curve::Border;

        Ca->xaxis.set_range(-1, 1);
        Ca->yaxis.set_range(-1, 1);

        for( size_t i=0; i <= 100; ++i )
        {
            const double angle  = (20.0*i)/100;
            const double radius = 0.05 + 0.12 * angle;
            crv.push_back( Point(radius*sin(angle),radius*cos(angle) ) );
        }
    }

    {
        Curve &crv = Ca->curves["data4"];

        crv.color      = FL_MAGENTA;
        crv.style      = Curve::WithPoints;
        crv.pointSize  = 4;
        crv.pointShape = Curve::Square;
        crv.pointStyle = Curve::Filled;

        Ca->xaxis.set_range(-1, 1);
        Ca->yaxis.set_range(-1, 1);

        for( size_t i=0; i <= 100; ++i )
        {
            const double angle  = (20.0*i)/100;
            const double radius = 0.05 + 0.12 * angle;
            crv.push_back( Point(radius*sin(-angle),radius*cos(-angle) ) );
        }
    }

    {
        Curve &crv = Ca->curves["data5"];

        crv.color      = FL_YELLOW;
        crv.style      = Curve::WithPoints;
        crv.pointSize  = 5;
        crv.pointShape = Curve::Square;
        crv.pointStyle = Curve::Border;

        Ca->xaxis.set_range(-1, 1);
        Ca->yaxis.set_range(-1, 1);

        for( size_t i=0; i <= 100; ++i )
        {
            const double angle  = (20.0*i)/100;
            const double radius = 0.05 + 0.12 * angle;
            crv.push_back( Point(radius*sin(-angle),radius*cos(-angle) ) );
        }
    }



    win->show(argc,argv);

    (void)Fl::run();
}
Y_UTEST_DONE()

