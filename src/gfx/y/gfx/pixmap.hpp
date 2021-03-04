
//! \file

#ifndef Y_GFX_PIXMAP_INCLUDED
#define Y_GFX_PIXMAP_INCLUDED 1

#include "y/gfx/bitmap.hpp"
#include "y/gfx/pixrow.hpp"

namespace upsylon
{
    namespace graphic
    {

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
            bitmap(W,H,sizeof(T)),
            rows(0)
            {
                setup();
            }

            //! cleanup
            inline virtual ~pixmap() throw()
            {
                rows = 0;
            }

            //! shared copy
            inline pixmap(const pixmap &other) throw() :
            bitmap(other),
            rows(0)
            {
                setup();
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
            inline void setup() throw() { rows = static_cast<row *>(a_rows->impl.block_addr); }


        };
    }

}

#endif

