//! \file
#ifndef Y_OXIDE_CONNECTIVITY_INCLUDED
#define Y_OXIDE_CONNECTIVITY_INCLUDED 1

#include "y/oxide/types.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! handle connectivity levels
        struct Conn
        {
            //! number of variying coords
            enum Level
            {
                Level1, //!< along a main axis
                Level2, //!< along a short diagonal
                Level3  //!< along a long diaogonal
            };

            //! get textual representation
            static const char *Level2Text( const Level ) throw();

            //! depends on the sign of the first coordinate of the displacement
            enum Course
            {
                Forward, //!< forward course, first  coordinate is  1
                Reverse  //!< reverse course, second coordinate is -1
            };

            static Course Opposite( const Course ) throw();

            //! get integer representation
            static  Coord1D     CourseSign( const Course ) throw();

            //! get textual representation
            static  const char *Course2Text( const Course ) throw();

            //! store the direction, orientation and connectivity level
            template <typename COORD>
            class Link
            {
            public:
                static const size_t Dimensions = Coord::Get<COORD>::Dimensions; //!< workspace dimension

                const COORD     direction;     //!< regularized direction
                const COORD     orientation;   //!< same for +/- direction
                const Course    course;        //!< direction = course * orientation
                const Level     level;         //!< number of active coordinates
                
                //! setup from a vector
                inline explicit Link(const COORD delta) :
                direction( delta ),
                orientation( 0 ),
                course(Forward),
                level( MakeLink( (Coord1D *)&direction, (Coord1D *)&orientation, (Course *)&course, Dimensions) )
                {
                }

                //! copy
                inline Link(const Link &link) throw() :
                direction(   link.direction   ),
                orientation( link.orientation ),
                course(      link.course      ),
                level(       link.level       )
                {
                    
                }

                //! cleanup
                inline ~Link() throw() {}

                //! test paired links
                static bool ArePaired( const Link &lhs, const Link &rhs ) throw()
                {
                    return (lhs.level==rhs.level) && (rhs.orientation==lhs.orientation) && (lhs.way!=rhs.way) && (lhs.direction==-rhs.direction);
                }

                inline const char *levelText()  const throw() { return Level2Text(level);   } //!< texual represntation
                inline const char *courseText() const throw() { return Course2Text(course); } //!< textual representation

                //! print info
                friend inline std::ostream & operator<<( std::ostream &os, const Link &link )
                {
                    os << "<<" << link.levelText() << '.' << link.courseText() << '*';
                    Coord::Disp(os,link.orientation,2) << '=';
                    Coord::Disp(os,link.direction,2) << ">>";
                    return os;
                }

            private:
                Y_DISABLE_ASSIGN(Link);
            };


            //! make a link from direction and orientation
            /**
             - direction[0..dims-1] must have not zero components, set to +/- 1
             - orientation is set accordingly, with first coordinate positive
             - return a link level according to the D.O.F
             */
            static Level MakeLink(Coord1D       *direction,
                                  Coord1D       *orientation,
                                  Course        *cs,
                                  const unsigned dims);


        };

    }

}

#endif


