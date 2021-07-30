
#include "y/alchemy/extent.hpp"

namespace upsylon
{
    namespace alchemy
    {

        extent:: ~extent() throw()
        {
        }
        
        extent::extent(const extent &_) throw() :
        index(_.index),
        value(_.value)
        {
        }
        
        extent:: extent(const size_t i, const double v) throw() :
        index(i),
        value(v)
        {
        }
        
        std::ostream & operator<<(std::ostream &os, const extent &xi)
        {
            os << "{" << xi.value << " @" << xi.index << "}";
            return os;
        }

    }
    
}

namespace upsylon
{
    namespace alchemy
    {
        extents:: extents(const extents &_) throw() :
        forward(_.forward),
        reverse(_.reverse)
        {
        }
        
        extents:: ~extents() throw()
        {
        }
        
        extents:: extents(const extent fwd, const extent rev) throw() :
        forward(fwd),
        reverse(rev)
        {
        }
       
        std::ostream & operator<<(std::ostream &os, const extents &x)
        {
            os << "[fwd=" << x.forward << " | rev=" << x.reverse << "]";
            return os;
        }
        
    }
}
