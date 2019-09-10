
//! \file
#ifndef Y_OXIDE_WORKSPACES_INCLUDED
#define Y_OXIDE_WORKSPACES_INCLUDED 1

#include "y/oxide/workspace.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon
{
    namespace Oxide
    {

        template <typename COORD>
        class Workspaces : public slots< Workspace<COORD> >
        {
        public:
            typedef Layout<COORD>                    LayoutType;
            typedef typename LayoutType::coord       coord;
            typedef typename LayoutType::const_coord const_coord;
            typedef Workspace<COORD>                 WorkspaceType;
            typedef slots<WorkspaceType>             SlotsType;

            virtual ~Workspaces() throw() { }

            explicit Workspaces(const LayoutType &full,
                                const_coord      &localSizes,
                                const_coord      &PBC,
                                const Coord1D     ng) :
            SlotsType( Coord::Product( __Workspace::CheckLocalSizes(localSizes) ) )
            {
                const Coord1D sz = Coord1D( this->count );
                for(Coord1D rank=0;rank<sz;++rank)
                {
                    this->template build<LayoutType,coord,Coord1D,coord,Coord1D>(full,
                                                                                 localSizes,
                                                                                 rank,
                                                                                 PBC,
                                                                                 ng);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspaces);
        };

    }

}

#endif
