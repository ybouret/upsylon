
//! \file
#ifndef Y_EUCLIDEAN_TYPES_INCLUDED
#define Y_EUCLIDEAN_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {


    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //! helper to find the right point[2|3]d<T>
            //
            //==================================================================
            template <typename T,size_t> struct __Core;

            //==================================================================
            //
            //! map to point2d<T>
            //
            //==================================================================
            template <typename T> struct __Core<T,2>
            {
                typedef point2d<T> Type; //!< alias

                //! output helper
                static inline ios::ostream &Print(ios::ostream &fp, const Type &v)
                {
                    fp("%.15g %.15g", v.x, v.y);
                    return fp;
                }
            };

            //==================================================================
            //
            //! map to point3d<T>
            //
            //==================================================================
            template <typename T> struct __Core<T,3>
            {
                typedef point3d<T> Type; //!< alias

                //! output helper
                static inline ios::ostream &Print(ios::ostream &fp, const Type &v)
                {
                    fp("%.15g %.15g %.15g", v.x, v.y, v.z);
                    return fp;
                }
            };

            //==================================================================
            //
            //! base class for object
            //
            //==================================================================
            typedef counted_object Object;
            
        }

    }

}

#endif

