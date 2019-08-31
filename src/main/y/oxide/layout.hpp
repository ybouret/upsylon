//! \file
#ifndef Y_OXIDE_LAYOUT_INCLUDED
#define Y_OXIDE_LAYOUT_INCLUDED 1

#include "y/oxide/types.hpp"
#include "y/sort/unique.hpp"
#include "y/sort/sequence.hpp"
#include "y/code/multi-loop.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! common data for layout
        class LayoutInfo
        {
        public:
            const size_t dimension;        //!< [1|2|3]
            const size_t items;            //!< linear items
            virtual ~LayoutInfo() throw(); //!< destructor

        protected:
            explicit LayoutInfo(const size_t) throw();        //!< setup dimensions, no items
            explicit LayoutInfo( const LayoutInfo &) throw(); //!< copy

            //! split in 1D
            static void Split1D( Coord1D &length, Coord1D &offset, const Coord1D sz, const Coord1D rk);

        private:
            Y_DISABLE_ASSIGN(LayoutInfo);
        };

        //! a layout is a metrics based on a coordinate
        template <typename COORD>
        class Layout : public LayoutInfo
        {
        public:
            static const size_t Dimensions = DimensionsOf<COORD>::Value; //!< alias
            typedef COORD       coord;                                   //!< alias
            typedef const coord const_coord;                             //!< alias
            typedef multi_loop<Coord1D,COORD> Loop;                      //!< loop over sub layout

            const_coord lower; //!< lower coordinate
            const_coord upper; //!< upper coordinate
            const_coord width; //!< witdh in each dimension
            const_coord pitch; //!< pitch 1, nx, nx*ny to compute indices

            //! cleanup
            inline virtual ~Layout() throw() {}

            //! setup by two coordinates
            inline explicit Layout(const_coord lo,
                                   const coord up) throw() :
            LayoutInfo(Dimensions),
            lower(lo),
            upper(up),
            width(up),
            pitch(up)
            {
                size_t  &n = (size_t  &)items;
                Coord1D *l = (Coord1D *)&lower;
                Coord1D *u = (Coord1D *)&upper;
                Coord1D *w = (Coord1D *)&width;
                Coord1D *p = (Coord1D *)&pitch;
                n=1;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    if(l[dim]>u[dim]) cswap(l[dim],u[dim]);
                    p[dim] = n;
                    n *= ( w[dim] = (u[dim]+1-l[dim]));
                }
            }

            //! copy
            inline Layout( const Layout &other ) throw() :
            LayoutInfo(other),
            lower( other.lower ),
            upper( other.upper ),
            width( other.width ),
            pitch( other.pitch )
            {
            }

            //! display
            friend inline std::ostream & operator<<( std::ostream &os, const Layout &L )
            {
                os << "{" << L.lower << "->" << L.upper << " : #" << L.width << "=" << L.items << "}";
                return os;
            }

            //! test if a coordinate is inside
            inline bool has( const_coord C ) const throw()
            {
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D l = CoordOf(lower,dim);
                    const Coord1D u = CoordOf(upper,dim);
                    const Coord1D c = CoordOf(C,dim);
                    if(c<l||c>u) return false;
                }
                return true;
            }

            //! test if a sub-layout is contained
            inline bool contains( const Layout &sub ) const throw()
            {
                return has( sub.lower ) && has( sub.upper );
            }


            //! index of a coordinate
            inline Coord1D indexOf(const_coord q) const throw()
            {
                assert( has(q) );
                Coord1D ans = CoordOf(q,0)-CoordOf(lower,0);
                for(size_t dim=1;dim<Dimensions;++dim)
                {
                    ans += (CoordOf(q,dim)-CoordOf(lower,dim))*CoordOf(pitch,dim);
                }
                assert(ans>=0); assert(ans<Coord1D(items));
                return ans;
            }

            //! coordinate of index
            inline coord coordOf(const Coord1D idx) const throw()
            {
                assert(idx>=0);
                assert(idx<Coord1D(items));
                coord   q(0);
                Coord1D rem = idx;
                for(size_t dim=Dimensions-1;dim>0;--dim)
                {
                    const Coord1D den = CoordOf(pitch,dim);
                    const Coord1D qot = rem / den;
                    CoordOf(q,dim) = qot + CoordOf(lower,dim);
                    rem -= qot * den;
                }
                CoordOf(q,0) = rem + CoordOf(lower,0);
                
                return q;
            }

            //! random coordinate within the layout
            inline COORD rand( randomized::bits &ran ) const throw()
            {
                return CoordOps::Within(lower,upper,ran);
            }

            //! collecting indices
            template <typename SEQUENCE> inline
            void collect(SEQUENCE     &indices,
                         const Layout &sub) const
            {
                assert(this->contains(sub));
                {                                                  //
                    Loop       loop(sub.lower,sub.upper);          // make the loop
                    indices.ensure( indices.size() + loop.count ); // adjust memory
                    for(loop.start();loop.active();loop.next())    // loop on sub coordinates
                    {                                              //
                        indices.push_back( indexOf(loop.value) );  //
                    }                                              //
                }                                                  //
                unique(indices);                                   // stay sorted and unique
            }

            //! split this
            Layout split(const_coord sizes,
                         const_coord ranks) const
            {
                coord nn = width;
                coord lo = lower;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    Split1D(CoordOf(nn,dim),
                            CoordOf(lo,dim ),
                            CoordOf(sizes,dim),
                            CoordOf(ranks,dim));

                }
                coord up = nn + lo;
                return Layout(lo,CoordDecrease(up));
            }
            
            //! return matching mapping
            inline void buildMappings( sequence<COORD> &mappings, const size_t cores ) const
            {
                //--------------------------------------------------------------
                // cleanup
                //--------------------------------------------------------------
                assert(cores>0);
                mappings.free();

                //--------------------------------------------------------------
                // build loop
                //--------------------------------------------------------------
                COORD org(0);
                COORD top(0);
                for(size_t dim=0;dim<DimensionsOf<COORD>::Value;++dim)
                {
                    CoordOf(org,dim) = 1;
                    CoordOf(top,dim) = cores;
                }
                Loop   loop(org,top);
                //--------------------------------------------------------------
                // check all possible values
                //--------------------------------------------------------------
                for(loop.start();loop.active();loop.next())
                {
                    //----------------------------------------------------------
                    // check number of cores
                    //----------------------------------------------------------
                    const size_t local_cores = CoordProduct(loop.value);
                    if(cores!=local_cores) continue;


                    //----------------------------------------------------------
                    // check enough item in each dimension
                    //----------------------------------------------------------
                    bool valid = true;
                    for(size_t dim=0;dim<DimensionsOf<COORD>::Value;++dim)
                    {
                        if( CoordOf(loop.value,dim)>CoordOf(width,dim) )
                        {
                            valid = false;
                            break;
                        }
                    }

                    //----------------------------------------------------------
                    // keep it
                    //----------------------------------------------------------
                    if(valid)
                    {
                        mappings.push_back(loop.value);
                    }
                }
            }


            void buildPartition( sequence<Layout> &partition, const COORD &mapping ) const
            {
                // check memory
                partition.free();
                const size_t cores = CoordProduct(mapping); assert(cores>0);
                partition.ensure(cores);

                // build look on local ranks
                coord org(0);
                coord top(0);
                for(size_t dim=0;dim<DimensionsOf<COORD>::Value;++dim)
                {
                    CoordOf(org,dim) = 0;
                    CoordOf(top,dim) = CoordOf(mapping,dim)-1;
                }

                Loop loop(org,top);
                for(loop.start();loop.active();loop.next())
                {
                    const Layout part = split(mapping,loop.value);
                    partition.push_back(part);
                }
                assert( partition.size() == cores );

            }





        private:
            Y_DISABLE_ASSIGN(Layout);
        };


        typedef Layout<Coord1D> Layout1D; //!< 1D layout
        typedef Layout<Coord2D> Layout2D; //!< 2D layout
        typedef Layout<Coord3D> Layout3D; //!< 3D layout
        
    }
}

#endif

