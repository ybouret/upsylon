#include "y/fltk/curve.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace FLTK
    {
        
        Curve:: ~Curve() throw() {}

#define Y_FLTK_CURVE_INIT() \
color(FL_WHITE),            \
style( WithLines ),         \
pointSize(0),               \
pointShape( Circle ),       \
pointStyle( Filled )

        Curve:: Curve( const string &id ) :
        name(id),
        Y_FLTK_CURVE_INIT()
        {
        }
        
        Curve:: Curve( const char *id ) :
        name(id),
        Y_FLTK_CURVE_INIT()
        {
        }
        
        const string & Curve:: key() const throw() { return name; }


        Point Curve:: getMin() const
        {
            const size_t n = size();
            if(n<=0) throw exception("FLTK::Curve: no points to getMin");

            const array<Point> &self = *this;
            Point ans = self[1];
            for(size_t i=n;i>1;--i)
            {
                ans = Point::min_of(ans,self[i]);
            }
            return ans;

        }

        Point Curve:: getMax() const
        {
            const size_t n = size();
            if(n<=0) throw exception("FLTK::Curve: no points to getMax");

            const array<Point> &self = *this;
            Point ans = self[1];
            for(size_t i=n;i>1;--i)
            {
                ans = Point::max_of(ans,self[i]);
            }
            return ans;
        }


        
        Curves:: Curves() throw() : Curve::Set()
        {}
        
        
        Curves:: ~Curves() throw()
        {
        }

        Curve & Curves:: operator[]( const string &id )
        {
            Curve::Pointer *ppC = search(id);
            if( ppC )
            {
                return **ppC;
            }
            else
            {
                Curve::Pointer p( new Curve(id) );
                if( ! insert(p) )
                    throw exception("Curves.insert('%s') unexpected failure!!!", *id );
                return *p;
            }
        }

        Curve & Curves:: operator[]( const char *name )
        {
            const string id = name;
            return (*this)[id];
        }
        
        const Curve & Curves:: operator[]( const string &id ) const
        {
            const Curve::Pointer *ppC = search(id);
            if( !ppC )
            {
                throw exception("no Curves['%s']",*id );
            }
            return **ppC;
        }
        
        const Curve & Curves:: operator[]( const char *name ) const
        {
            const string id = name;
            return (*this)[id];
        }


        Point Curves:: getMin() const
        {
            if( size() <= 0 )
            {
                throw exception("FLTK::Curves empty for getMin()");
            }
            const_iterator i   = begin();
            Point          ans = (**i).getMin();
            while( ++i != end() )
            {
                ans = Point::min_of(ans,(**i).getMin());
            }
            return ans;
        }

        Point Curves:: getMax() const
        {
            if( size() <= 0 )
            {
                throw exception("FLTK::Curves empty for getMax()");
            }
            const_iterator i   = begin();
            Point          ans = (**i).getMin();
            while( ++i != end() )
            {
                ans = Point::max_of(ans,(**i).getMax());
            }
            return ans;
        }

        
    }
}

