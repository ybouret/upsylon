
//! \file

#ifndef Y_MAT3X3_INCLUDED
#define Y_MAT3X3_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! 3D matrices ops
    //
    //__________________________________________________________________________
    template <typename T>
    class mat3x3
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);        //!< alias
        typedef point3d<T>  vertex; //!< alias
        static const size_t rows=3; //!< alias
        static const size_t cols=3; //!< alias

        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        vertex ex; //!< column vector
        vertex ey; //!< column vector
        vertex ez; //!< column vector
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup as zero
        inline  mat3x3() : ex(), ey(), ez() {}

        //! cleanup
        inline ~mat3x3() throw() {}

        //! copy
        inline  mat3x3(const mat3x3 &m) : ex(m.ex), ey(m.ey), ez(m.ez) {}

        //! assign
        inline  mat3x3 & operator=( const mat3x3 &m)
        {
            ex = m.ex;
            ey = m.ey;
            ez = m.ez;
            return *this;
        }


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! output
        inline friend std::ostream & operator<<(std::ostream &os, const mat3x3 &m)
        {
            os << '[';
            os << m.ex.x << ' ' << m.ey.x << ' ' << m.ez.x;
            os << ';';
            os << m.ex.y << ' ' << m.ey.y << ' ' << m.ez.y;
            os << ';';
            os << m.ex.z << ' ' << m.ey.z << ' ' << m.ez.z;
            os << ']';
            return os;
        }



    };

}

#endif
