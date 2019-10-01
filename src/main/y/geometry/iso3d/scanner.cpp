#include "y/geometry/iso3d/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {
            

            void Scanner:: scanCube() const
            {

                static const size_t edges[12][2] =
                {
                    {0,1},
                    {0,3},
                    {1,2},
                    {2,3},

                    {4,5},
                    {4,7},
                    {5,6},
                    {6,7},

                    {0,4},
                    {1,5},
                    {2,6},
                    {3,7}

                };


                assert(c);
                assert(v);
                assert(d);
                assert(s);

                Surface_ &S = *s;

                static const unsigned negHead = 0x01;
                static const unsigned zzzHead = 0x02;
                static const unsigned posHead = 0x04;

                static const unsigned negTail = 0x08;
                static const unsigned zzzTail = 0x10;
                static const unsigned posTail = 0x20;


                for(size_t e=0;e<12;++e)
                {
                    const size_t iHead = edges[e][0];
                    const size_t iTail = edges[e][1];



                    const double      dHead = d[iHead];
                    const double      dTail = d[iTail];

                    const Contour::SignType sHead = Contour::SignOf(dHead);
                    const Contour::SignType sTail = Contour::SignOf(dTail);

                    unsigned flag = 0;
                    switch( sHead )
                    {
                        case Contour::IsNegative: flag |= negHead; break;
                        case Contour::IsZero:     flag |= zzzHead; break;
                        case Contour::IsPositive: flag |= posHead; break;

                    }

                    switch( sTail )
                    {
                        case Contour::IsNegative: flag |= negTail; break;
                        case Contour::IsZero:     flag |= zzzTail; break;
                        case Contour::IsPositive: flag |= posTail; break;
                    }

                    switch(flag)
                    {
                        case negHead | negTail:
                        case posHead | posTail:
                            // do nothing
                            break;

                        case negHead | posTail:
                        case posHead | negTail:
                            // one interception
                            (void) S.couple(c[iHead], v[iHead], dHead,
                                            c[iTail], v[iTail], dTail);

                            break;

                        case zzzHead | posTail:
                        case zzzHead | negTail:
                            // head is single point
                            (void) S.single(c[iHead],v[iHead]);
                            break;

                        case posHead | zzzTail:
                        case negTail | zzzTail:
                            // tail is single point
                            (void) S.single(c[iTail],v[iTail]);
                            break;

                        case zzzHead | zzzTail:
                            // both points are created
                            (void) S.single(c[iHead],v[iHead]);
                            (void) S.single(c[iTail],v[iTail]);
                            break;

                        default:
                            throw exception("unexpected edge flags!!!!");
                    }





                }



            }

        }

    }
}
