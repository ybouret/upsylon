//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/topology.hpp"
#include "y/memory/static-slots.hpp"
#include "y/ptr/arc.hpp"
#include "y/sequence/vector.hpp"

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
            static const size_t AtLevel1    = 2;            //!< along main axis back/forth
            static const size_t AtLevel2    = 0;            //!< N/A
            static const size_t AtLevel3    = 0;            //!< N/A
        };

        //! 2D Metrics
        template <> struct  Metrics<2>
        {
            static const size_t LocalNodes  = 9;            //!< [-1:0:1]^2
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 4;            //!< along main axis back/forth
            static const size_t AtLevel2    = 4;            //!< along diagonals
            static const size_t AtLevel3    = 0;            //!< N/A
        };

        //! 3D Metrics
        template <> struct  Metrics<3>
        {
            static const size_t LocalNodes  = 27;           //!< [-1:0:1]^3
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 6;            //!< along  6  axis
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
            typedef Layout<COORD>                             LayoutType;                                   //!< alias
            typedef typename LayoutType::Loop                 Loop;                                         //!< alias
            typedef typename LayoutType::coord                coord;                                        //!< alias
            typedef typename LayoutType::const_coord          const_coord;                                  //!< alias
            typedef Topology::Hub<COORD>                      HubType;                                      //!< alias
            typedef Topology::Node<COORD>                     NodeType;                                     //!< alias
            static const size_t                               Dimensions = HubType::Dimensions;             //!< alias
            static const size_t                               Neighbours = Metrics<Dimensions>::Neighbours; //!< number of possible neighbours
            static const size_t                               Directions = Neighbours/2;                    //!< number of direction
            static const size_t                               AtLevel1   = Metrics<Dimensions>::AtLevel1;   //!< alias
            static const size_t                               AtLevel2   = Metrics<Dimensions>::AtLevel2;   //!< alias
            static const size_t                               AtLevel3   = Metrics<Dimensions>::AtLevel3;   //!< alias


            class Ghost
            {
            public:
                const LayoutType area;
                vector<Coord1D>  indices;

                inline Ghost( const LayoutType &sub, const LayoutType &out ) :
                area(sub)
                {
                    assert(out.contains(area));
                    out.collect(indices, sub);
                    assert(area.items==indices.size());
                }

                inline ~Ghost() throw()
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ghost);
            };


            //! base class for ghosts inner and outer sublayouts
            /**
             The positional NodeType will be used as peer information for I/O
            */
            class Ghosts_ : public NodeType, public counted_object
            {
            public:
                const Topology::Level   level;  //!< directionality of ghosts
                const Coord1D           host;   //!< global host rank
                const bool              local;  //!< is local
                const bool              async;  //!< is async

                //! setup
                inline explicit Ghosts_(const_coord           &localSizes,
                                        const Coord1D         &globalRank,
                                        const Coord1D         &globalHost,
                                        const Topology::Level &l,
                                        const LayoutType      &innerArea,
                                        const LayoutType      &outerArea,
                                        const LayoutType      &outerLayout) throw() :
                NodeType(localSizes,globalRank), level(l),
                host(globalHost),
                local( (host==this->rank)  ),
                async( !local ),
                inner(innerArea,outerLayout),
                outer(outerArea,outerLayout)
                {
                }

                //! cleanup
                inline virtual ~Ghosts_() throw()
                {
                }

                const Ghost inner; //!< to send
                const Ghost outer; //!< to recv

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ghosts_);
            };

            typedef arc_ptr<Ghosts_> Ghosts; //!< pointer for multiple locations, same data

#if 0
            //! length dependent template for different metrics
            template <size_t LENGTH> class GhostsStore : public memory::static_slots<Ghosts,LENGTH>
            {
            public:
                inline explicit GhostsStore() throw() : memory::static_slots<Ghosts,LENGTH>() {} //!< setup
                inline virtual ~GhostsStore() throw() {} //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(GhostsStore);
            };
#endif

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const size_t            size;  //!< product of sizes
            const LayoutType        inner; //!< inner layout
            const LayoutType        outer; //!< outer layout
#if 0
            GhostsStore<Neighbours> ghosts;  //!< all the ghosts
            GhostsStore<AtLevel1>   ghosts1; //!< Level1 ghosts
            GhostsStore<AtLevel2>   ghosts2; //!< Level2 ghosts
            GhostsStore<AtLevel3>   ghosts3; //!< Level3 ghosts
