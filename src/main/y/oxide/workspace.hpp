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
            static const size_t                               Dimensions = HubType::Dimensions;
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
                                      const Coord1D     globalRank,
                                      const_coord      &globalPBC,
                                      const Coord1D     ng = 0
                                      ) :
            HubType(localSizes,globalRank,globalPBC),
            size(  Coord::Product(this->sizes) ),
            inner( full.split(this->sizes,this->ranks) ),
            outer( expandInner( abs_of(ng) ) )
            {
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
                    //const HubType &self = *this;
                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                    }
                    return inner;
                }
            }


        };
        
    }
}



#endif

