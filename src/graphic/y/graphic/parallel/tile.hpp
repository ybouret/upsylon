
//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILE_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILE_INCLUDED 1

#include "y/graphic/area.hpp"

namespace upsylon {

    namespace Graphic {

        class Tiles; // forward declaration

        //! subdivision for operation on bitmaps/pixmaps, may be empty
        class Tile : public Area
        {
        public:

            const Point lower; //!< lower index
            const Point upper; //!< upper index

            //! setup with possible zero Area
            explicit  Tile(const Point &lo, const Point &up);

            //! cleanup
            virtual  ~Tile() throw();

            //! display
            friend std::ostream & operator<<( std::ostream &, const Tile &);



            //! alias internal data
            template <typename T> inline
            T &as() throw()
            {
                assert(size>=sizeof(T));
                return *static_cast<T*>(data);
            }

            //! alias internal data
            template <typename T> inline
            const T &as() const throw()
            {
                assert(size>=sizeof(T));
                return *static_cast<T*>(data);
            }

            //! alias internal data
            template <typename T> inline
            T &as( const size_t indx )   throw()
            {
                assert(size>=(1+indx)*sizeof(T));
                return *(static_cast<T*>(data)+indx);
            }


            //! alias internal data
            template <typename T> inline
            const T &as( const size_t indx ) const throw()
            {
                assert(size>=(1+indx)*sizeof(T));
                return *(static_cast<T*>(data)+indx);
            }

            size_t localMemory() const throw(); //!< get allocated bytes


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tile);
            void        *data; //!< extra shared memory
            const size_t size; //!< available size

            friend class Tiles;
        };


    }

}

#endif

