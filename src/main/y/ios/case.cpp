#include "y/ios/case.hpp"

namespace upsylon
{
    const char *CharCaseInfo(const CharCase t) throw()
    {
        switch(t)
        {
            case Lowercase: return "lowercase";
            case Uppercase: return "uppercase";
        }
        return "unknown case";
    }
    
}
