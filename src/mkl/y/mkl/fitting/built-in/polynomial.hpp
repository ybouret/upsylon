
//! \file

#ifndef Y_FITTING_BUILT_IN_POLYNOMIAL_INCLUDED
#define Y_FITTING_BUILT_IN_POLYNOMIAL_INCLUDED 1

#include "y/mkl/fitting/v-gradient.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                class __polynomial
                {
                public:
                    typedef vector<string,memory::pooled> strings;

                    virtual ~__polynomial() throw();

                    template <typename ID> inline
                    explicit __polynomial(const ID &id, const size_t degree) :
                    var(id),
                    deg(degree),
                    num(deg+1),
                    ids(num,as_capacity)
                    {
                        setup();
                    }

                    

                    const string    var; //!< variable base name
                    const size_t    deg; //!< degree
                    const size_t    num; //!< degree+1
                    const strings   ids;


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(__polynomial);
                    void setup();
                };

                template <typename T>
                class polynomial : public __polynomial
                {
                public:

                    template <typename ID> inline
                    explicit polynomial(const ID &id, const size_t degree) :
                    __polynomial(id,degree)
                    {
                    }

                    inline virtual ~polynomial() throw()
                    {
                        
                    }





                private:
                    Y_DISABLE_COPY_AND_ASSIGN(polynomial);
                };

            }

        }

    }

}

#endif

