//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/ghosts.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! metrics information
        template <size_t DIM> struct Metrics;

        //! 1D Metrics
        template <> struct  Metrics<1>
        {
            static const size_t LocalNodes  = 3;            //!< [-1:0:1]
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 2;            //!< along 2 main axis
            static const size_t AtLevel2    = 0;            //!< N/A
            static const size_t AtLevel3    = 0;            //!< N/A
        };

        //! 2D Metrics
        template <> struct  Metrics<2>
        {
            static const size_t LocalNodes  = 9;            //!< [-1:0:1]^2
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 4;            //!< along 4 mains
            static const size_t AtLevel2    = 4;            //!< along 4 diagonals
            static const size_t AtLevel3    = 0;            //!< N/A
        };

        //! 3D Metrics
        template <> struct  Metrics<3>
        {
            static const size_t LocalNodes  = 27;           //!< [-1:0:1]^3
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 6;            //!< across 6  axis
            static const size_t AtLevel2    = 12;           //!< across 12 edges
            static const size_t AtLevel3    = 8;            //!< across 8  vertices
        };


        //! workspace metrics
        template <typename COORD>
        class Workspace : public Topology::Hub<COORD>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                             LayoutType;                                     //!< alias
            typedef typename LayoutType::Loop                 Loop;                                           //!< alias
            typedef typename LayoutType::coord                coord;                                          //!< alias
            typedef typename LayoutType::const_coord          const_coord;                                    //!< alias
            typedef Topology::Hub<COORD>                      HubType;                                        //!< alias
            typedef Topology::Node<COORD>                     NodeType;                                       //!< alias
            static const size_t                               Dimensions   = Coord::Get<COORD>::Dimensions;   //!< alias
            static const size_t                               Neighbours   = Metrics<Dimensions>::Neighbours; //!< number of possible neighbours and directions
            static const size_t                               Orientations = Neighbours/2;                    //!< number of orientations
            static const size_t                               AtLevel1     = Metrics<Dimensions>::AtLevel1;   //!< alias
            static const size_t                               AtLevel2     = Metrics<Dimensions>::AtLevel2;   //!< alias
            static const size_t                               AtLevel3     = Metrics<Dimensions>::AtLevel3;   //!< alias
            typedef Connectivity::Link<COORD>                 LinkType;                                       //!< alias
            typedef _Ghosts<COORD>                            _GhostsType; //!< alias
            typedef arc_ptr<_GhostsType>                      Ghosts;      //!< dynamic ghosts

            //! lightweight ghosts I/O context
            struct GIO
            {
                const _GhostsType *forward; //!< if has forward
                const _GhostsType *reverse; //!< if has reverse
                unsigned           status;  //!< from GhostsInfo
            };

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const size_t               size;   //!< product of sizes
            const LayoutType           inner;  //!< inner layout
            const LayoutType           outer;  //!< outer layout
            const auto_ptr<LayoutType> heart;  //!< optional heart layout

        private:
            vector<Ghosts>          repository;           //!< all created ghosts
            GIO                     ghosts[Orientations]; //!< placed according to their orientation
        public:

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            //! cleanup
            inline virtual ~Workspace() throw()
            {
                bzset_(size);
            }
            
            //! setup
            inline explicit Workspace(const LayoutType &full,
                                      const_coord      &localSizes,
                                      const Coord1D     globalRank,
                                      const_coord      &PBC,
                                      const Coord1D     ng = 0
                                      ) :
            HubType(localSizes,globalRank,PBC),
            size(  Coord::Product(this->sizes) ),
            inner( full.split(this->sizes,this->ranks) ),
            outer( expandInner(abs_of(ng)) ),
            heart(0),
            repository( Neighbours, as_capacity ),
            ghosts()
            {
                memset( ghosts, 0, sizeof(ghosts) );
                std::cerr << "\t@tile["; Coord::Disp(std::cerr,this->rank,2) << "]=" << inner << " -> " << outer << std::endl;
                if(ng>0)
                {
                    buildGhosts(ng-1);
                }
            }
            
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);

            inline LayoutType expandInner(const Coord1D ng)
            {
                if(ng<=0)
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
                                     ng,
                                     Dimensions);
                    //----------------------------------------------------------
                    //
                    // return modified layout
                    //
                    //----------------------------------------------------------
                    return LayoutType(lower,upper);
                }
            }


            //! try to create Ghosts, shift=ng-1
            inline void findGhosts(const size_t  where,
                                   const_coord   delta,
                                   const Coord1D shift,
                                   coord        &heart_lower,
                                   coord        &heart_upper)
            {
                assert(shift>=0);
                const LinkType link(delta);

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
                    std::cerr << "\t" << link << ':';
                    //----------------------------------------------------------
                    //
                    // get info
                    //
                    //----------------------------------------------------------
                    const_coord           granks = Coord::Regularized(this->sizes,this->ranks+link.direction);

                    //----------------------------------------------------------
                    //
                    // build recv/send layouts from delta
                    //
                    //----------------------------------------------------------
                    coord inner_lower = inner.lower;
                    coord inner_upper = inner.upper;

                    coord outer_lower = inner.lower;
                    coord outer_upper = inner.upper;

                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                        Coord1D &h_up = Coord::Of(heart_upper,dim);
                        Coord1D &h_lo = Coord::Of(heart_lower,dim);
                        switch( Coord::Of(link.direction,dim)  )
                        {
                            case  1:
                                Coord::Of(outer_upper,dim) = Coord::Of(outer.upper,dim);
                                Coord::Of(outer_lower,dim) = Coord::Of(inner_upper,dim) + 1;
                                Coord::Of(inner_lower,dim) = Coord::Of(inner_upper,dim) - shift;
                                h_up = min_of(Coord::Of(inner_lower,dim) - 1,h_up);
                                break;
                            case -1:
                                Coord::Of(outer_lower,dim) = Coord::Of(outer.lower,dim);
                                Coord::Of(outer_upper,dim) = Coord::Of(inner_lower,dim) - 1;
                                Coord::Of(inner_upper,dim) = Coord::Of(inner_lower,dim) + shift;
                                h_lo = max_of(Coord::Of(inner_upper,dim) + 1, h_lo);
                                break;
                            default: break;
                        }
                    }

                    const LayoutType ghostInnerLayout(inner_lower,inner_upper);
                    const LayoutType ghostOuterLayout(outer_lower,outer_upper);
                    assert(outer.contains(ghostInnerLayout));
                    assert(outer.contains(ghostOuterLayout));

                    //----------------------------------------------------------
                    //
                    // create ghosts and push them in their positions
                    //
                    //----------------------------------------------------------
                    _GhostsType *g = new _GhostsType(this->sizes,
                                                     Coord::GlobalRank(this->sizes,granks),
                                                     this->rank,
                                                     link,
                                                     ghostInnerLayout,
                                                     ghostOuterLayout,
                                                     outer);
                    {
                        const Ghosts G = g;
                        std::cerr << G << std::endl;
                        repository.push_back(G);
                    }
                    GIO &gio = ghosts[where];
                    switch(g->link.way)
                    {
                        case Connectivity::Forward:  assert(0==gio.forward); gio.forward = g; gio.status |= GhostsInfo::Fwd;  break;
                        case Connectivity::Reverse:  assert(0==gio.reverse); gio.reverse = g; gio.status |= GhostsInfo::Both; break;
                    }
                }

            }


            //! loop over all directions, two for each orientation
            inline void buildGhosts(const Coord1D shift)
            {
                //--------------------------------------------------------------
                // half loop on [-1:1]^Dimensions, using symetry
                //--------------------------------------------------------------
                coord __lo(0); Coord::LD(__lo,-1);
                coord __up(0); Coord::LD(__up, 1);
                Loop loop(__lo,__up);
                loop.start();

                coord heart_lower = inner.lower;
                coord heart_upper = inner.upper;
                for( size_t j=0; j<Orientations; ++j, loop.next() )
                {
                    // two directions by orientation
                    findGhosts(j, loop.value,shift,heart_lower,heart_upper);
                    findGhosts(j,-loop.value,shift,heart_lower,heart_upper);
                }
                std::cerr << "\t\t#ghosts=" << repository.size() << std::endl;
                std::cerr << "\t\theart_lower=" << heart_lower << std::endl;
                std::cerr << "\t\theart_upper=" << heart_upper << std::endl;
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
                    (auto_ptr<LayoutType>&)heart = new LayoutType(heart_lower,heart_upper);
                    std::cerr << "\t\t\theart=" << heart << std::endl;
                }
                else
                {
                    std::cerr << "\t\t\tNo Heart!" << std::endl;
                }
            }

        };

    }
}


#endif

