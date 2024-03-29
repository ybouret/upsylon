//! \file

#ifndef Y_GFX_IMAGE_FORMAT_INCLUDED
#define Y_GFX_IMAGE_FORMAT_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/color/type-to-rgba.hpp"
#include "y/gfx/color/rgba-to-type.hpp"
#include "y/fs/vfs.hpp"
#include "y/gfx/image/format/options.hpp"

namespace upsylon
{
    namespace graphic
    {
        
        namespace image
        {
            
            //__________________________________________________________________
            //
            //! format object: match/load/save
            //__________________________________________________________________
            class format : public entity
            {
            public:
                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual     ~format() throw();                     //!< cleanup
                virtual bool matches(const string &ext) const = 0; //!< exact extension math
                
            protected:
                //! load a new bitmap
                virtual bitmap load_(const string       &file,
                                     const options      *opts,
                                     const rgba_to_type &conv) const = 0;
                
                //! save a bitmap
                virtual void   save_(const bitmap       &bmp,
                                     const string       &file,
                                     const options      *opts,
                                     const type_to_rgba &conv) const = 0;
                
            public:
                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                
                //! handles file if matches extension
                template <typename FILENAME>
                bool handles(const FILENAME &filename) const
                {
                    const string ext = vfs::get_extension(filename);
                    return matches(ext);
                }
                
                //! load a pixmap
                template <typename T> inline
                pixmap<T> load(const string       &file,
                               const options      *opts=NULL,
                               const rgba_to_type *conv=NULL) const
                {
                    put_rgba<T> proc;
                    if(!conv)   conv = &proc;
                    pixmap<T>   pxm  = load_(file,opts,*conv);
                    return pxm;
                }
                
                
                //! save a pixmap
                template <typename T> inline
                void save(const pixmap<T>    &pxm,
                          const string       &file)
                {
                    get_rgba<T> proc;
                    save_(pxm,file,NULL,proc);
                }
                
                //! save a pixmap
                template <typename T> inline
                void save(const pixmap<T>    &pxm,
                          const string       &file,
                          const options      *opts)
                {
                    get_rgba<T> proc;
                    save_(pxm,file,opts,proc);
                }
                
                //! save a pixmap
                template <typename T> inline
                void save(const pixmap<T>    &pxm,
                          const string       &file,
                          const options      *opts,
                          const type_to_rgba &conv) const
                {
                    if(conv.depth()!=sizeof(T)) crux::pixmap::invalid_depth(conv.depth(), typeid(T) );
                    save_(pxm,file,opts,conv);
                }
                
                
                
            protected:
                explicit format() throw(); //!< setup
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(format);
            };
            
        }
        
    }
    
}

#endif
