
//! \file

#ifndef Y_SPADE_GHOSTS_INCLUDED
#define Y_SPADE_GHOSTS_INCLUDED 1

#include "y/spade/layout/ghost.hpp"
#include "y/spade/layout.hpp"

namespace upsylon {

    namespace Spade
    {

        //----------------------------------------------------------------------
        //
        //! pair of ghosts
        //
        //----------------------------------------------------------------------
        class Ghosts
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------

            //! cleanup
            virtual ~Ghosts() throw();

            //! setup from computed zone in a full layout
            template <typename LAYOUT> inline
            explicit Ghosts(const LAYOUT &sendLayout,
                            const LAYOUT &recvLayout,
                            const LAYOUT &fullLayout,
                            const size_t  peerRank = 0 ) :
            peer(peerRank),
            innerGhost(sendLayout,fullLayout),
            outerGhost(recvLayout,fullLayout)
            {
                assert(innerGhost.items==outerGhost.items);
                assert(peer>=0);
            }

            //------------------------------------------------------------------
            //
            // member
            //
            //------------------------------------------------------------------
            const size_t peer;       //!< peer rank
            const Ghost  innerGhost; //!< to send
            const Ghost  outerGhost; //!< to recv
            
           

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ghosts);
        };

       
    }

}

#endif

