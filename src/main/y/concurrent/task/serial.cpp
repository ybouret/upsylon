#include "y/concurrent/task/serial.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {

        serial:: ~serial() throw()
        {
        }
        
        serial:: serial() throw()
        {
        }

        job::uuid serial:: enqueue( const job::type &J )
        {
            aliasing::_(J)(access);
            return ++jid;
        }
    }
}
