
#include "y/geometry/contour2d.hpp"
#include "y/type/bzset.hpp"
#include "y/comparison.hpp"
#include "y/type/bswap.hpp"
#include "y/sort/sorted-sum.hpp"

namespace  {
    static const char Fn[] = "contour2d: ";
}

namespace upsylon {
    
    namespace geometry {
        
        double contour:: average( const double v1, const double v2, const double v3, const double v4) throw()
        {
            double a[4] = { v1, v2, v3, v4 };
            lightweight_array<double> arr(a,sizeof(a)/sizeof(a[0]));
            return sorted_sum_abs(arr) * 0.25;
        }
        
        contour::sign_type contour:: sign_of( const double value ) throw()
        {
            if(value<0.0)
            {
                return is_negative;
            }
            else if(0.0<value)
            {
                return is_positive;
            }
            else
            {
                return is_zero;
            }
        }
        
    }
    
}


namespace upsylon {
    
    namespace geometry {
        
        contour2d:: coordinate:: coordinate(const unit_t I,
                                            const unit_t J,
                                            const short  Q) throw() :
        i(I), j(J), q(Q)
        {
        }
        
        contour2d:: coordinate:: ~coordinate() throw()
        {
            bzset_(i);
            bzset_(j);
            bzset_(q);
        }
        
        contour2d:: coordinate:: coordinate( const coordinate &_ ) throw() :
        i( _.i ),
        j( _.j ),
        q( _.q )
        {
        }
        
        void contour2d:: coordinate:: __run(hashing::function &H) const throw()
        {
            H.run_type(i);
            H.run_type(j);
            H.run_type(q);
        }
        
        void contour2d:: coordinate:: __sto( unit_t *target ) const throw()
        {
            assert(target);
            target[0] = i;
            target[1] = j;
            target[2] = q;
        }
        
        
        int  contour2d:: coordinate:: compare(const coordinate &lhs, const coordinate &rhs) throw()
        {
            unit_t L[3], R[3];
            lhs.__sto(L);
            rhs.__sto(R);
            return comparison::increasing_lexicographic(L,R,3);
        }
        
        bool operator==(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return (lhs.i==rhs.i) && (lhs.j==rhs.j) && (lhs.q==rhs.q);
        }
        
        bool operator!=(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return (lhs.i!=rhs.i) || (lhs.j!=rhs.j) || (lhs.q!=rhs.q);
        }
        
        std::ostream & operator<<(std::ostream &os,const contour2d::coordinate&c)
        {
            os << '(' << c.i << ',' << c.j << ':' << c.q << ')';
            return os;
        }
        
        bool operator<(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return contour2d::coordinate::compare(lhs,rhs) < 0;
        }
        
    }
    
}

namespace upsylon {
    
    namespace geometry {
        
        
        contour2d:: edge:: ~edge() throw()
        {
        }
        
        contour2d:: edge:: edge( const coordinate &single) throw() :
        lower(single),
        upper(single)
        {
        }

        contour2d:: edge:: edge( const edge &_) throw() :
        lower(_.lower),
        upper(_.upper)
        {
        }
        
        contour2d:: edge:: edge( const coordinate &a, const coordinate &b) throw() :
        lower(a),
        upper(b)
        {
            assert(a!=b);
            if( coordinate::compare(lower,upper) > 0 )
            {
                bswap(lower,upper);
            }
            assert(coordinate::compare(lower,upper)<0);
            
        }
        
        bool operator==(const contour2d::edge&lhs,const contour2d::edge&rhs) throw()
        {
            return (lhs.lower==rhs.lower) && (lhs.upper==rhs.upper);
        }
        
        bool operator!=(const contour2d::edge&lhs,const contour2d::edge&rhs) throw()
        {
            return (lhs.lower!=rhs.lower) || (lhs.upper!=rhs.upper);
        }
        
        contour2d:: edge:: hasher::  hasher() throw() : H() {}
        contour2d:: edge:: hasher:: ~hasher() throw()  {}
        
        size_t contour2d:: edge:: hasher::operator()(const edge &_) throw()
        {
            H.set();
            _.lower.__run(H);
            _.upper.__run(H);
            return H.key<size_t>();
        }
        
    }
    
}

namespace upsylon {
    
    namespace geometry {
        
        const contour2d::edge & contour2d:: point_:: key() const throw()
        {
            return location;
        }
        
        contour2d::point_:: ~point_() throw()
        {
        }
        
        contour2d::point_:: point_(const edge &l, const vertex &p) throw() :
        location(l),
        position(p)
        {
        }
    }
    
}

namespace upsylon {

    namespace geometry {

        contour2d:: level_:: level_( const size_t k ) throw() :
        index(k),
        slist()
        {

        }

