#include "y/math/adjust/variable.hpp"

namespace upsylon {
    
    namespace Adjust {
        
        Variable:: ~Variable() throw() {}
        
        Variable:: Variable(const string &id) : name(id) {}
        
        Variable:: Variable(const char *id) : name(id) {}
        
        Variable:: Variable(const Variable &other) : name(other.name) {}
        
        
        
    }
    
}

