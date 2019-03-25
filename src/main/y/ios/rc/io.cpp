#include "y/ios/rc/io.hpp"

namespace upsylon
{
    namespace ios
    {

#define Y_RC_CTOR() name(filename), hash(), verbose(v)
        
        rc:: io:: io( const string &filename, const bool v) :
        Y_RC_CTOR()
        {
        }

        rc:: io:: io( const char *filename, const bool v ) :
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
        
        std::ostream & operator<<( std::ostream &os, const rc::item &it )
        {
            os << "@" << it.start << "+" << it.bytes << " : <" << it.label << ">";
            return os;
        }

    }

}
