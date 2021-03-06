
//! \file

#ifndef Y_GFX_STACK_INCLUDED
#define Y_GFX_STACK_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/image/format/tiff.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon
{
    namespace graphic
    {
        
        namespace crux
        {
            struct stack
            {
                static void check_tiff(const string &filename);
            };
        }
        //______________________________________________________________________
        //
        //
        //! stack of pixmaps of same sizes
        //
        //______________________________________________________________________
        template <typename T>
        class stack : public area, public slots< pixmap<T> >
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef slots< pixmap<T> > slots_type;
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //!setup
            inline explicit stack(const size_t n, const unit_t W, const unit_t H) :
            area(W,H),
            slots_type(n)
            {
                for(size_t i=0;i<n;++i)
                {
                    this->template build<size_t,size_t>(W,H);
                }
            }
            
            //! cleanup
            inline virtual ~stack() throw()
            {
            }            
            
            
            
            //! save [ini..end-1] as a multiple tiff
            inline void save_tiff(const string &filename,
                                  const size_t  ini,
                                  const size_t  end,
                                  type_to_rgba *conv = 0) const
            {
                //--------------------------------------------------------------
                // sanity check
                //--------------------------------------------------------------
                crux::stack::check_tiff(filename);
                get_rgba<T>   proc;
                if(!conv)     conv = &proc;
                
                //--------------------------------------------------------------
                // prepare to write
                //--------------------------------------------------------------
                _TIFF::Raster raster;
                bool          append = false;
                raster.startup(w*h);
                for(size_t i=ini,j=0;i<end;++i,++j)
                {
                    O_TIFF img(filename,append);
                    tiff_format::compile(raster, (*this)[i],*conv);
                    img.WriteRGBAImage(raster, int(w), int(h), int(j) );
                    append=true;
                }
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(stack);
        };
    }
    
}

#endif
