#include "y/fltk/canvas.hpp"

namespace upsylon
{
    namespace FLTK
    {
        Canvas:: ~Canvas() throw()
        {
            
        }
        
        Canvas:: Canvas( int X, int Y, int W, int H, const char *l ) :
        Fl_Box(X,Y,W,H,l),
        xaxis(),
        yaxis(),
        y2axis(),
        curves(),
        curves2()
        {
        }
        
        
        void Canvas:: draw()
        {
            //------------------------------------------------------------------
            // draw the box
            //------------------------------------------------------------------
            Fl_Box::draw();
            
            //------------------------------------------------------------------
            // draw the curves
            //------------------------------------------------------------------
            __draw( *this, xaxis, yaxis,  curves);
            __draw( *this, xaxis, y2axis, curves2);
        }
        
        
        void Canvas:: __draw( const Fl_Box &box, const Axis &Xaxis, const Axis &Yaxis, const Curves &C )
        {
            //------------------------------------------------------------------
            // compute scaling factors
            //------------------------------------------------------------------
            const int W = box.w();
            const int H = box.h();
            const int X = box.x();
            const int Y = box.y();
            if( W <= 0 || H <= 0 ) return;
            const double xscale = (W-1)/Xaxis.length;
            const double yscale = (H-1)/Yaxis.length;
            const double ytop   = Y+H-1;
            
            fl_push_clip(X, Y, W, H);
            //------------------------------------------------------------------
            // iterate on curves
            //------------------------------------------------------------------
            for( Curves::const_iterator i = C.begin(); i != C.end(); ++i )
            {
                const Curve &crv = **i;
               // std::cerr << "Drawing [" << crv.name << "]: #" << crv.size() << std::endl;
                if(crv.size()<=0) continue;

                fl_color( crv.color );

                switch(crv.style)
                {
                    case Curve::WithLines:
                        for( size_t j=2; j <= crv.size(); ++j )
                        {
                            const Point &p0 = crv[j-1];
                            const Point &p1 = crv[j];

                            const double x0 = X+(p0.x - Xaxis.vmin) * xscale;
                            const double y0 = ytop-(p0.y - Yaxis.vmin) * yscale;

                            const double x1 = X+(p1.x - Xaxis.vmin) * xscale;
                            const double y1 = ytop-(p1.y - Yaxis.vmin) * yscale;
                            fl_line( int(x0), int(y0), int(x1), int(y1) );
                        }
                        break;

                    case Curve::WithPoints:
                        for(size_t j=1; j<=crv.size(); ++j)
                        {
                            const Point  p0 = crv[j];
                            const double x0 = X+(p0.x - Xaxis.vmin) * xscale;
                            const double y0 = ytop-(p0.y - Yaxis.vmin) * yscale;
                            const double r  = crv.pointSize;

                            if(r<=0)
                            {
                                fl_point(x0,y0);
                            }
                            else
                            {

                                switch(crv.pointShape)
                                {
                                    case Curve::Circle:
                                        switch( crv.pointStyle )
                                        {
                                            case Curve::Border:
                                                fl_arc(x0, y0, r, r, 0, 360);
                                                break;

                                            case Curve::Filled:
                                                fl_pie(x0, y0, r,r, 0, 360);
                                                break;
                                        }
                                        break;

                                    case Curve::Square: {
                                        const int sz = 2*r+1;
                                        switch( crv.pointStyle )
                                        {
                                            case Curve::Border:
                                                fl_rect(x0-r,y0-r, sz, sz);
                                                break;

                                            case Curve::Filled:
                                                fl_rectf(x0-r,y0-r, sz, sz);
                                                break;
                                        }
                                    } break;
                                }
                            }
                        }
                        break;

                    default:
                        break;
                }


            }
            fl_pop_clip();
        }
        
        
    }
}
