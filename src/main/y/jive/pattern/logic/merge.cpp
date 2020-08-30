
#include "y/jive/pattern/logic/all.hpp"
#include "y/os/error.hpp"

namespace upsylon
{
    
    namespace Jive
    {
        void Logical:: mergeSameUUID() throw()
        {
            const uint32_t key = uuid;
            Operands       ops;
            while(size)
            {
                Pattern *op = pop_front();
                if(key==op->uuid)
                {
                    switch(key)
                    {
                        case Or ::UUID:  ops.merge_back( * op->as<Or>()   );  break;
                        case And::UUID:  ops.merge_back( * op->as<And>()   ); break;
                        case None::UUID: ops.merge_back( * op->as<None>() );  break;
                        default:
                            fatal_error("Jive:Logical: corrupted uuid!!!");
                            break;
                    }
                    delete op;
                }
                else
                {
                    ops.push_back(op);
                }
            }
            swap_with(ops);
        }
    }
    
}
