//! \file

#ifndef Y_FITTING_SAMPLE_API_INCLUDED
#define Y_FITTING_SAMPLE_API_INCLUDED 1

#include "y/mkl/fitting/sample/info.hpp"
#include "y/mkl/fitting/sequential/function.hpp"
#include "y/mkl/fitting/v-gradient.hpp"
#include "y/container/matrix.hpp"
#include "y/mkl/stat/correlation.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            
            //__________________________________________________________________
            //
            //
            //! sample type definition
            /**
             - ABSCISSA: abscissa type in 'REAL^d'
             - ORDINATE: ordinate type, float or double
             */
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE >
            class sample_api : public sample_info
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef sequential<ABSCISSA,ORDINATE>               sequential_type; //!< alias
                typedef typename sequential_type::function          sequential_func; //!< alias
                typedef v_gradient<ABSCISSA,ORDINATE>               v_gradient_type; //!< alias

                //______________________________________________________________
                //
                // virtual inteface
                //______________________________________________________________
                inline virtual  ~sample_api()   throw() {}   //!< cleanup
                virtual void     setup(const accessible<ORDINATE> &)                      = 0; //!< prepare for a cycle and parameters
                virtual void     update_correlation(correlation<ORDINATE> &corr  )  const = 0; //!< add points to correlation
                virtual void     update_SSE_and_SST( ORDINATE &SSE, ORDINATE &SST ) const = 0; //!< update SSE and SST

                //______________________________________________________________
                //
                // non-virtual inteface
                //______________________________________________________________

                //! wrapper for regular function
                inline ORDINATE D2(sequential_type &F, const accessible<ORDINATE> &a)
                {
                    return D2_only(F,a);
                }

                //! wrapper for regular function
                inline ORDINATE D2(sequential_func &f, const accessible<ORDINATE> &a)
                {
                    sequential_function<ABSCISSA,ORDINATE> F(f);
                    return D2_only(F,a);
                }
                
                //!  D2 with curvature and gradient
                inline ORDINATE D2(matrix<ORDINATE>           &alpha,
                                   addressable<ORDINATE>      &beta,
                                   sequential_type            &F,
                                   v_gradient_type            &G,
                                   const accessible<ORDINATE> &A,
                                   const accessible<bool>     &used)
                {
                    const ORDINATE res = D2_full(alpha,beta,F,G,A,used);
                    regularize(alpha,beta,used);
                    return res;
                }

                //!  D2 with curvature and gradient, wrapper
                inline ORDINATE D2(matrix<ORDINATE>           &alpha,
                                   addressable<ORDINATE>      &beta,
                                   sequential_func            &f,
                                   v_gradient_type            &G,
                                   const accessible<ORDINATE> &A,
                                   const accessible<bool>     &used)
                {
                    sequential_function<ABSCISSA,ORDINATE> F(f);
                    const ORDINATE res = D2_full(alpha,beta,F,G,A,used);
                    regularize(alpha,beta,used);
                    return res;
                }



                //! compute total correlation
                inline ORDINATE compute_corr(correlation<ORDINATE> &cr) const
                {
                    cr.free();
                    update_correlation(cr);
                    return cr.compute();
                }

                //! compute R_square
                inline ORDINATE compute_R2() const
                {
                    ORDINATE SSE=zero, SST=zero, one=ORDINATE(1);
                    update_SSE_and_SST(SSE,SST);
                    return (SST>zero) ? one-SSE/SST : one;
                }

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________
                
                //! make alpha symmetric and set diagonal term to 1 for unused
                inline void regularize(matrix<ORDINATE>       &alpha,
                                       addressable<ORDINATE>  &beta,
                                       const accessible<bool> &used) const
                {
                    assert(alpha.rows ==alpha.cols);
                    assert(used.size()==alpha.rows);
                    for(size_t i=alpha.rows;i>0;--i)
                    {
                        if(used[i])
                        {
                            for(size_t j=i-1;j>0;--j)
                            {
                                alpha[j][i] = alpha[i][j];
                            }
                        }
                        else
                        {
                            alpha[i][i] = one;
                            beta[i]     = zero;
                        }
                    }
                }
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const ORDINATE zero; //!< a 0 ordinate
                const ORDINATE one;  //!< a 1 ordinate
                
            protected:
                //! setup
                template <typename ID>
                inline explicit sample_api( const ID &id):
                sample_info(id),
                zero(0),
                one(1)
                {
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(sample_api);
                virtual ORDINATE D2_only(sequential_type &, const accessible<ORDINATE> &) = 0;
                virtual ORDINATE D2_full(matrix<ORDINATE>           &alpha,
                                         addressable<ORDINATE>      &beta,
                                         sequential_type            &F,
                                         v_gradient_type            &G,
                                         const accessible<ORDINATE> &A,
                                         const accessible<bool>     &used) = 0;
            };
            
        }
        
    }
    
}


#endif
