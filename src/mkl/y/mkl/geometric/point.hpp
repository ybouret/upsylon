//! \file
#ifndef Y_GEOMETRIC_POINT_INCLUDED
#define Y_GEOMETRIC_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/block/zset.hpp"
#include "y/container/key-address.hpp"
#include "y/ptr/intr.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{

    namespace Geometric
    {

        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef counted_object Object;    //!< base class for dynamic objects
        typedef key_address<1> PointKey;  //!< address to key

        //______________________________________________________________________
        //
        //
        //! unique point that can be shared between multiple nodes
        //
        //______________________________________________________________________
        template <typename T, template <class> class VTX>
        class Point : public Object
        {
        public:
            //__________________________________________________________________
            //
            // types and defintions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                                                       //!< aliases
            typedef VTX<T>                   vertex;                                   //!< vertex type
            static const size_t              Dimensions = sizeof(vertex)/sizeof(type); //!< [2|3]
            typedef intr_ptr<PointKey,Point> Pointer;                                  //!< alias

            vertex              position; //!< physical position
            const PointKey      uuid;     //!< unique value

            //__________________________________________________________________
            //
            //C++
            //__________________________________________________________________

            //! setup [0...]
            inline explicit Point() throw() : position(), uuid( *this ) {}

            //! cleanup
            inline virtual ~Point() throw() { _bzset(position); }

            //! setup from vertex
            inline explicit Point(const vertex p) throw() : position(p), uuid( *this ) {}

            //! key for database
            inline const PointKey & key() const throw() { return uuid; }

            //! helper to display
            static inline ios::ostream & Print( ios::ostream &os, const vertex &v )
            {
                const_type *p = (const_type *)&v;
                os("%.15g",p[0]); for(size_t dim=1;dim<Dimensions;++dim) os(" %1.5g",p[dim]);
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Point);
        };

    }

}


#endif

