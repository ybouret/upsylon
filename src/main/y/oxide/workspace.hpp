//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/topology.hpp"
#include "y/memory/static-slots.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace Oxide
    {
        
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
            typedef Layout<COORD>                             LayoutType; //!< alias
            typedef typename LayoutType::Loop                 Loop;       //!< alias
            typedef typename LayoutType::coord                coord;      //!< alias
            typedef typename LayoutType::const_coord          const_coord;//!< alias
            typedef Topology::Hub<COORD>                      HubType;
            typedef Topology::Node<COORD>                     NodeType;
            static const size_t                               Dimensions = HubType::Dimensions;
            static const size_t                               Directions = HubType::Directions;
            static const size_t                               Neighbours = HubType::Neighbours;
            static const size_t                               AtLevel1   = Metrics<Dimensions>::AtLevel1;
            static const size_t                               AtLevel2   = Metrics<Dimensions>::AtLevel2;
            static const size_t                               AtLevel3   = Metrics<Dimensions>::AtLevel3;

            class Ghosts_ : public NodeType
            {
            public:
                const Topology::Level   level;
                inline explicit Ghosts_(const_coord           &localSizes,
                                        const Coord1D         &globalRank,
                                        const Topology::Level &l ) throw() :
                NodeType(localSizes,globalRank),
                level(l)
                {
                }

                inline virtual ~Ghosts_() throw()
                {
                }

                //const LayoutType inner;
                //const LayoutType outer;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ghosts_);
            };

            typedef arc_ptr<Ghosts_> Ghosts;

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const size_t     size;  //!< product of sizes
            const LayoutType inner; //!< inner layout
            const LayoutType outer; //!< outer layout
            memory::static_slots<Ghosts,Neighbours> ghosts;
            memory::static_slots<Ghosts,AtLevel1>   ghosts1;
            memory::static_slots<Ghosts,AtLevel2>   ghosts2;
            memory::static_slots<Ghosts,AtLevel3>   ghosts3;

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------

            //! cleanup
            inline virtual ~Workspace() throw()
            {
                Coord::LDZ_(size);
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
            outer( expandInner( abs_of(ng) ) ),
            ghosts(), ghosts1(), ghosts2(), ghosts3()
            {
                std::cerr << "tile[" << this->rank << "]=" << inner << " -> " << outer << std::endl;
                buildGhosts();
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
                    // Expand per dimensionm
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


            inline void tryCreateGhosts(const_coord delta)
            {
                //--------------------------------------------------------------
                //
                // first pass: try to find the probe
                //
                //--------------------------------------------------------------
                coord probe = (inner.lower+inner.upper)/2;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D d = Coord::Of(delta,dim);
                    switch( d )
                    {
                        case -1: Coord::Of(probe,dim) = Coord::Of(inner.lower,dim)-1; break;
                        case  1: Coord::Of(probe,dim) = Coord::Of(inner.upper,dim)+1; break;
                        default:
                            break;
                    }
                }
                std::cerr << "\tlink@delta=" << delta << " : probe=" << probe << " : ";
                if(outer.has(probe))
                {
                    //----------------------------------------------------------
                    //
                    // get info
                    //
                    //----------------------------------------------------------
                    const Topology::Level glevel = Topology::LevelOf(delta);
                    const_coord           granks = Coord::Regularized(this->sizes,this->ranks + delta);

                    //----------------------------------------------------------
                    //
                    // build recv/send layouts from delta
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    //
                    // create ghosts and push them in their positions
                    //
                    //----------------------------------------------------------
                    const Ghosts  g = new Ghosts_(this->sizes,Coord::GlobalRank(this->sizes,granks),glevel);
                    std::cerr << "ghosts.ranks=" << g->ranks << " | " << g->rank << " <-- " << this->rank << std::endl;

                    ghosts.push_back(g);
                    switch (glevel)
                    {
                        case Topology::Level1:  ghosts1.push_back(g); break;
                        case Topology::Level2:  ghosts2.push_back(g); break;
                        case Topology::Level3:  ghosts3.push_back(g); break;
                    }






                }
                else
                {
                    std::cerr << "NO" << std::endl;
                }
            }

            inline void buildGhosts()
            {
                std::cerr << "links@ranks="<< this->ranks << std::endl;

                //! half loop on [-1:1]^Dimensions, using symetry
                coord __lo(0); Coord::LD(__lo,-1);
                coord __up(0); Coord::LD(__up, 1);
                Loop loop(__lo,__up);
                loop.start();

                //size_t levels[3] = { 0,0,0 };
                for( size_t j=0; j<Directions; ++j, loop.next() )
                {
                    tryCreateGhosts( loop.value);
                    tryCreateGhosts(-loop.value);
                }
                //display_int::to(std::cerr << "links={",levels,3) << "}" << std::endl;

            }

        };
        
    }
}



#endif

