//! \file
#ifndef Y_OXIDE_LAYOUTS_INCLUDED
#define Y_OXIDE_LAYOUTS_INCLUDED 1

#include "y/oxide/axis-layouts.hpp"
#include "y/oxide/ghosts.hpp"
#include "y/oxide/field/metrics.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/auto.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Oxide {

      

        //======================================================================
        //
        //! Layouts for a local compute node
        //
        //======================================================================
        template <typename COORD>
        class Layouts : public Topology::Hub<COORD>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                             LayoutType;                                     //!< alias
            typedef typename LayoutType::coord                coord;                                          //!< alias
            typedef typename LayoutType::const_coord          const_coord;                                    //!< alias
            typedef typename LayoutType::Loop                 Loop;                                           //!< alias
            typedef Topology::Hub<COORD>                      HubType;                                        //!< alias
            static const size_t                               Dimensions   = Coord::Get<COORD>::Dimensions;   //!< alias
            static const size_t                               Neighbours   = Metrics<Dimensions>::Neighbours; //!< number of possible neighbours and directions
            static const size_t                               Orientations = Neighbours/2;                    //!< number of orientations
            typedef Ghosts<COORD>                             GhostsType;                                     //!< alias
            typedef const GhostsType                         *Peer;                                           //!< alias
            typedef arc_ptr<GhostsType>                       GhostsPointer;                                  //!< dynamic ghosts
            typedef vector<coord>                             Coordinates;                                    //!< Coordinates for exteriro
            typedef slots<AxisLayouts>                        AxisType;

            //! lightweight ghosts I/O context for one orientation
            struct GIO
            {
                Peer       forward; //!< if has forward
                Peer       reverse; //!< if has reverse
                unsigned   status;  //!< from GhostsInfo
                bool       local;   //!< for exchange
                bool       async;   //!< for load/save
            };

            //! lighweight node to handle local-only nodes in a list
            class gNode : public object
            {
            public:
                gNode     *next; //!< for list
                gNode     *prev; //!< for list
                const GIO &gio;  //!< reference to local ghosts

                //! setup
                inline explicit gNode( const GIO &ref ) throw() : next(0), prev(0), gio(ref) { assert(gio.local); }

                //! cleanup
                inline virtual ~gNode() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(gNode);
            };

            //! list of gNodes
            typedef core::list_of_cpp<gNode> gList;

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const LayoutType           inner;     //!< inner layout
            const LayoutType           outer;     //!< outer layout
            const auto_ptr<LayoutType> center;    //!< layout that doesn't need ASYNC exchange for computation
            const Coordinates          border;    //!< coordinates that need ASYNC to be updated

        private:
            vector<GhostsPointer>   repository;           //!< all created ghosts
        protected:
            GIO                     ghosts[Orientations]; //!< placed according to their orientation
        public:
            const gList             localGhosts;           //!< pair of local ghosts
            const size_t            localComms;            //!< local exchanged items per field
            const size_t            asyncComms;            //!< async exchanged items per field
            const AxisType          axis;

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            //! cleanup
            inline virtual ~Layouts() throw()
            {
                memset( ghosts, 0, sizeof(ghosts) );
            }
            
            //! setup subordinate
            inline explicit Layouts(const LayoutType &fullLayout,
                                    const_coord       localSizes,
                                    const Coord1D     globalRank,
                                    const_coord       boundaries,
                                    const Coord1D     ghostsZone
                                    ) :
            HubType(localSizes,globalRank,boundaries),
            inner( fullLayout.split(this->sizes,this->ranks) ),
            outer( expandInner(abs_of(ghostsZone)) ),
            center(0),
            border(),
            repository( Neighbours, as_capacity ),
            ghosts(),
            localGhosts(),
            localComms(0),
            asyncComms(0),
            axis( Dimensions )
            {
                setup(ghostsZone);
                buildAxis();


            }

            
            //! display information
            void display(std::ostream &os, const char *pfx=0) const
            {
                static const char default_pfx[] = "";
                if(!pfx)    pfx = default_pfx;
                os << pfx << "ranks    = "; Coord::Disp(os, this->ranks, 2) << " <=> rank=" << this->rank << std::endl;
                os << pfx << "inner    = " << inner             << std::endl;
                os << pfx << "outer    = " << outer             << std::endl;
                os << pfx << "@center  = " << center            << std::endl;
                os << pfx << "#border  = " << border.size()     << std::endl;
                os << pfx << "#ghosts  = " << repository.size() << std::endl;
                os << pfx << "#local   = " << localGhosts.size  << std::endl;
                os << pfx << "#comms   = local:" << localComms <<  " async:" << asyncComms << std::endl;
                for(Coord1D i=0;i<Coord1D(Orientations);++i)
                {
                    os << pfx << "@orientation#" << std::setw(2) << i << " : " << std::endl;
                    const GIO &gio = ghosts[i];
                    if(gio.forward)
                    {
                        assert( 0!= (gio.status&GhostsInfo::Fwd) );
                        std::cerr << pfx << "|_" << (*gio.forward) << std::endl;
                    }
                    if(gio.reverse)
                    {
                        assert( 0!= (gio.status&GhostsInfo::Rev) );
                        std::cerr << pfx << "|_" << (*gio.reverse) << std::endl;
                    }
                }

            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layouts);

            //! setup function
            void setup( const size_t ghostsZone )
            {
                memset( ghosts, 0, sizeof(ghosts) );
                if(ghostsZone>0)
                {
                    buildGhosts(ghostsZone-1);
                }
                else
                {
                    (auto_ptr<LayoutType>&)center = new LayoutType(inner);
                }
                buildBorder();
            }

            //==================================================================
            //
            //
            //! expand inner boundaries according to local topology
            //
            //
            //==================================================================
            inline LayoutType expandInner(const Coord1D ghostsZone) const
            {
                if(ghostsZone<=0)
                {
                    return inner;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // get coordinates to modify
                    //
                    //----------------------------------------------------------
                    coord          lower = inner.lower;
                    coord          upper = inner.upper;

                    //----------------------------------------------------------
                    //
                    // Expand per dimension, using Hub properties
                    //
                    //----------------------------------------------------------
                    Topology::Expand((Coord1D *)       & lower,
                                     (Coord1D *)       & upper,
                                     (const Coord1D *) & inner.width,
                                     (const bool *)    & this->head,
                                     (const bool *)    & this->tail,
                                     (const bool *)    & this->bulk,
                                     (const bool *)    & this->par,
                                     (const bool *)    & this->pbc,
                                     ghostsZone,
                                     Dimensions);
                    //----------------------------------------------------------
                    //
                    // return modified layout
                    //
                    //----------------------------------------------------------
                    return LayoutType(lower,upper);
                }
            }

            //==================================================================
            //
            //
            //! try to create Ghosts, shift=ng-1
            //
            //
            //==================================================================
            inline void findGhosts(const size_t  where,
                                   const_coord   delta,
                                   const Coord1D shift,
                                   coord        &heart_lower,
                                   coord        &heart_upper)
            {
                assert(shift>=0);
                const Conn::Link<COORD>  link(delta); // normalize delta

                //--------------------------------------------------------------
                //
                // build the probe: set the center of layout and move
                // coodinates according to delta
                //
                //--------------------------------------------------------------
                coord probe = (inner.lower+inner.upper)/2;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    switch( Coord::Of(link.direction,dim)  )
                    {
                        case -1: Coord::Of(probe,dim) = Coord::Of(inner.lower,dim)-1; break;
                        case  1: Coord::Of(probe,dim) = Coord::Of(inner.upper,dim)+1; break;
                        default: break;
                    }
                }

                if(outer.has(probe))
                {
                    //----------------------------------------------------------
                    //
                    // get info
                    //
                    //----------------------------------------------------------

                    // get local ranks and global rank
                    const_coord           gRanks  = Coord::Regularized(this->sizes,this->ranks+link.direction);
                    const Coord1D         gRank   = Coord::GlobalRank(this->sizes,gRanks);

                    // deduce if asynchronous ghost and prepare heart shift
                    const bool            gAsync  = (gRank  != this->rank);
                    const Coord1D         gHeart  = (gAsync ? shift+1 : 0);
                    
                    //----------------------------------------------------------
                    //
                    // compute recv/send layouts from delta
                    //
                    //----------------------------------------------------------
                    coord inner_lower = inner.lower;
                    coord inner_upper = inner.upper;

                    coord outer_lower = inner.lower;
                    coord outer_upper = inner.upper;

                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                        Coord1D &heartUp = Coord::Of(heart_upper,dim);
                        Coord1D &heartLo = Coord::Of(heart_lower,dim);
                        switch( Coord::Of(link.direction,dim)  )
                        {
                            case  1:
                                Coord::Of(outer_upper,dim) = Coord::Of(outer.upper,dim);
                                Coord::Of(outer_lower,dim) = Coord::Of(inner_upper,dim) + 1;
                                Coord::Of(inner_lower,dim) = Coord::Of(inner_upper,dim) - shift;
                                if(gAsync)
                                {
                                    heartUp = min_of(Coord::Of(inner_lower,dim) - gHeart,heartUp);
                                }
                                break;
                            case -1:
                                Coord::Of(outer_lower,dim) = Coord::Of(outer.lower,dim);
                                Coord::Of(outer_upper,dim) = Coord::Of(inner_lower,dim) - 1;
                                Coord::Of(inner_upper,dim) = Coord::Of(inner_lower,dim) + shift;
                                if(gAsync)
                                {
                                    heartLo = max_of(Coord::Of(inner_upper,dim) + gHeart, heartLo);
                                }
                                break;
                            default: break;
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // build  layouts from computed coordinates
                    //
                    //----------------------------------------------------------
                    const LayoutType ghostInnerLayout(inner_lower,inner_upper);
                    const LayoutType ghostOuterLayout(outer_lower,outer_upper);
                    assert(outer.contains(ghostInnerLayout));
                    assert(outer.contains(ghostOuterLayout));

                    //----------------------------------------------------------
                    //
                    // create ghosts and push them in their positions
                    //
                    //----------------------------------------------------------
                    GhostsType *g = new GhostsType(this->sizes,
                                                   gRank,
                                                   this->rank,
                                                   link,
                                                   ghostInnerLayout,
                                                   ghostOuterLayout,
                                                   outer);
                    { const GhostsPointer G = g; repository.push_back(G); }

                    GIO &gio = ghosts[where];
                    switch(g->link.way)
                    {
                        case Conn::Forward:  assert(0==gio.forward); gio.forward = g; gio.status |= GhostsInfo::Fwd; break;
                        case Conn::Reverse:  assert(0==gio.reverse); gio.reverse = g; gio.status |= GhostsInfo::Rev; break;
                    }
                }

            }

            //==================================================================
            //
            //
            //! loop over all directions, two for each orientation
            //
            //
            //==================================================================
            inline void buildGhosts(const Coord1D shift)
            {
                coord heart_lower = inner.lower;
                coord heart_upper = inner.upper;

                //--------------------------------------------------------------
                //
                // half loop on [-1:1]^Dimensions, using symetry
                //
                //--------------------------------------------------------------
                {
                    coord __lo(0); Coord::Ld(__lo,-1);
                    coord __up(0); Coord::Ld(__up, 1);
                    typename LayoutType::Loop loop(__lo,__up);
                    loop.start();

                    for( size_t j=0; j<Orientations; ++j, loop.next() )
                    {
                        findGhosts(j, loop.value,shift,heart_lower,heart_upper);
                        findGhosts(j,-loop.value,shift,heart_lower,heart_upper);
                    }
                }

                //--------------------------------------------------------------
                //
                // setting up full connectivity
                //
                //--------------------------------------------------------------
                for( size_t j=0; j<Orientations; ++j )
                {
                    GIO &gio = ghosts[j];
                    switch (gio.status) {
                        case GhostsInfo::Fwd: assert(gio.forward); assert(gio.forward->async); gio.local = false; gio.async=true; break;
                        case GhostsInfo::Rev: assert(gio.reverse); assert(gio.reverse->async); gio.local = false; gio.async=true; break;
                        case GhostsInfo::Both:
                            assert(gio.forward);
                            assert(gio.reverse);
                            assert(gio.forward->local==gio.reverse->local);
                            assert(gio.forward->async==gio.reverse->async);
                            gio.local = gio.forward->local;
                            gio.async = gio.forward->async;
                            if(gio.local)
                            {
                                ( (gList&)localGhosts ).push_back( new gNode(gio) );
                            }
                            break;
                        default:
                            assert(0==gio.status);
                            assert(0==gio.forward);
                            assert(0==gio.reverse);
                            gio.local = false;
                            gio.async = false;
                            break;
                    }
                }

                //--------------------------------------------------------------
                //
                // check if a valid internal region is present
                //
                //--------------------------------------------------------------
                bool hasHeart = true;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    if( Coord::Of(heart_lower,dim) > Coord::Of(heart_upper,dim) )
                    {
                        hasHeart = false;
                        break;
                    }
                }
                if(hasHeart)
                {
                    (auto_ptr<LayoutType>&)center = new LayoutType(heart_lower,heart_upper);
                }

                //--------------------------------------------------------------
                //
                // count comms
                //
                //--------------------------------------------------------------
                for(size_t i=repository.size();i>0;--i)
                {
                    const GhostsType &G =  *repository[i];
                    const size_t      n = G.inner.items;
                    if(G.local)
                    {
                        (size_t &)localComms += n;
                    }
                    else
                    {
                        assert(G.async);
                        (size_t &)asyncComms += n;
                    }
                }
            }

            //==================================================================
            //
            //
            //! if a center is detected, build border coordinates
            //
            //
            //==================================================================
            void buildBorder()
            {
                assert( 0 == border.size() );
                vector<COORD> &seq = (vector<COORD> &)border;
                Loop          loop(inner.lower,inner.upper);
                if(center.is_valid())
                {
                    // first pass: count and allocate
                    {
                        size_t count = 0;
                        for(loop.start();loop.valid();loop.next()) if( !center->has( loop.value ) ) ++count;
                        seq.ensure(count);
                    }
                    // second pass: load
                    for(loop.start();loop.valid();loop.next()) if( !center->has( loop.value ) ) seq.push_back(loop.value);

                }
                else
                {
                    // everybody is border
                    seq.ensure( inner.items );
                    for(loop.start();loop.valid();loop.next()) seq.push_back( loop.value );
                }
            }

            //==================================================================
            //
            //
            //! build axis layouts
            //
            //
            //==================================================================
            inline void buildAxis() throw()
            {
                AxisType        &ax = (AxisType &) axis;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const AxisLayout  axis_inner = inner.projectOn(dim);
                    const AxisLayout  axis_outer = outer.projectOn(dim);
                    const AxisLayouts axis_descr(axis_inner,axis_outer);
                    ax.push(axis_descr);
                }
            }

        };

    }
}


#endif