#endif
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
            outer( expandInner( abs_of(ng) ) )//,ghosts(), ghosts1(), ghosts2(), ghosts3()
            {
                std::cerr << "\ttile[" << this->rank << "]=" << inner << " -> " << outer << std::endl;
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
            inline void tryCreateGhosts(const_coord   delta,
                                        const Coord1D shift)
            {
                assert(shift>=0);
                //--------------------------------------------------------------
                //
                // build the probe: set the center of layout and move
                // coodinates according to delta
                //
                //--------------------------------------------------------------
                coord probe = (inner.lower+inner.upper)/2;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    switch( Coord::Of(delta,dim)  )
                    {
                        case -1: Coord::Of(probe,dim) = Coord::Of(inner.lower,dim)-1; break;
                        case  1: Coord::Of(probe,dim) = Coord::Of(inner.upper,dim)+1; break;
                        default: break;
                    }
                }
                std::cerr << "\t\tlink@delta=" << delta << "\t: ";
                if(outer.has(probe))
                {
                    //----------------------------------------------------------
                    //
                    // get info
                    //
                    //----------------------------------------------------------
                    const Topology::Level glevel = Topology::LevelOf(delta);
                    const_coord           granks = Coord::Regularized(this->sizes,this->ranks+delta);

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
                        switch( Coord::Of(delta,dim)  )
                        {
                            case  1:
                                Coord::Of(outer_lower,dim) = Coord::Of(inner_upper,dim) + 1;
                                Coord::Of(inner_lower,dim) = Coord::Of(inner_upper,dim) - shift;
                                break;
                            case -1:
                                Coord::Of(outer_upper,dim) = Coord::Of(inner_lower,dim) - 1;
                                Coord::Of(inner_upper,dim) = Coord::Of(inner_lower,dim) + shift;
                                break;
                            default: break;
                        }
                    }

                    const LayoutType g_inner(inner_lower,inner_upper);
                    const LayoutType g_outer(outer_lower,outer_upper);
                    assert(outer.contains(g_outer));
                    assert(inner.contains(g_inner));

                    //----------------------------------------------------------
                    //
                    // create ghosts and push them in their positions
                    //
                    //----------------------------------------------------------
                    const Ghosts  g = new Ghosts_(this->sizes,
                                                  Coord::GlobalRank(this->sizes,granks),
                                                  this->rank,
                                                  glevel,
                                                  g_inner,
                                                  g_outer,
                                                  outer);
                    std::cerr << "ghosts.ranks=" << g->ranks << " | " << g->rank << " <-- " << this->rank << " | send: " << g->inner.area << " | recv: " << g->outer.area;
                    if(g->async) std::cerr << " [async]"; else std::cerr << " [local]";
                    std::cerr << std::endl;
                    assert(g_inner.items==g_outer.items);

#if 0
                    ghosts.push_back(g);
                    switch (glevel)
                    {
                        case Topology::Level1:  ghosts1.push_back(g); break;
                        case Topology::Level2:  ghosts2.push_back(g); break;
                        case Topology::Level3:  ghosts3.push_back(g); break;
                    }
#endif
                }
                else
                {
                    std::cerr << "NO" << std::endl;
                }
            }

            inline void buildGhosts(const Coord1D shift)
            {
                std::cerr << "\tlinks@ranks="<< this->ranks << std::endl;

                //! half loop on [-1:1]^Dimensions, using symetry
                coord __lo(0); Coord::LD(__lo,-1);
                coord __up(0); Coord::LD(__up, 1);
                Loop loop(__lo,__up);
                loop.start();

                //size_t levels[3] = { 0,0,0 };
                for( size_t j=0; j<Directions; ++j, loop.next() )
                {
                    tryCreateGhosts( loop.value,shift);
                    tryCreateGhosts(-loop.value,shift);
                }
#if 0
                std::cerr << "#ghosts: " << ghosts.size << std::endl;
                std::cerr << "     @1: " << ghosts1.size << std::endl;
                std::cerr << "     @2: " << ghosts2.size << std::endl;
                std::cerr << "     @3: " << ghosts3.size << std::endl;
#endif
            }

        };
        
    }
}



#endif

