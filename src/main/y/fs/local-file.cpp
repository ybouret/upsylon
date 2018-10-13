#include "y/fs/local-file.hpp"

namespace upsylon
{
    namespace ios
    {
        local_file:: ~local_file() throw()
        {
            switch(type)
            {
                case is_regular:
                case is_pipe: {
                    const int ans = descriptor::close(handle);
                    if(status) *status = ans;
                } break;
                default:
                    break;
            }
            handle = descriptor::invalid();
        }
    }

}
