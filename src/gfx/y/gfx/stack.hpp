
//! \file

#ifndef Y_GFX_STACK_INCLUDED
#define Y_GFX_STACK_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/image/format/tiff.hpp"
#include "y/sequence/slots.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace upsylon
{
    namespace graphic
    {
        
        namespace crux
        {
            //! common stuff for stacks
            struct stack
            {
                static void   check_tiff_handles(const string &filename);                         //!< check extension
                static void   check_same_metrics(const area &, I_TIFF &, const string &filename); //!< WxH compatible
                static size_t min_directories(const size_t nmax, const string &filename);         //!< check
            };
        }
        
        //______________________________________________________________________
        //
        //
        //! stack of pixmaps of same sizes
        //
        //______________________________________________________________________
        template <typename T>
        class stack : public area, public slots< pixmap<T>, memory::dyadic >
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef slots< pixmap<T>, memory::dyadic > slots_type; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //!setup
            inline explicit stack(const size_t n, const unit_t W, const unit_t H) :
            area(W,H),
            slots_type(n)
            {
                setup();
            }
            
            //! cleanup
            inline virtual ~stack() throw()
            {
            }
            
            //! load file
            explicit stack(const string  &filename,
                           rgba_to_type   *conv = 0,
                           const size_t    nmax = 0) :
            area( I_TIFF::WidthOf(filename), I_TIFF::HeightOf(filename)),
            slots_type( crux::stack::min_directories(nmax,filename) )
            {
                setup();
                const size_t loaded = load_tiff(filename,conv);
                (void)loaded;
                assert(loaded==this->size());
            }
            
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! load tiff(s)
            inline size_t load_tiff(const string  &filename,
                                    rgba_to_type  *conv    = 0)
            {
                crux::stack::check_tiff_handles(filename);
                put_rgba<T>   proc;
                if(!conv)     conv     = &proc;
                const size_t  num_dirs = I_TIFF::CountDirectoriesOf(filename);
                const size_t  max_dirs = min_of(num_dirs,this->size());
                
                _TIFF::Raster raster;
                I_TIFF        tiff(filename);
                crux::stack::check_same_metrics(*this,tiff,filename);
                for(size_t i=0;i<max_dirs;++i)
                {
                    tiff.SetDirectory(i);
                    tiff.ReadRBGAImage(raster);
                    tiff_format::expand((*this)[i], raster, *conv);
                }
                
                return max_dirs;
            }
            
            //! load tiff(s) wrapper
            inline size_t load_tiff(const char   *filename,
                                    rgba_to_type *conv = 0)
            {
                const string _(filename); return load_tiff(_,conv);
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
                crux::stack::check_tiff_handles(filename);
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
            
            inline void setup()
            {
                for(size_t i=0;i<this->count;++i)
                {
                    this->template build<size_t,size_t>(w,h);
                }
            }
        };
    }
    
}

#endif
