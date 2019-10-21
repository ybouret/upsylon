
#ifndef Y_MATH_ADJUST_VARIABLE_GLOBAL_INCLUDED
#define Y_MATH_ADJUST_VARIABLE_GLOBAL_INCLUDED 1

#include "y/math/adjust/variable.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            class GlobalVariable : public Variable
            {
            public:
                const size_t position;

                explicit GlobalVariable(const string &n, const size_t i);
                explicit GlobalVariable(const char   *n, const size_t i);
                GlobalVariable(const GlobalVariable &other);

                virtual ~GlobalVariable() throw();
                virtual  size_t index() const throw();

            private:
                Y_DISABLE_ASSIGN(GlobalVariable);
            };

        }
    }
}

#endif

