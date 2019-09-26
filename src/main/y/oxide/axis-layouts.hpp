//! \file
#ifndef Y_OXIDE_AXIS_LAYOUTS_INCLUDED
#define Y_OXIDE_AXIS_LAYOUTS_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon {

    namespace Oxide {

        typedef Layout1D AxisLayout;

        class AxisLayouts
        {
        public:
            const AxisLayout inner;
            const AxisLayout outer;

            explicit AxisLayouts(const AxisLayout &in, const AxisLayout &out ) throw();
            virtual ~AxisLayouts() throw();
            AxisLayouts(const AxisLayouts &) throw();

            //! assume field1d with same metrics
            template <typename FIELD, typename T>
            void mapBoundaries(FIELD &axis,
                               const T lowerValue,
                               const T upperValue) const throw()
            {
                checkSpace();
                const unit_t i0 = inner.lower;
                const unit_t i1 = inner.upper;
                const unit_t di = i1-i0;
                const T      dv = upperValue-lowerValue;
                for(unit_t i=outer.lower;i<=outer.upper;++i)
                {
                    axis[i] = lowerValue + (dv*T(i-i0))/di;
                }
                axis[i0] = lowerValue;
                axis[i1] = upperValue;
            }

        private:
            Y_DISABLE_ASSIGN(AxisLayouts);
            void checkSpace() const;
        };

    }

}

#endif

