//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/topology.hpp"
#include "y/memory/static-slots.hpp"

namespace upsylon
{
    namespace Oxide
    {

        
        //! workspace metrics
        template <typename COORD>
        class Workspace : public Topology::Node<COORD>
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
            static  const size_t                              Dimensions = Coord::Get<COORD>::Dimensions; //!< alias
            static  const size_t                              LocalNodes = Coord::Get<COORD>::LocalNodes; //!< alias
            static  const size_t                              Neighbours = Coord::Get<COORD>::Neighbours; //!< alias
            static  const size_t                              Directions = Neighbours/2;
            typedef Topology::Node<COORD>                     NodeType;

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const size_t     size;  //!< product of sizes
            const LayoutType inner; //!< inner layout
            const LayoutType outer; //!< outer layout

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
                                      const size_t      globalRank) :
            NodeType(localSizes,globalRank),
            size(  Coord::Product(this->sizes) ),
            inner( full.split(this->sizes,this->ranks) ),
            outer( inner )
            {
                buildLinks();
            }
            
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);

            void buildLinks()
            {
                coord           direction[LocalNodes];
                Topology::Level level[LocalNodes];
                {
                    coord lo(0); Coord::LD(lo,-1);
                    coord up(0); Coord::LD(up, 1);
                    Loop loop(lo,up);
                    for( loop.start(); loop.valid(); loop.next() )
                    {
                        const size_t j = loop.index-1;
                        direction[j]   = loop.value;
                        level[j]       = Topology::LevelOf(loop.value);
                    }
                }

                size_t n1 = 0;
                size_t n2 = 0;
                size_t n3 = 0;
                for(size_t i=0,j=LocalNodes;i<Directions;++i)
                {
                    --j;
                    Topology::CheckSameLevels(level[i],level[j]);
                   // const coord &lo = direction[i];
                    // const coord &up = direction[j];
                    switch (level[i])
                    {
                        case Topology::Level1: ++n1; break;
                        case Topology::Level2: ++n2; break;
                        case Topology::Level3: ++n3; break;
                        default: break;
                    }
                }
                std::cerr << "n1=" << n1 << ", n2=" << n2 << ", n3=" << n3 << std::endl;
                
            }
        };
        
    }
}



#endif

