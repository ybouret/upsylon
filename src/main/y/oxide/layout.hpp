//! \file
#ifndef Y_OXIDE_LAYOUT_INCLUDED
#define Y_OXIDE_LAYOUT_INCLUDED 1

#include "y/oxide/types.hpp"
#include "y/sort/unique.hpp"
#include "y/sort/sequence.hpp"
#include "y/counting/mloop.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //======================================================================
        //
        //! common data for layout
        //
        //======================================================================
        class LayoutInfo
        {
        public:
            const size_t dimension;        //!< [1|2|3]
            const size_t items;            //!< linear items
            virtual ~LayoutInfo() throw(); //!< destructor

        protected:
            explicit LayoutInfo(const size_t) throw();        //!< setup dimensions, no items
            explicit LayoutInfo( const LayoutInfo &) throw(); //!< copy

            //! split in 1D, with checking
            static void Split1D( Coord1D &length, Coord1D &offset, const Coord1D sz, const Coord1D rk);

        private:
            Y_DISABLE_ASSIGN(LayoutInfo);
        };

        //======================================================================
        //
        //! a layout is a based on a coordinates...
        //
        //======================================================================
        template <typename COORD>
        class Layout : public LayoutInfo
        {
        public:
            //==================================================================
            //
            // types and definitions
            //
            //==================================================================
            typedef typename type_traits<COORD>::mutable_type   coord;       //!< alias
            typedef const coord                                 const_coord; //!< alias
            typedef mloop<Coord1D,coord>                        Loop;        //!< loop over sub layout
            //! get dimensions
            static const size_t                                 Dimensions = Coord::Get<COORD>::Dimensions;

            //==================================================================
            //
            // members
            //
            //==================================================================
            const_coord lower; //!< lower coordinate
            const_coord upper; //!< upper coordinate
            const_coord width; //!< witdh in each dimension
            const_coord pitch; //!< pitch 1, nx, nx*ny to compute indices

            //==================================================================
            //
            // C++ setup
            //
            //==================================================================

            //! cleanup
            inline virtual ~Layout() throw()
            {
                bzset_(lower);
                bzset_(upper);
                bzset_(width);
                bzset_(pitch);
            }

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
                assert(items==other.items);
            }

            //==================================================================
            //
            // utilities
            //
            //==================================================================

            inline bool is_same_than( const Layout &other ) const throw()
            {
                if( lower == other.lower && upper==other.upper )
                {
                    assert(width==other.width);
                    assert(pitch==other.pitch);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            
            //! display
            friend inline std::ostream & operator<<( std::ostream &os, const Layout &L )
            {
                os << "{";
                Coord::Disp(os,L.lower) << "->";
                Coord::Disp(os,L.upper) << " : #";
                Coord::Disp(os,L.width) << "=";
                const Coord1D n = L.items;
                Coord::Disp(os,n,5) << "}";
                return os;
            }

            //! test if a coordinate is inside
            inline bool has( const_coord C ) const throw()
            {
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D l = Coord::Of(lower,dim);
                    const Coord1D u = Coord::Of(upper,dim);
                    const Coord1D c = Coord::Of(C,dim);
                    if(c<l||c>u) return false;
                }
                return true;
            }

            //! test if a sub-layout is contained
            inline bool contains( const Layout &sub ) const throw()
            {
                return has( sub.lower ) && has( sub.upper );
            }

            //! random coordinate within the layout
            inline COORD rand( randomized::bits &ran ) const throw()
            {
                return Coord::Within(lower,upper,ran);
            }
            
            //==================================================================
            //
            // index<->coordinate conversion
            //
            //==================================================================
            
            //! index of a coordinate
            inline Coord1D indexOf(const_coord q) const throw()
            {
                assert( has(q) );
                Coord1D ans = Coord::Of(q,0)-Coord::Of(lower,0);
                for(size_t dim=1;dim<Dimensions;++dim)
                {
                    ans += (Coord::Of(q,dim)-Coord::Of(lower,dim))*Coord::Of(pitch,dim);
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
                    const Coord1D den = Coord::Of(pitch,dim);
                    const Coord1D qot = rem / den;
                    Coord::Of(q,dim) = qot + Coord::Of(lower,dim);
                    rem -= qot * den;
                }
                Coord::Of(q,0) = rem + Coord::Of(lower,0);
                
                return q;
            }

           
            //==================================================================
            //
            // sub layouts construction
            //
            //==================================================================

            //! collecting indices
            template <typename SEQUENCE> inline
            void collect(SEQUENCE     &indices,
                         const Layout &sub) const
            {
                assert(this->contains(sub));
                {                                                  //
                    Loop       loop(sub.lower,sub.upper);          // make the loop
                    indices.ensure( indices.size() + loop.count ); // adjust memory
                    for(loop.start();loop.valid();loop.next())     // loop on sub coordinates
                    {                                              //
                        indices.push_back( indexOf(loop.value) );  //
                    }                                              //
                }                                                  //
                unique(indices);                                   // stay sorted and unique
            }

            //! split this according to local ranks among sizes
            Layout split(const_coord sizes,
                         const_coord ranks) const
            {
                coord nn = width;
                coord lo = lower;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    Split1D(Coord::Of(nn,dim),
                            Coord::Of(lo,dim ),
                            Coord::Of(sizes,dim),
                            Coord::Of(ranks,dim));

                }
                coord up = nn + lo;
                return Layout(lo,Coord::Decrease(up));
            }

            //! what to do with a mapping
            typedef void (*MappingProc)( const COORD &, void * );

            //! push back mapping into a sequence
            static inline
            void MappingPushBack( const COORD &mapping, void *args )
            {
                assert(args);
                static_cast<sequence<COORD> *>(args)->push_back(mapping);
            }

            //! compute all valid mappings and call proc(mapping,args) on them
            /**
             the product of a mapping coordinates is equal to
             the number of cores, and each coordinate is strictly lower
             than the layout width in the same dimension
             */
            inline void forEachMapping( const size_t cores, MappingProc proc, void *args ) const
            {
                assert(cores>0);
                assert(proc);

                //--------------------------------------------------------------
                // build loop
                //--------------------------------------------------------------
                COORD org(0);
                COORD top(0);
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    Coord::Of(org,dim) = 1;
                    Coord::Of(top,dim) = cores;
                }
                Loop   loop(org,top);
                //--------------------------------------------------------------
                // check all possible values
                //--------------------------------------------------------------
                for(loop.start();loop.valid();loop.next())
                {
                    //----------------------------------------------------------
                    // check number of cores
                    //----------------------------------------------------------
                    const size_t loopCores = Coord::Product(loop.value);
                    if(cores!=loopCores) continue;


                    //----------------------------------------------------------
                    // check enough item in each dimension
                    //----------------------------------------------------------
                    bool valid = true;
                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                        if( Coord::Of(loop.value,dim)>Coord::Of(width,dim) )
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
                        proc(loop.value,args);
                    }
                }
            }

            //! return matching mapping
            inline void buildMappings( sequence<COORD> &mappings, const size_t cores ) const
            {
                mappings.free();
                forEachMapping(cores,MappingPushBack, &mappings);
            }

            //! what to do with a partition layout
            typedef void (*PartitionProc)( const Layout &, void * );
            

            //! build all partition layout from a valid mapping
            inline void forEachPartition( const COORD &mapping, PartitionProc proc, void *args ) const
            {
                assert(Coord::Product(mapping)>0);

                // build loop on local ranks
                coord org(0);
                coord top(0);
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    Coord::Of(org,dim) = 0;
                    Coord::Of(top,dim) = Coord::Of(mapping,dim)-1;
                }
                Loop loop(org,top);
                for(loop.start();loop.valid();loop.next())
                {
                    const Layout part = split(mapping,loop.value);
                    proc(part,args);
                }
            }

            //! push back layout
            static inline void PartitionPushBack(const Layout &L, void *args )
            {
                assert(args);
                static_cast< sequence<Layout> *>(args)->push_back(L);
            }

            //! build partitions
            inline void buildPartition( sequence<Layout> &partition, const COORD &mapping ) const
            {
                // check memory
                partition.free();
                const size_t cores = Coord::Product(mapping); assert(cores>0);
                partition.ensure(cores);
                forEachPartition(mapping, PartitionPushBack, &partition);
            }

            //! get max items for a part
            static inline void PartitionMaxItems(const Layout &L, void *args ) throw()
            {
                assert(args);
                size_t       &max_items = *static_cast<size_t *>(args);
                const size_t  lay_items = L.items;
                if(lay_items>max_items) max_items = lay_items;
            }

            //! get max items of all possible parts
            inline size_t getPartitionMaxItems( const COORD &mapping ) const
            {
                size_t ans = 0;
                forEachPartition(mapping,PartitionMaxItems,&ans);
                return ans;
            }



        private:
            Y_DISABLE_ASSIGN(Layout);
        };

        //======================================================================
        //
        // different layouts
        //
        //======================================================================
        typedef Layout<Coord1D> Layout1D; //!< 1D layout
        typedef Layout<Coord2D> Layout2D; //!< 2D layout
        typedef Layout<Coord3D> Layout3D; //!< 3D layout
        
    }
}

#endif

