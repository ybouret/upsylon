#include "y/string/time.hpp"
#include <ctime>

namespace upsylon
{
    string string_time:: stamp()
    {
        Y_GIANT_LOCK();
        time_t       timer   = time(NULL);
        struct tm *  tm_info = localtime(&timer);
        char         buffer[128];
        
        memset(buffer,0,sizeof(buffer));
        strftime(buffer, sizeof(buffer)-1, "%Y-%m-%d %H:%M:%S", tm_info);
        return string(buffer);
    }
}
