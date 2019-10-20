#include "y/math/adjust/variable.hpp"
#include "y/exceptions.hpp"

#include <cerrno>

namespace upsylon {
    
    namespace Adjust {
        
        Variable:: ~Variable() throw() {}
        
        Variable:: Variable(const string &id) : name(id) {}
        
        Variable:: Variable(const char *id) : name(id) {}
        
        Variable:: Variable(const Variable &other) : name(other.name) {}
        
        const string & Variable:: key() const throw()
        {
            return name;
        }
        
        size_t Variable:: query(const collection &source) const
        {
            const size_t idx = index();
            const size_t num = source.size();
            if(idx<=0||idx>source.size())
            {
                throw libc::exception(EDOM,"variable '%s'@%u not in [1:%u]", *name, unsigned(idx), unsigned(num) );
            }
            return idx;
        }
        
        std::ostream & operator<<( std::ostream &os, const Variable &v)
        {
            os << '<' << v.name << '@' << v.index() << '>';
            return os;
        }
        
        
    }
    
}

