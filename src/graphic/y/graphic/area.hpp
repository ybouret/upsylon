//! \file

#ifndef Y_GRAPHIC_AREA_INCLUDED
#define Y_GRAPHIC_AREA_INCLUDED 1

#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {

        class Rectangle; //!< forward declaration

        //! basic area desing
        class Area : public Object
        {
        public:
            //! cleanup
            virtual ~Area() throw();
            //! construct
            explicit Area(const size_t W, const size_t H);
            //! no-throw copy
            Area(const Area &) throw();
            
            const unit_t w;       //!< internally signed
            const unit_t h;       //!< internally signed
            const size_t items;   //!< w*h

            //! check ownership
            bool contains(const unit_t i, const unit_t j) const throw();

            //! check ownership
            bool contains(const Point &p ) const throw();

            //! check inclusion
            bool contains(const Rectangle &r) const throw();

            //! display
            friend std::ostream & operator<<( std::ostream &, const Area & );

            bool sameAreaThan( const Area &area ) const throw();

        private:
            Y_DISABLE_ASSIGN(Area);
        };
    }

}

#endif

