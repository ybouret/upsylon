
//! \file

#ifndef Y_SPADE_GHOSTS_INCLUDED
#define Y_SPADE_GHOSTS_INCLUDED 1

#include "y/spade/ghost.hpp"

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
                            const int     peerRank = 0 ) :
            peer(peerRank),
            inner(sendLayout,fullLayout),
            outer(recvLayout,fullLayout)
            {
                assert(inner.items==outer.items);
                assert(peer>=0);
            }

            //------------------------------------------------------------------
            //
            // member
            //
            //------------------------------------------------------------------
            const int   peer;  //!< peer rank
            const Ghost inner; //!< to send
            const Ghost outer; //!< to recv


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ghosts);
        };


    }

}

#endif

