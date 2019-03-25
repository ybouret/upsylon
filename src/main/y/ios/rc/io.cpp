#include "y/ios/rc/io.hpp"

namespace upsylon
{
    namespace ios
    {

#define Y_RC_CTOR() name(filename), hash()
        
        rc:: io:: io( const string &filename ) :
        Y_RC_CTOR()
        {
        }

        rc:: io:: io( const char *filename ) :
        Y_RC_CTOR()
        {
        }

        rc::io:: ~io() throw()
        {
        }
        
        
    }
    
}



namespace upsylon
{
    namespace ios
    {

        rc:: item:: ~item() throw() {}
        const string & rc::item::key() const throw() { return label; }

        rc::item:: item( const string &id, const offset_t at, const size_t sz ) :
        label(id),
        start(at),
        bytes(sz)
        {
        }
        


    }

}
