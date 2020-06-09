//! \file

#ifndef Y_MKL_FIT_VARIABLE_GLOBAL_INCLUDED
#define Y_MKL_FIT_VARIABLE_GLOBAL_INCLUDED 1

#include "y/mkl/fit/variable.hpp"

namespace upsylon {

    namespace mkl {

        namespace Fit {

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

