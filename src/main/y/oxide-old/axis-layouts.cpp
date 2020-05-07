
#include "y/oxide/axis-layouts.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Oxide {

        AxisLayouts:: ~AxisLayouts() throw()
        {
        }

        AxisLayouts:: AxisLayouts(const AxisLayout &in,
                                  const AxisLayout &out) throw() :
        inner(in),
        outer(out)
        {
        }

        AxisLayouts:: AxisLayouts(const AxisLayouts &_) throw() :
        inner( _.inner ),
        outer( _.outer )
        {
        }

        void AxisLayouts:: checkSpace() const
        {
            if(inner.lower>=inner.upper)
            {
                throw exception("Squeezed Oxide::AxisLayout detected");
            }
        }
    }

}
