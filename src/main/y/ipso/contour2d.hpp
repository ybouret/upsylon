//! \file
#ifndef Y_IPSO_CONTOUR2D_INCLUDED
#define Y_IPSO_CONTOUR2D_INCLUDED 1

#include "y/sequence/array.hpp"
#include "y/type/utils.hpp"
#include "y/type/point2d.hpp"
#include "y/ptr/counted.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ipso
    {

        //! algorithm to find 2d contour segments
        struct contour2d
        {
            //! called upon matching segment
            typedef void (*callback)(double x1,
                                     double y1,
                                     double x2,
                                     double y2,
                                     double level,
                                     size_t indx,
                                     void *args);

            /** a low-level contour algorithm
             d               ! matrix/field of data to contour
             ilb,iub,jlb,jub ! index bounds of data matrix
             x               ! data matrix column coordinates
             y               ! data matrix row coordinates
             z               ! contour levels in increasing order, nc=z.size()
             */
            template<
            typename FIELD,
            typename ARRAY
            > static inline
            void scan(const FIELD         &d,
                      const unit_t         ilb,
                      const unit_t         iub,
                      const unit_t         jlb,
                      const unit_t         jub,
                      const ARRAY         &x,
                      const ARRAY         &y,
                      const array<double> &z,
                      callback             proc,
                      void                *args
                      )
            {

#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])

                double h[5]  = { 0 };
                int    sh[5] = { 0 };
                double xh[5] = { 0 };
                double yh[5] = { 0 };
                static const unit_t im[4] = {0,1,1,0};
                static const unit_t jm[4] = {0,0,1,1};
                static const int    ic[3][3][3] =
                {
                    { {0,0,8},{0,2,5},{7,6,9} },
                    { {0,3,4},{1,3,1},{4,3,0} },
                    { {9,6,7},{5,2,0},{8,0,0} }
                };

                const size_t nc  = z.size();
                const double zlo = z[1];
                const double zhi = z[nc]; assert(zhi>=zlo);
                for(unit_t j=jlb;j<jub;++j)
                {
                    for(unit_t i=ilb;i<iub;++i)
                    {
                        // min value
                        double       temp1 = min_of<double>(d[i][j],d[i][j+1]);
                        double       temp2 = min_of<double>(d[i+1][j],d[i+1][j+1]);
                        const double dmin  = min_of(temp1,temp2);
                        // max value
                        temp1 = max_of<double>(d[i][j],d[i][j+1]);
                        temp2 = max_of<double>(d[i+1][j],d[i+1][j+1]);
                        const double dmax  = max_of(temp1,temp2);
                        if( (dmax<zlo) || (dmin>zhi) )
                        {
                            continue;
                        }
                        // scan levels
                        for(size_t k=nc;k>0;--k)
                        {
                            const double level = z[k];
                            if ( level<dmin || level>dmax )
                            {
                                continue;
                            }
                            for(int m=4;m>=0;--m)
                            {
                                if (m > 0)
                                {
                                    h[m]  = double(d[i+im[m-1]][j+jm[m-1]])-level;
                                    xh[m] = double(x[i+im[m-1]]);
                                    yh[m] = double(y[j+jm[m-1]]);
                                }
                                else
                                {
                                    h[0]  = 0.25 * (h[1]+h[2]+h[3]+h[4]);
                                    xh[0] = 0.50 * (double(x[i])+double(x[i+1]));
                                    yh[0] = 0.50 * (double(y[j])+double(y[j+1]));
                                }
                                if (h[m] > 0.0)
                                    sh[m] = 1;
                                else if (h[m] < 0.0)
                                    sh[m] = -1;
                                else
                                    sh[m] = 0;
                            }

                            /*
                             Note: at this stage the relative heights of the corners and the
                             centre are in the h array, and the corresponding coordinates are
                             in the xh and yh arrays. The centre of the box is indexed by 0
                             and the 4 corners by 1 to 4 as shown below.
                             Each triangle is then indexed by the parameter m, and the 3
                             vertices of each triangle are indexed by parameters m1,m2,and m3.
                             It is assumed that the centre of the box is always vertex 2
                             though this isimportant only when all 3 vertices lie exactly on
                             the same contour level, in which case only the side of the box
                             is drawn.
                             vertex 4 +-------------------+ vertex 3
                             .        | \               / |
                             .        |   \    m=3    /   |
                             .        |     \       /     |
                             .        |       \   /       |
                             .        |  m=2    X   m=2   |       the centre is vertex 0
                             .        |       /   \       |
                             .        |     /       \     |
                             .        |   /    m=1    \   |
                             .        | /               \ |
                             vertex 1 +-------------------+ vertex 2
                             */
                            /* Scan each triangle in the box */
                            for(int m=1;m<=4;m++) {
                                const int m1 = m;
                                const int m2 = 0;
                                const int m3 = (m!=4) ? (m+1) : 1;
                                const int iv = ic[sh[m1]+1][sh[m2]+1][sh[m3]+1];
                                if (0==iv)
                                    continue;
                                double x1=0,x2=0,y1=0,y2=0;
                                switch (iv) {
                                    case 1: /* Line between vertices 1 and 2 */
                                        x1 = xh[m1];
                                        y1 = yh[m1];
                                        x2 = xh[m2];
                                        y2 = yh[m2];
                                        break;
                                    case 2: /* Line between vertices 2 and 3 */
                                        x1 = xh[m2];
                                        y1 = yh[m2];
                                        x2 = xh[m3];
                                        y2 = yh[m3];
                                        break;
                                    case 3: /* Line between vertices 3 and 1 */
                                        x1 = xh[m3];
                                        y1 = yh[m3];
                                        x2 = xh[m1];
                                        y2 = yh[m1];
                                        break;
                                    case 4: /* Line between vertex 1 and side 2-3 */
                                        x1 = xh[m1];
                                        y1 = yh[m1];
                                        x2 = xsect(m2,m3);
                                        y2 = ysect(m2,m3);
                                        break;
                                    case 5: /* Line between vertex 2 and side 3-1 */
                                        x1 = xh[m2];
                                        y1 = yh[m2];
                                        x2 = xsect(m3,m1);
                                        y2 = ysect(m3,m1);
                                        break;
                                    case 6: /* Line between vertex 3 and side 1-2 */
                                        x1 = xh[m3];
                                        y1 = yh[m3];
                                        x2 = xsect(m1,m2);
                                        y2 = ysect(m1,m2);
                                        break;
                                    case 7: /* Line between sides 1-2 and 2-3 */
                                        x1 = xsect(m1,m2);
                                        y1 = ysect(m1,m2);
                                        x2 = xsect(m2,m3);
                                        y2 = ysect(m2,m3);
                                        break;
                                    case 8: /* Line between sides 2-3 and 3-1 */
                                        x1 = xsect(m2,m3);
                                        y1 = ysect(m2,m3);
                                        x2 = xsect(m3,m1);
                                        y2 = ysect(m3,m1);
                                        break;
                                    case 9: /* Line between sides 3-1 and 1-2 */
                                        x1 = xsect(m3,m1);
                                        y1 = ysect(m3,m1);
                                        x2 = xsect(m1,m2);
                                        y2 = ysect(m1,m2);
                                        break;
                                    default:
                                        break;
                                }

                                /* Finally 'draw' the line */
                                if(proc)
                                {
                                    proc(x1,y1,x2,y2,level,k,args);
                                }
                            } /* m */
                        } /* k - contour */
                    } /* i */
                } /* j */
#undef ysect
#undef xsect
            }

            //! point for segment
            typedef point2d<double> point;

            //! segment=two points
            class segment : public object
            {
            public:
                segment *next;
                segment *prev;
                const point a;
                const point b;

                inline explicit segment( const point _a, const point _b) throw() : next(0), prev(0), a(_a), b(_b) {}
                inline virtual ~segment() throw() {}
                inline void swap() const throw() { cswap(a,b); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };

            //! base class for segments
            typedef core::list_of_cpp<segment> segments_type;

            //! list of segments
            class segments : public segments_type, public counted_object
            {
            public:
                typedef intr_ptr<size_t,segments>   pointer;
                typedef set<size_t,pointer>         database;
                typedef core::list_of_cpp<segments> list_type;

                const size_t indx;
                const double level;
                segments    *next;
                segments    *prev;
                inline explicit segments( const size_t id, const double value) throw() :
                segments_type(), counted_object(), indx(id), level(value), next(0), prev(0) {}

                inline virtual ~segments() throw() {}
                inline const size_t & key() const throw() { return indx; }

                void split( list_type &L )
                {
                    L.clear();
                    while( size )
                    {
                        const point a = tail->a;
                        const point b = tail->b;
                        bool linked = false;
                        for( segments *segs = L.head; segs; segs=segs->next )
                        {

                        }
                        if(!linked)
                        {
                            segments *segs = new segments(indx,level);
                            L.push_back(segs);
                            segs->push_back( pop_back() );
                        }
                        // else continue
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segments);
            };

            class iso_segments : public segments::database
            {
            public:
                inline explicit iso_segments(size_t n=0) : segments::database(n,as_capacity) {}
                inline virtual ~iso_segments() throw() {}

                template<
                typename FIELD,
                typename ARRAY
                >  inline
                void ld(const FIELD         &d,
                        const unit_t         ilb,
                        const unit_t         iub,
                        const unit_t         jlb,
                        const unit_t         jub,
                        const ARRAY         &x,
                        const ARRAY         &y,
                        const array<double> &z)
                {
                    scan<FIELD,ARRAY>(d,ilb,iub,jlb,jub,x,y,z, cb, this );
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(iso_segments);
                static inline void cb(double x1,
                                      double y1,
                                      double x2,
                                      double y2,
                                      double level,
                                      size_t indx,
                                      void  *args)
                {
                    const point   a(x1,y1);
                    const point   b(x2,y2);
                    iso_segments &self = *static_cast<iso_segments*>(args);
                    segments     *pSeg = 0;
                    {
                        segments::pointer *ppSeg = self.search(indx);
                        if(ppSeg)
                        {
                            pSeg = & **ppSeg;
                        }
                        else
                        {
                            pSeg = new segments(indx,level);
                            const segments::pointer q = pSeg;
                            if(!self.insert(q))
                            {
                                throw exception("unexpected failure to insert segments#%u", unsigned(indx) );
                            }
                        }
                    }
                    assert(pSeg);
                    pSeg->push_back( new segment(a,b) );
                }
            };

        };

    }

}

#endif


