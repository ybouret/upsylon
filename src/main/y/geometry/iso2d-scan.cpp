#include "y/geometry/iso2d.hpp"

namespace upsylon
{
    namespace geometry
    {
        void Iso2D:: scan_triangles(Levels             &levels,
                                    const double       *g,
                                    const Vertex       *vtx,
                                    const Coordinate   *coord,
                                    const array<double> &z)
        {
            static const size_t tri[4][2] =
            {
                {1,2},
                {2,3},
                {3,4},
                {4,1}
            };
            
            assert(g);
            assert(vtx);
            assert(coord);
            assert(levels.size()==z.size());
            const size_t nc = z.size();
            
            for(size_t k=nc;k>0;--k)
            {
                const double   zk        = z[k];
                const double   f[5]      = { 0, g[1]-zk, g[2]-zk, g[3]-zk, g[4]-zk };
                SharedPoints  &db       = *levels[k];
                Segments      &segments = db.segments;

                //--------------------------------------------------
                // loop over triangles
                //--------------------------------------------------
                static const size_t m0   = 0;
                const Vertex        p0   = vtx[m0];
                const double        f0   = 0.25*(f[1]+f[2]+f[3]+f[4]);
                const unsigned      s0   = (sign_flag(f0) << sign_shift0);
                const Coordinate    c0   = coord[m0];
                for(size_t l=0;l<4;++l)
                {
                    //----------------------------------------------
                    // get indices
                    //----------------------------------------------
                    const size_t        m1 = tri[l][0];
                    const size_t        m2 = tri[l][1];
                    const Coordinate    c1 = coord[m1];
                    const Coordinate    c2 = coord[m2];

                    //----------------------------------------------
                    // get real coordinates
                    //----------------------------------------------
                    const Vertex    p1    = vtx[m1];
                    const Vertex    p2    = vtx[m2];

                    const double   f1    = f[m1];
                    const double   f2    = f[m2];
                    const unsigned flags = s0 | (sign_flag(f1)<<sign_shift1) | (sign_flag(f2)<<sign_shift2);

                    //----------------------------------------------
                    //
                    // there are nine symetrical possibilities
                    //
                    //----------------------------------------------
                    switch(flags)
                    {
                            //--------------------------------------
                            //
                            // intercepting two edges
                            //
                            //--------------------------------------
                            // (p0/p1) - (p0/p2)
                        case neg0|pos1|pos2:
                        case pos0|neg1|neg2:
                            segments.push_back( new Segment(db(c0,p0,f0,c1,p1,f1),db(c0,p0,f0,c2,p2,f2)) );
                            break;

                            // (p0/p1) - (p1/p2)
                        case neg0|pos1|neg2:
                        case pos0|neg1|pos2:
                            segments.push_back( new Segment(db(c0,p0,f0,c1,p1,f1),db(c1,p1,f1,c2,p2,f2)) );
                            break;

                            // (p0/p2) - (p1/p2)
                        case neg0|neg1|pos2:
                        case pos0|pos1|neg2:
                            segments.push_back( new Segment(db(c0,p0,f0,c2,p2,f2),db(c1,p1,f1,c2,p2,f2)) );
                            break;

                            //--------------------------------------
                            //
                            // vertex+intercep on one edge
                            //
                            //--------------------------------------

                            // p0 - (p1/p2)
                        case zzz0|pos1|neg2:
                        case zzz0|neg1|pos2:
                            segments.push_back( new Segment(db(c0,p0),db(c1,p1,f1,c2,p2,f2)) );
                            break;

                            // p1 - (p0/p2)
                        case pos0|zzz1|neg2:
                        case neg0|zzz1|pos2:
                            segments.push_back( new Segment(db(c1,p1),db(c0,p0,f0,c2,p2,f2)) );
                            break;

                            // p2 - (p0/p1)
                        case pos0|neg1|zzz2:
                        case neg0|pos1|zzz2:
                            segments.push_back( new Segment(db(c2,p2),db(c0,p0,f0,c1,p1,f1)) );
                            break;

                            //--------------------------------------
                            //
                            // one of the full edge
                            //
                            //--------------------------------------

                            // p0-p1
                        case zzz0|zzz1|pos2:
                        case zzz0|zzz1|neg2:
                            segments.push_back( new Segment(db(c0,p0),db(c1,p1)) );
                            break;

                            // p1-p2
                        case neg0|zzz1|zzz2:
                        case pos0|zzz1|zzz2:
                            segments.push_back( new Segment(db(c1,p1),db(c2,p2)) );
                            break;

                            // p0-p2
                        case zzz0|neg1|zzz2:
                        case zzz0|pos1|zzz2:
                            segments.push_back( new Segment(db(c0,p0),db(c2,p2)) );
                            break;

                        default:
                            // no creating...
                            break;
                    }

                }

            }

        }

    }
    
}


