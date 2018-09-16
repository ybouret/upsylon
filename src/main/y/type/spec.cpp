
#include "y/type/spec.hpp"

namespace upsylon
{

    type_spec :: ~type_spec() throw()
    {
        
    }

    type_spec :: type_spec( const std::type_info &i ) :
    info(i),
    name( new string(info.name()) )
    {
    }

    type_spec :: type_spec( const type_spec &other ) throw() :
    info(other.info),
    name(other.name)
    {
    }


    const string & type_spec:: key() const throw()
    {
        return *name;
    }

    
}


