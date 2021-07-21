//! \file

#ifndef Y_COLOR_RGB_INCLUDED
#define Y_COLOR_RGB_INCLUDED 1

#include "y/sort/network/sort3.hpp"
#include "y/comparison.hpp"
#include <iostream>

namespace upsylon
{
    namespace graphic
    {

        namespace crux
        {

            template <typename T> class rgba; //!< forward declaration

            //! RGB type
            template <typename T>
            class rgb
            {
            public:
                T r; //!< read
                T g; //!< green
                T b; //!< blue

                //! default setup
                inline  rgb( T R=0, T G=0, T B=0) throw() : r(R), g(G), b(B) {}
                //! cleanup
                inline ~rgb() throw() {}
                //! copy
                inline  rgb(const rgb &_) throw() : r(_.r), g(_.g), b(_.b) {}
                //! assign
                inline  rgb & operator=(const rgb &_) throw()
                {
                    r=_.r;
                    g=_.g;
                    b=_.b;
                    return *this;
                }
                //! assign RGB part of RGBA
                rgb( const rgba<T> &) throw();

                //! 0..2 access
                inline T & operator[](const size_t i) throw()
                {
                    assert(i<3);
                    return *((&r)+i);
                }

                //! 0..2 const access
                inline const T & operator[]( const size_t i) const throw()
                {
                    assert(i<3);
                    return *((&r)+i);
                }

                //! display
                friend inline std::ostream & operator<<(std::ostream &os, const rgb &c)
                {
                    os << '(' << double(c.r) << ',' << double(c.g) << ',' << double(c.b) << ')';
                    return os;
                }

                //! integral comparison
                friend inline bool operator==(const rgb &lhs, const rgb &rhs) throw()
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b);
                }

                //! integral comparison
                friend inline bool operator!=(const rgb &lhs, const rgb &rhs) throw()
                {
                    return (lhs.r!=rhs.r) || (lhs.g!=rhs.g) || (lhs.b!=rhs.b);
                }

                //! lexicographic comparison
                static inline int compare(const rgb &lhs, const rgb &rhs) throw()
                {
                    return comparison::increasing_lexicographic<T>(&lhs.r, &rhs.r,3);
                }
                
                //! less operator
                friend inline bool operator<(const rgb &lhs, const rgb &rhs) throw()
                {
                    return compare(lhs,rhs) < 0;
                }

                //! computed a saturated rgb
                inline rgb saturated() const throw()
                {
                    int indx[3] = {0,1,2};
                    int chan[3] = {r,g,b};
                    network_sort<3>::co(chan,indx,compare_decreasing_chan);
                    assert(chan[0]>=chan[1]);
                    assert(chan[1]>=chan[2]);

                    const unsigned m = chan[0];
                    if(m>0)
                    {
                        chan[0]   = 0xff;
                        (chan[1] *= 0xff) /= m;
                        (chan[2] *= 0xff) /= m;
                        network_sort<3>::co(indx,chan,compare_increasing_indx);
                        return rgb(static_cast<uint8_t>(chan[0]),
                                   static_cast<uint8_t>(chan[1]),
                                   static_cast<uint8_t>(chan[2]));
                    }
                    else
                    {
                        return rgb(0,0,0);
                    }
                }

                //! to sort channels
                static inline int compare_decreasing_chan(const int &lhs, const int &rhs) throw()
                {
                    return rhs-lhs;
                }

                //! to sort indices
                static inline int compare_increasing_indx(const int &lhs, const int &rhs) throw()
                {
                    return lhs-rhs;
                }
            };

            //! RGBA
            template <typename T>
            class rgba
            {
            public:
                static const T Opaque; //!< type dependent opaque value
                T r; //!< read
                T g; //!< green
                T b; //!< blue
                T a; //!< alpha

                //! setup
                inline  rgba( T R=0, T G=0, T B=0, T A=Opaque) throw() : r(R), g(G), b(B), a(A) {}
                //! cleanup
                inline ~rgba() throw() {}
                //! copy
                inline  rgba(const rgba &_) throw() : r(_.r), g(_.g), b(_.b), a(_.a) {}
                //! assign
                inline  rgba & operator=(const rgba &_) throw()
                {
                    r=_.r;
                    g=_.g;
                    b=_.b;
                    a=_.a;
                    return *this;
                }

                //! opaque RGB
                inline rgba( const rgb<T> &C, T A=Opaque) throw() : r(C.r), g(C.g), b(C.b), a(A) {}


                //! 0..3 access
                inline T & operator[](const size_t i) throw()
                {
                    assert(i<4);
                    return *((&r)+i);
                }

                //! 0..3 const access
                inline const T & operator[]( const size_t i) const throw()
                {
                    assert(i<4);
                    return *((&r)+i);
                }

                //! display
                friend inline std::ostream & operator<<(std::ostream &os, const rgba &c)
                {
                    os << '(' << double(c.r) << ',' << double(c.g) << ',' << double(c.b) << ',' << double(c.a) << ')';
                    return os;
                }


                //! integral comparison
                friend inline bool operator==(const rgba &lhs, const rgba &rhs) throw()
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b) && (lhs.a==rhs.a);
                }

                //! integral partial comparison
                friend inline bool operator==(const rgb<T> &lhs, const rgba &rhs) throw()
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b);
                }

                //! integral partial comparison
                friend inline bool operator==(const rgba &lhs, const rgb<T> &rhs) throw()
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b);
                }

            };

            //! RGBA->RGB implementation
            template <typename T>
            inline rgb<T>::rgb( const rgba<T> &C ) throw() : r(C.r), g(C.g), b(C.b) {}

        }

        typedef crux::rgb<uint8_t>  rgb;   //!< alias
        typedef crux::rgba<uint8_t> rgba;  //!< alias
        typedef crux::rgb<float>    rgbF;  //!< alias
        typedef crux::rgba<float>   rgbaF; //!< alias

    }


}


#endif

