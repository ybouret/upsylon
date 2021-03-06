//! \file


#ifndef Y_FITTING_BUILT_IN_GAUSSIANS_INCLUDED
#define Y_FITTING_BUILT_IN_GAUSSIANS_INCLUDED 1

#include "y/mkl/fitting/v-gradient.hpp"
#include "y/mkl/fitting/sequential.hpp"
#include "y/mkl/fitting/built-in/qualified.hpp"

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
                    //! base class for gaussians fits
                    //
                    //__________________________________________________________
                    class gaussians : public qualified
                    {
                    public:
                        //______________________________________________________
                        //
                        // C++
                        //______________________________________________________
                        //! setup
                        explicit gaussians(size_t num);
                        
                        //! cleanup
                        virtual ~gaussians() throw();

                        //______________________________________________________
                        //
                        // members
                        //______________________________________________________
                        const size_t count; //!< number of gaussians
                        
                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(gaussians);
                    };
                }


                //______________________________________________________________
                //
                //
                //! class for gaussians fit
                //
                //______________________________________________________________
                template <typename T>
                class gaussians : public bulk::gaussians,
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
                    explicit gaussians(const size_t num) :
                    bulk::gaussians(num)
                    {
                    }

                    //! cleanup
                    inline virtual ~gaussians() throw() {}

                    //! compute expression
                    static inline T at(const T X, const T A, const T k, const T m)
                    {
                        return A * exp_of( -square_of( k*(X-m) ));
                    }

                    //! evaluation function
                    inline T eval(const T X, const accessible<T> &aorg, const variables &vars) const
                    {
                        assert(aorg.size()>=3*count);
                        T ans = 0;
                        for(size_t i=count,j=0;i>0;--i)
                        {
                            const variable &A = vars[ names[++j] ];
                            const variable &k = vars[ names[++j] ];
                            const variable &m = vars[ names[++j] ];
                            ans += at(X,A(aorg),k(aorg),m(aorg));
                        }
                        return ans;
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(gaussians);

                    inline virtual T onStart(const T X, const accessible<T> &a,const variables &v)
                    {
                        return eval(X,a,v);
                    }

                    inline virtual T onReach(const T X, const accessible<T> &a,const variables &v)
                    {
                        return eval(X,a,v);
                    }

                    inline virtual void compute(addressable<T>         &dF,
                                                const T                 X,
                                                const accessible<T>    &aorg,
                                                const variables        &vars,
                                                const accessible<bool> &used)
                    {
                        for(size_t i=count,j=0;i>0;--i)
                        {
                            const variable &A = vars[ names[++j] ];
                            const variable &k = vars[ names[++j] ];
                            const variable &m = vars[ names[++j] ];
                            const T         dX  = (X-m(aorg));
                            const T         kdX = k(aorg) * dX;
                            const T         E   = exp_of( -square_of(kdX) );
                            if( A(used) )
                            {
                                A(dF) = E;
                            }

                            if( k(used) )
                            {
                                const T tmp = A(aorg) * kdX * dX * E;
                                k(dF) = -(tmp+tmp);
                            }

                            if( m(used) )
                            {
                                const T tmp = A(aorg) * k(aorg) * kdX * E;
                                m(dF) = tmp+tmp;
                            }
                        }
                    }
                };
                
            }
            
        }
    }
    
}

#endif
