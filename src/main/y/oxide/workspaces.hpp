
//! \file
#ifndef Y_OXIDE_WORKSPACES_INCLUDED
#define Y_OXIDE_WORKSPACES_INCLUDED 1

#include "y/oxide/workspace.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! multiple workspaces for threading or MPI simulation
        template <typename COORD>
        class Workspaces : public slots< Workspace<COORD> >
        {
        public:
            typedef Layout<COORD>                    LayoutType;     //!< alias
            typedef typename LayoutType::coord       coord;          //!< alias
            typedef typename LayoutType::const_coord const_coord;    //!< alias
            typedef Workspace<COORD>                 WorkspaceType;  //!< alias
            typedef slots<WorkspaceType>             SlotsType;      //!< alias
            
            //! cleanup
            virtual ~Workspaces() throw() { }
            
            //! setup all sub-workspaces
            explicit Workspaces(const LayoutType &fullLayout,
                                const_coord      &localSizes,
                                const_coord      &boundaries,
                                const Coord1D     ghostsZone) :
            SlotsType( Coord::Product( __Workspace::CheckLocalSizes(localSizes) ) )
            {
                SlotsType    &self       = *this;
                const Coord1D globalSize = Coord1D( this->count );
                for(Coord1D globalRank=0;globalRank<globalSize;++globalRank)
                    self.template build<LayoutType,coord,Coord1D,coord,Coord1D>(fullLayout,
                                                                                localSizes,
                                                                                globalRank,
                                                                                boundaries,
                                                                                ghostsZone);
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspaces);
        };
        
    }
    
}

#endif
