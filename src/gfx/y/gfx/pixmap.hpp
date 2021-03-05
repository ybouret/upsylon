
//! \file

#ifndef Y_GFX_PIXMAP_INCLUDED
#define Y_GFX_PIXMAP_INCLUDED 1

#include "y/gfx/bitmap.hpp"
#include "y/gfx/pixrow.hpp"
#include <typeinfo>

namespace upsylon
{
    namespace graphic
    {

        namespace crux
        {
            //! common stuff for pixmaps
            struct pixmap
            {
                //! throw exception
                static void invalid_depth(const unit_t depth, const std::type_info &);
                
            };
        }

        //______________________________________________________________________
        //
        //! generic pixmap, using bitmap as memory layout
        //______________________________________________________________________
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);     //!< aliase
            typedef pixrow<T> row;   //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit pixmap(const unit_t W, const unit_t H) :
            bitmap(W,H,sizeof(T)), rows( mine() )
            {
            }

            //! cleanup
            inline virtual ~pixmap() throw() { rows = 0; }

            //! shared copy
            inline pixmap(const pixmap &pxm) throw() :
            bitmap(pxm), rows( mine() )
            {
                
            }

            //! from bitmap
            inline pixmap(const bitmap &bmp):
            bitmap(bmp), rows( mine() )
            {
                if( depth != sizeof(T) ) crux::pixmap::invalid_depth(depth, typeid(T) );
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! raw access
            inline row & operator()(const unit_t j) throw()
            {
                assert(j>=0);assert(j<h);
                return rows[j];
            }

            //! raw access, const
            inline const row & operator()(const unit_t j) const throw()
            {
                assert(j>=0);assert(j<h);
                return rows[j];
            }

            //! zero-flux access
            inline row & operator[](const unit_t j) throw()
            {
                return rows[ zfh(j) ];
            }

            //! zero-flux access, const
            inline const row & operator[](const unit_t j) const throw()
            {
                return rows[ zfh(j) ];
            }


        private:
            row *rows;

            Y_DISABLE_ASSIGN(pixmap);

            inline row * mine() throw() { return static_cast<row *>(a_rows->impl.block_addr); }

        };
    }

}

#endif

