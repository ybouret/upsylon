#include "y/spade/dispatcher.hpp"

namespace upsylon {
 
    namespace Spade {
        
        Dispatcher:: ~Dispatcher() throw()
        {
        }
        
        Dispatcher:: Dispatcher( const comms::topology where) :
        topology(where),
        delivery(comms::computed_block_size),
        IO( ios::conveyors::instance() )
        {
        }
        
        void Dispatcher:: asyncInitialize(Block &block) throw()
        {
            block.free();
            aliasing::_(delivery) = comms::computed_block_size;
        }

        
    }
    
}
