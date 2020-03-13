
#include "y/utest/sizeof.hpp"
#include <cstring>

namespace upsylon {

    size_t __sizeof::width = 40;

    std::ostream & __sizeof::display(std::ostream &os, const char *type, const size_t value )
    {
        assert(type);
        const size_t len = (NULL != type) ? strlen(type) : 0;
        os << "sizeof" << '(';
        for(size_t i=0;i<len;++i) os << type[i];
        os << ')';
        for(size_t i=len;i<width;++i) os << ' ';
        os << " = " << value;
        return os;
    }

}