        contour2d:: level_:: ~level_() throw()
        {
            bzset_(index);
        }

        const size_t & contour2d:: level_:: key() const throw()
        {
            return index;
        }

    }

}

namespace upsylon {

    namespace geometry {

        contour2d:: segments:: segments() throw()
        {
        }

        contour2d:: segments:: ~segments() throw()
        {
        }



    }
}

#include "y/exception.hpp"

namespace upsylon {

    namespace geometry {

        contour2d:: level_set:: level_set() throw()
        {
        }

        contour2d:: level_set:: ~level_set() throw()
        {
        }

        void contour2d:: level_set:: create(const size_t n)
        {
            free();
            ensure(n);
            for(size_t k=1;k<=n;++k)
            {
                const level l = new level_(k);
                if( !insert(l) )
                {
                    throw exception("%sinsert#%u failure", Fn,unsigned(k) );
                }
            }
            assert(size()==n);
        }

    }
}

namespace upsylon {
    
    namespace geometry {
        
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
switch(contour::sign_of(d##I))\
{\
case contour::is_negative:  flags |= n##I; break;\
case contour::is_zero:      flags |= z##I; break;\
case contour::is_positive:  flags |= p##I; break;\
} } while(false)
        
        void contour2d:: scan_triangles(context &ctx)
        {
            static const unsigned i0 = 0;
            static const unsigned ii[4][2] =
            {
                {1,2}, {2,3}, {3,4}, {4,1}
            };

            //------------------------------------------------------------------
            //
            // Loop over the four triangles
            //
            //------------------------------------------------------------------
            assert(ctx.c);
            assert(ctx.d);
            assert(ctx.v);
            assert(ctx.l);
            level_ &L = *ctx.l;
            for(size_t t=0;t<4;++t)
            {
                const unsigned i1 = ii[t][0];
                const unsigned i2 = ii[t][1];

                const double   d0 = ctx.d[i0];
                const double   d1 = ctx.d[i1];
                const double   d2 = ctx.d[i2];



                //const coordinate &c0 = ctx.c[0];
                //const coordinate &c1 = ctx.c[1];
                //const coordinate &c2 = ctx.c[2];

                unsigned flags = 0;
                Y_CONTOUR2D_FLAG(0);
                Y_CONTOUR2D_FLAG(1);
                Y_CONTOUR2D_FLAG(2);
                
                switch(flags)
                {
                        //------------------------------------------------------
                        //
                        // negative@i0
                        //
                        //------------------------------------------------------

                        //------------------------------------------------------
                        // negative@i1
                        //------------------------------------------------------
                    case n0|n1|n2:break;
                    case n0|n1|z2:break;
                    case n0|n1|p2:break;

                        //------------------------------------------------------
                        // zero@i1
                        //------------------------------------------------------
                    case n0|z1|n2:break;
                    case n0|z1|z2:break;
                    case n0|z1|p2:break;

                        //------------------------------------------------------
                        // positive@i1
                        //------------------------------------------------------
                    case n0|p1|n2:break;
                    case n0|p1|z2:break;
                    case n0|p1|p2:break;

                        //------------------------------------------------------
                        //
                        // zero@i0
                        //
                        //------------------------------------------------------

                        //------------------------------------------------------
                        // negative@i1
                        //------------------------------------------------------
                    case z0|n1|n2:break;
                    case z0|n1|z2:break;
                    case z0|n1|p2:break;

                        //------------------------------------------------------
                        // zero@i1
                        //------------------------------------------------------
                    case z0|z1|n2:break;
                    case z0|z1|z2:break;
                    case z0|z1|p2:break;

                        //------------------------------------------------------
                        // positive@i1
                        //------------------------------------------------------
                    case z0|p1|n2:break;
                    case z0|p1|z2:break;
                    case z0|p1|p2:break;


                        //------------------------------------------------------
                        //
                        // positive@i0
                        //
                        //------------------------------------------------------

                        //------------------------------------------------------
                        // negative@i1
                        //------------------------------------------------------
                    case p0|n1|n2:break;
                    case p0|n1|z2:break;
                    case p0|n1|p2:break;

                        //------------------------------------------------------
                        // zero@i1
                        //------------------------------------------------------
                    case p0|z1|n2:break;
                    case p0|z1|z2:break;
                    case p0|z1|p2:break;

                        //------------------------------------------------------
                        // positive@i1
                        //------------------------------------------------------
                    case p0|p1|n2:break;
                    case p0|p1|z2:break;
                    case p0|p1|p2:break;


                    default:
                        throw exception("%sscan(unexpected case)",Fn);


                }
                
            }
        }
        
        
    }
    
}

