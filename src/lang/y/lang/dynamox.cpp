#include "y/lang/dynamox.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoX::DynamoX(const char  *data_name,
                         const void  *data_addr,
                         const size_t data_size,
                         const void  *terminals_data,
                         const size_t terminals_size,
                         const void  *internals_data,
                         const size_t internals_size) :
        Dynamo(data_name,data_addr,data_size),
        terminalHash(terminals_data,terminals_size),
        internalHash(internals_data,internals_size)
        {
            std::cerr << name << std::endl;
            std::cerr << " #terminalsNode=" << terminalHash.nodes << std::endl;
            std::cerr << " #internalsNode=" << internalHash.nodes << std::endl;

        }

        DynamoX:: ~DynamoX() throw()
        {
        }
        
    }

}

