//! \file
#ifndef Y_FLTK_CURVE_INCLUDED
#define Y_FLTK_CURVE_INCLUDED 1

#include "y/string.hpp"
#include "y/type/point2d.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/counted.hpp"

#include "y/sequence/vector.hpp"
#include "y/associative/set.hpp"

#include <FL/fl_draw.H>

namespace upsylon
{
    namespace FLTK 
    {
        
        typedef point2d<double>   Point;   //!< alias
        typedef vector<Point>     Points;  //!< alias

        //! named points+color
        class Curve : public Points
        {
        public:
            explicit Curve(const string &id);
            explicit Curve(const char   *id);
            virtual ~Curve() throw();
            
            const string name;
            Fl_Color     color;
            
            const string &key() const throw();
            typedef intr_ptr<string,Curve>      Pointer;
            typedef set<string,Curve::Pointer>  Set;

            Point getMin() const;
            Point getMax() const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Curve);
        };
        
        //! a set of named curves
        class Curves : public Curve::Set
        {
        public:
            explicit Curves() throw(); //!< setup
            virtual ~Curves() throw(); //!< desctructor
            
            Curve &       operator[]( const string &id );       //!< existing or create
            const Curve & operator[]( const string &id ) const; //!< must exist
            
            Curve &       operator[]( const char * );       //!< alias: get existing or create
            const Curve & operator[]( const char *) const;  //!< alias: must exist

            Point getMin() const;
            Point getMax() const;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Curves);
        };
        
    }
}

#endif
