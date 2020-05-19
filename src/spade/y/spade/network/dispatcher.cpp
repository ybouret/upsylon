#include "y/spade/network/dispatcher.hpp"

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

        void Dispatcher:: updateDelivery(const ios::conveyor &io) const throw()
        {
            switch(io.mode)
            {
                case comms::flexible_block_size: aliasing::_(delivery) = comms::flexible_block_size; break;
                case comms::computed_block_size: break;
            }
        }
        
    }
    
}
