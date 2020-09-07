#include "y/jargon/char.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Char:: ~Char() throw()
        {
            _bzset(code);
        }
        
        Char:: Char(const Context &context,
                    const uint8_t  content) throw() :
        Object(),
        inode<Char>(),
        Context(context),
        code(content)
        {
        }
        
        Char:: Char(const Char &other) throw() :
        Object(),
        inode<Char>(),
        Context(other),
        code(other.code)
        {
        }
        
        
        std::ostream & operator<<( std::ostream &os, const Char &ch)
        {
            os << cchars::visible[ch.code];
            return os;
        }
        
        
    }
    
}


