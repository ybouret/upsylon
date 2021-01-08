
//! \file

#ifndef Y_FITTING_BUILT_IN_POLYNOMIAL_INCLUDED
#define Y_FITTING_BUILT_IN_POLYNOMIAL_INCLUDED 1

#include "y/mkl/fitting/v-gradient.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                template <typename T>
                class polynomial
                {
                public:

                    template <typename ID> inline
                    explicit polynomial(const ID &id, const size_t degree) :
                    v(id),
                    d(degree),
                    n(d+1)
                    {
                    }

                    inline virtual ~polynomial() throw()
                    {
                        
                    }


                    const string v; //!< variable base name
                    const size_t d; //!< degree
                    const size_t n; //!< degree+1

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(polynomial);
                };

            }

        }

    }

}

#endif

