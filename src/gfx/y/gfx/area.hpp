//! \file

#ifndef Y_GFX_AREA_INCLUDED
#define Y_GFX_AREA_INCLUDED 1

#include "y/gfx/types.hpp"


namespace upsylon
{
    namespace randomized
    {
        class bits;
    }

    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! basic area
        //
        //______________________________________________________________________
        class Area
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            Area(const unit_t X, const unit_t Y, unit_t W, unit_t H);
            //! copy
            Area(const Area &) throw();
            //! cleanup
            virtual ~Area() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            friend std::ostream & operator<<(std::ostream &os, const Area &);

            Area getCore()        const;          //!< core   content
            Area getLeft()        const;          //!< left   content
            Area getRight()       const;          //!< right  content
            Area getTop()         const;          //!< top    content
            Area getBottom()      const;          //!< bottom content
            Area getBottomLeft()  const;          //!< bottom|left
            Area getBottomRight() const;          //!< bottom|right
            Area getTopLeft()     const;          //!< top|left
            Area getTopRight()    const;          //!< top|right
            Area get(const unsigned flags) const; //!< uses flag

            bool hasCore() const throw(); //!< w>2 and h>2
            

            //! used named method
            template <int IDX> inline
            Area get(const int2type<IDX> &)
            {
                return get( unsigned(IDX) );
            }

            //! get a sub area
            Area rand(randomized::bits &) const;

            
            bool owns(const unit_t X,  const unit_t Y) const throw(); //!< ownership
            bool owns(const Point  p)  const throw();                 //!< ownership
            bool owns(const Area &sub) const throw();                 //!< ownership

            bool matches(const Area &rhs) const throw(); //!< width and height
            bool equals(const Area &rhs)  const throw(); //!< width, height and position

            friend bool operator==(const Area &lhs, const Area &rhs) throw(); //!< equality
            friend bool operator!=(const Area &lhs, const Area &rhs) throw(); //!< difference


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t x;  //!< bottom-left  position
            const unit_t y;  //!< bottom-right position
            const unit_t w;  //!< width  >= 0
            const unit_t h;  //!< height >= 0
            const unit_t xm; //!< x+w-1
            const unit_t ym; //!< y+h-1
            const unit_t n;  //!< width*height >= 0




        private:
            Y_DISABLE_ASSIGN(Area);
        };
    }

}

#endif
