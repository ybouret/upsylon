
#include "y/alchemy/extent.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        Extent:: ~Extent() throw()
        {
        }
        
        Extent::Extent(const Extent &_) throw() :
        index(_.index),
        value(_.value)
        {
        }
        
        Extent:: Extent(const size_t i, const double v) throw() :
        index(i),
        value(v)
        {
        }
        
        std::ostream & operator<<(std::ostream &os, const Extent &xi)
        {
            os << "{" << xi.value << " @" << xi.index << "}";
            return os;
        }

    }
    
}

namespace upsylon
{
    namespace Alchemy
    {
        Extents:: Extents(const Extents &_) throw() :
        forward(_.forward),
        reverse(_.reverse)
        {
        }
        
        Extents:: ~Extents() throw()
        {
        }
        
        Extents:: Extents(const Extent fwd, const Extent rev) throw() :
        forward(fwd),
        reverse(rev)
        {
        }
       
        std::ostream & operator<<(std::ostream &os, const Extents &x)
        {
            os << "[fwd=" << x.forward << " | rev=" << x.reverse << "]";
            return os;
        }
        
    }
}
