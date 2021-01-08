
//! \file

#ifndef Y_FITTING_BUILT_IN_POLYNOMIAL_INCLUDED
#define Y_FITTING_BUILT_IN_POLYNOMIAL_INCLUDED 1

#include "y/mkl/fitting/v-gradient.hpp"
#include "y/mkl/fitting/sequential.hpp"
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
                    explicit __polynomial(const ID &id, const size_t d) :
                    var_id(id),
                    degree(d),
                    coeffs(degree+1),
                    vnames(coeffs,as_capacity)
                    {
                        setup();
                    }

                    void make(variables &vars) const;

                    const string    var_id; //!< variable base name
                    const size_t    degree; //!< degree
                    const size_t    coeffs; //!< degree+1
                    const strings   vnames; //!< var_id[0...degree]


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(__polynomial);
                    void setup();
                };

                template <typename T>
                class polynomial : public __polynomial,
                public sequential<T,T>
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


                    inline T eval(const T X, const accessible<T> &aorg, const variables &vars) const
                    {
                        size_t n = coeffs;
                        T      p = vars(aorg,vnames[n--]);
                        while(n>0)
                        {
                            p *= X;
                            p += vars(aorg,vnames[n--]);
                        }
                        return p;
                    }


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(polynomial);
                    
                    inline virtual T onStart(const T X, const accessible<T> &a,const variables &v)
                    {
                        return eval(X,a,v);
                    }
                    
                    inline virtual T onReach(const T X, const accessible<T> &a,const variables &v)
                    {
                        return eval(X,a,v);
                    }
                };

            }

        }

    }

}

#endif

