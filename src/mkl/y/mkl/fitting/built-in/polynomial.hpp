
//! \file

#ifndef Y_FITTING_BUILT_IN_POLYNOMIAL_INCLUDED
#define Y_FITTING_BUILT_IN_POLYNOMIAL_INCLUDED 1

#include "y/mkl/fitting/v-gradient.hpp"
#include "y/mkl/fitting/sequential.hpp"
#include "y/mkl/fitting/built-in/qualified.hpp"
#include "y/core/ipower.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                namespace bulk
                {
                    //__________________________________________________________
                    //
                    //
                    //! base class for polynomial fits
                    //
                    //__________________________________________________________
                    class polynomial : public qualified
                    {
                    public:
                        //______________________________________________________


                        //______________________________________________________
                        //
                        // C++
                        //______________________________________________________

                        //! cleanup
                        virtual ~polynomial() throw();

                        //! setup with base id
                        template <typename ID> inline
                        explicit polynomial(const ID &id, const size_t d) :
                        qualified(d+1),
                        var_id(id),
                        degree(d),
                        coeffs(degree+1)
                        {
                            setup();
                        }

                        //______________________________________________________
                        //
                        // members
                        //______________________________________________________
                        const string    var_id; //!< variable base name
                        const size_t    degree; //!< degree
                        const size_t    coeffs; //!< degree+1


                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(polynomial);
                        void setup();
                    };
                }
                
                //______________________________________________________________
                //
                //
                //! class for polynomial fit
                //
                //______________________________________________________________
                template <typename T>
                class polynomial : public bulk::polynomial,
                public sequential<T,T>,
                public v_gradient<T,T>
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    typedef sequential<T,T> sequential_type; //!< alias
                    typedef v_gradient<T,T> v_gradient_type; //!< alias

                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    //! setup
                    template <typename ID> inline
                    explicit polynomial(const ID &id, const size_t degree) :
                    bulk::polynomial(id,degree)
                    {
                    }

                    //! cleanup
                    inline virtual ~polynomial() throw()
                    {
                        
                    }

                    //! evaluation function
                    inline T eval(const T X, const accessible<T> &aorg, const variables &vars) const
                    {
                        size_t n = coeffs;
                        T      p = vars(aorg,names[n--]);
                        while(n>0)
                        {
                            p *= X;
                            p += vars(aorg,names[n--]);
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
                    
                    inline virtual void compute(addressable<T>         &dFdA,
                                                const T                 X,
                                                const accessible<T>    &,
                                                const variables        &vars,
                                                const accessible<bool> &used)
                    {
                        for(size_t i=1,d=0;i<=coeffs;++i,++d)
                        {
                            const string   &vid = names[i];
                            const variable &var = vars[vid];
                            if(var(used))
                            {
                                var(dFdA) = ipower<T>(X,d);
                            }
                        }
                    }
                    
                };

            }

        }

    }

}

#endif

