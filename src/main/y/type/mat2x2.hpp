//! \file

#ifndef Y_MAT2X2_INCLUDED
#define Y_MAT2X2_INCLUDED 1

#include "y/type/point2d.hpp"

namespace upsylon
{

    namespace core
    {

    }
    //__________________________________________________________________________
    //
    //
    //! 2D matrices ops
    //
    //__________________________________________________________________________
    template <typename T>
    class mat2x2
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);               //!< alias
        typedef point2d<T>       vertex;   //!< alias
        static const size_t      rows=2;   //!< alias
        static const size_t      cols=2;   //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup as zero
        inline  mat2x2() : ex(), ey() {}

        //! cleanup
        inline ~mat2x2() throw() {}

        //! copy
        inline  mat2x2(const mat2x2 &m) : ex(m.ex), ey(m.ey) {}

        //! assign
        inline  mat2x2 & operator=( const mat2x2 &m)
        {
            ex = m.ex;
            ey = m.ey;
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! output
        inline friend std::ostream & operator<<(std::ostream &os, const mat2x2 &m)
        {
            os << '[';
            os << m.ex.x << ' ' << m.ey.x;
            os << ';';
            os << m.ex.y << ' ' << m.ey.y;
            os << ']';
            return os;
        }


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        vertex ex; //!< column vector
        vertex ey; //!< column vector
    };

}

#endif
