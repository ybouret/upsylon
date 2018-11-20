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
        
        typedef point2d<double>   Point;
        typedef vector<Point>     Points;
        
        class Curve : public Points, public counted
        {
        public:
            explicit Curve(const string &id);
            explicit Curve(const char   *id);
            virtual ~Curve() throw();
            
            const string name;
            Fl_Color     color;
            
            const string &key() const throw();
            typedef intr_ptr<string,Curve>      Ptr;
            typedef set<string,Curve::Ptr>      DB;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Curve);
        };
        
        
        class Curves : public Curve::DB
        {
        public:
            explicit Curves() throw();
            virtual ~Curves() throw();
            
            Curve & operator[]( const string &id ); //!< existing or create
            const Curve & operator[]( const string &id ) const; //! must exist
            
            Curve & operator[]( const char * );
            const Curve & operator[]( const char *) const;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Curves);
        };
        
    }
}

#endif
