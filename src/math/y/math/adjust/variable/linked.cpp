
#include "y/math/adjust/variable/linked.hpp"

namespace upsylon {
    
    namespace Adjust {
        
        LinkedVariable:: LinkedVariable(const string &n, const Variable::Pointer &l) :
        Variable(n),
        link(l)
        {
        }
        
        LinkedVariable:: LinkedVariable(const char *n, const Variable::Pointer &l) :
        Variable(n),
        link(l)
        {
        }
        
        LinkedVariable:: ~LinkedVariable() throw()
        {
        }
        
        LinkedVariable:: LinkedVariable(const LinkedVariable &other) :
        Variable(other),
        link(other.link)
        {
        }
        
        size_t LinkedVariable:: index() const throw()
        {
            return link->index();
        }
        
        
    }
    
}

