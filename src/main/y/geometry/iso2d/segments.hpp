//! \file
#ifndef Y_GEOM_ISO2D_SEGMENTS_INCLUDED
#define Y_GEOM_ISO2D_SEGMENTS_INCLUDED 1

#include "y/geometry/iso2d/lines.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            //__________________________________________________________________
            //
            //! a segment as a node for a list
            //__________________________________________________________________
            class Segment : public object
            {
            public:
                const Point head; //!< head point
                const Point tail; //!< tail point
                Segment    *next; //!< for list
                Segment    *prev; //!< for list

                typedef core::list_of_cpp<Segment> List; //!< base class for segments

                explicit Segment(const Point &h, const Point &t) throw(); //!< setup
                virtual ~Segment() throw();                               //!< cleanup
                Segment(const Segment &) throw();                         //!< copy

                static bool AreTheSame(const Segment &, const Segment &) throw(); //!< test
                static bool AreOpposite(const Segment &, const Segment &) throw(); //!< test
                friend bool operator==(const Segment &, const Segment &) throw();   //!< are_the_same or are_opposite
                friend bool operator!=(const Segment &, const Segment &) throw();   //!< test if different
                
            private:
                Y_DISABLE_ASSIGN(Segment);
            };


            //__________________________________________________________________
            //
            //! a list of segments
            //__________________________________________________________________
            class Segments : public Segment::List
            {
            public:
                explicit Segments() throw();     //!< setup
                virtual ~Segments() throw();     //!< cleanup
                Segments(const Segments &other); //!< copy

                //! compile to lines, optionaly rebuild segments
                void buildLines( Lines &lines, const bool keep );
                
            private:
                Y_DISABLE_ASSIGN(Segments);
                static bool grow( Lines &, const Segment *s );
            };


        }

    }
}

#endif


