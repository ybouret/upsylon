
//! \file

#ifndef Y_SPADE_SWAPS_INCLUDED
#define Y_SPADE_SWAPS_INCLUDED 1

#include "y/spade/ghosts.hpp"
#include "y/spade/layout.hpp"

namespace upsylon {
    
    namespace Spade
    {
        template <typename COORD>
        class Swaps : public Ghosts
        {
        public:
            typedef Layout<COORD> LayoutType;
            
            inline virtual ~Swaps() throw()
            {
            }
            
            template <typename LAYOUT> inline
            explicit Swaps(const LayoutType &sendLayout,
                           const LayoutType &recvLayout,
                           const LayoutType &fullLayout,
                           const int         peerRank) :
            Ghosts(sendLayout,recvLayout,fullLayout,peerRank),
            innerRange(sendLayout),
            outerRange(recvLayout)
            {
                
            }
            
            const LayoutType innerRange;
            const LayoutType outerRange;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Swaps);
        };
    }
    
}

#endif

