#include "y/geometry/iso2d/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            namespace {
                static const unsigned n0 = 0x0001; // 1
                static const unsigned z0 = 0x0002; // 2
                static const unsigned p0 = 0x0004; // 4

                static const unsigned n1 = 0x0008; // 8
                static const unsigned z1 = 0x0010; // 16
                static const unsigned p1 = 0x0020; // 32

                static const unsigned n2 = 0x0040; // 64
                static const unsigned z2 = 0x0080; // 128
                static const unsigned p2 = 0x0100; // 256
            }


#define Y_CONTOUR2D_FLAG(I) do { \
switch(Contour::SignOf(d##I))\
{\
case Contour::IsNegative:  flags |= n##I; break;\
case Contour::IsZero:      flags |= z##I; break;\
case Contour::IsPositive:  flags |= p##I; break;\
} } while(false)

            void Scanner:: scanTriangles() const
            {
                static const unsigned i0       = 0;
                static const unsigned ii[4][2] = { {1,2}, {2,3}, {3,4}, {4,1} };

                //------------------------------------------------------------------
                //
                // Loop over the four triangles
                //
                //------------------------------------------------------------------
                assert(c);
                assert(d);
                assert(v);
                assert(l);
                Level_ &L = *l;
                for(size_t t=0;t<4;++t)
                {
                    const unsigned i1 = ii[t][0];
                    const unsigned i2 = ii[t][1];

                    const double   d0 = d[i0];
                    const double   d1 = d[i1];
                    const double   d2 = d[i2];



                    const Coordinate &c0 = c[i0];
                    const Coordinate &c1 = c[i1];
                    const Coordinate &c2 = c[i2];

                    const Vertex     &v0 = v[i0];
                    const Vertex     &v1 = v[i1];
                    const Vertex     &v2 = v[i2];


                    unsigned flags = 0;
                    Y_CONTOUR2D_FLAG(0);
                    Y_CONTOUR2D_FLAG(1);
                    Y_CONTOUR2D_FLAG(2);

                    switch(flags)
                    {
                            //------------------------------------------------------
                            //
                            // almost do nothing cases 0+3=3/27
                            //
                            //------------------------------------------------------
                        case z0|z1|z2:  (void) L.single(c0,v0);  (void) L.single(c1,v1);  (void) L.single(c2,v2);
                        case n0|n1|n2:
                        case p0|p1|p2:
                            break;

                            //------------------------------------------------------
                            //
                            // create single point, no segment cases
                            // 3+6=9/27
                            //
                            //------------------------------------------------------
                        case n0|n1|z2:
                        case p0|p1|z2:
                            (void) L.single(c2,v2);
                            break;

                        case n0|z1|n2:
                        case p0|z1|p2:
                            (void) L.single(c1,v1);
                            break;

                        case z0|n1|n2:
                        case z0|p1|p2:
                            (void) L.single(c0,v0);
                            break;


                            //------------------------------------------------------
                            //
                            // create segment=full edge
                            // 9+6=15/27
                            //
                            //------------------------------------------------------
                        case z0|z1|n2:
                        case z0|z1|p2:
                            L.full(c0,v0,c1,v1);
                            break;

                        case z0|n1|z2:
                        case z0|p1|z2:
                            L.full(c0,v0,c2,v2);
                            break;

                        case n0|z1|z2:
                        case p0|z1|z2:
                            L.full(c1,v1,c2,v2);
                            break;


                            //------------------------------------------------------
                            //
                            // create segment=point+intersection
                            // 15+6=21/27
                            //
                            //------------------------------------------------------
                        case z0|p1|n2:
                        case z0|n1|p2:
                            L.inter1(c0,v0, c1,v1,d1, c2,v2,d2);
                            break;

                        case n0|z1|p2:
                        case p0|z1|n2:
                            L.inter1(c1,v1, c0,v0,d0, c2,v2,d2);
                            break;

                        case n0|p1|z2:
                        case p0|n1|z2:
                            L.inter1(c2,v2, c0,v0,d0, c1,v1,d1);
                            break;

                            //------------------------------------------------------
                            //
                            // create segment=intersection+intersection
                            // 21+6=27/27
                            //
                            //------------------------------------------------------
                        case n0|p1|p2:
                        case p0|n1|n2:
                            L.inter2(c0,v0,d0, c1,v1,d1, c2,v2,d2);
                            break;

                        case n0|p1|n2:
                        case p0|n1|p2:
                            L.inter2(c1,v1,d1, c0,v0,d0, c2,v2,d2);
                            break;

                        case n0|n1|p2:
                        case p0|p1|n2:
                            L.inter2(c2,v2,d2, c0,v0,d0, c1,v1,d1);
                            break;


                        default:
                            throw exception("Iso2d::Scanner(unexpected case)");

                    }

                }
            }



        }

    }

}


