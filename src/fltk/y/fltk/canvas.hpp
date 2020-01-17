#ifndef YOCTO_FLTK_CANVAS_INCLUDED
#define YOCTO_FLTK_CANVAS_INCLUDED 1

#include "y/fltk/axis.hpp"
#include <FL/Fl_Box.H>

namespace upsylon
{
    namespace FLTK
    {
        
        class Canvas : public Fl_Box
        {
        public:
            explicit Canvas( int X, int Y, int W, int H, const char *l = 0 );
            virtual ~Canvas() throw();
            
            Axis   xaxis;
            Axis   yaxis;
            Axis   y2axis;
            Axis   y3axis;
            Curves curves;
            Curves curves2;
            Curves curves3;
            
            virtual void draw();
            
        private:
            static 
            void __draw( const Fl_Box &box, const Axis &X, const Axis &Y, const Curves &C );
            Y_DISABLE_COPY_AND_ASSIGN(Canvas);
        };
        
    }
    
}

#endif
