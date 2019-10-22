//! \file

#ifndef Y_MATH_ADJUST_VARIABLE_GLOBAL_INCLUDED
#define Y_MATH_ADJUST_VARIABLE_GLOBAL_INCLUDED 1

#include "y/math/adjust/variable.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //! a variable with a given position for Parameters
            class GlobalVariable : public Variable
            {
            public:
                const size_t position; //!< fixed position

                explicit GlobalVariable(const string &n, const size_t i); //!< setup
                explicit GlobalVariable(const char   *n, const size_t i); //!< setup
                virtual ~GlobalVariable() throw();                        //!< cleanup
                GlobalVariable(const GlobalVariable &other);              //!< copy
                virtual  size_t index() const throw();                    //!< return position

            private:
                Y_DISABLE_ASSIGN(GlobalVariable);
            };

        }
    }
}

#endif

