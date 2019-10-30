
//! \file

#ifndef Y_MATH_ADJUST_COMMON_INCLUDED
#define Y_MATH_ADJUST_COMMON_INCLUDED 1

#include "y/math/adjust/variables.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            class Common
            {
            public:
                const Variable::Names names;

                virtual ~Common() throw();

                void appendTo( Variables &vars ) const;

                static string Suffix(const size_t value);

            protected:
                explicit Common(const size_t);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Common);
            };

        }

    }

}

#endif

