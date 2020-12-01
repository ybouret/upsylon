//! \file
#ifndef Y_GEOMETRIC_POINT_INCLUDED
#define Y_GEOMETRIC_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/block/zset.hpp"
#include "y/container/key-address.hpp"
#include "y/ptr/intr.hpp"
#include "y/ios/ostream.hpp"
#include "y/string.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef counted_object Object;    //!< base class for dynamic objects
            typedef key_address<1> PointKey;  //!< address to key

            //__________________________________________________________________
            //
            //
            //! unique point that can be shared between multiple nodes
            //
            //__________________________________________________________________
            template <typename T, template <class> class VTX>
            class Point : public Object
            {
            public:
                //______________________________________________________________
                //
                // types and defintions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                                                       //!< aliases
                typedef VTX<T>                   vertex;                                   //!< vertex type
                static const size_t              Dimensions = sizeof(vertex)/sizeof(type); //!< [2|3]
                typedef intr_ptr<PointKey,Point> Pointer;                                  //!< alias

                vertex              site; //!< physical position
                const PointKey      uuid; //!< unique value

                //______________________________________________________________
                //
                //C++
                //______________________________________________________________

                //! setup [0...]
                inline explicit Point() throw() : site(), uuid( *this ) {}

                //! cleanup
                inline virtual ~Point() throw() { _bzset(site); }

                //! setup from vertex
                inline explicit Point(const vertex p) throw() : site(p), uuid( *this ) {}

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! access
                inline vertex       & operator*()       throw() { return site; }

                //! const access
                inline const vertex & operator*() const throw() { return site; }


                //! key for database
                inline const PointKey & key() const throw() { return uuid; }

                //! helper to display
                template <typename OSTREAM> static inline
                OSTREAM & Print(OSTREAM &os, const vertex &v)
                {
                    const_type *p = (const_type *)&v;
                    os << '[';
                    Format(os,p[0]);
                    for(size_t dim=1;dim<Dimensions;++dim)
                    {
                        os << ';'; Format(os,p[dim]);
                    }
                    os << ']';
                    return os;
                }

                inline friend std::ostream & operator<< (std::ostream &os, const Point &p)
                {
                    return Print(os,*p);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
                template <typename OSTREAM> static inline
                void Format(OSTREAM &os, const_type c)
                {
                    static const char fmt[] = "%.15g";
                    const  string     str   = vformat(fmt,double(c));
                    os << str;
                }
            };

        }

    }
}


#endif

