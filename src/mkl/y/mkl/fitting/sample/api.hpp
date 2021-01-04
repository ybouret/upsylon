//! \file

#ifndef Y_FITTING_SAMPLE_API_INCLUDED
#define Y_FITTING_SAMPLE_API_INCLUDED 1

#include "y/mkl/fitting/sequential/function.hpp"
#include "y/mkl/fitting/v-gradient.hpp"
#include "y/container/matrix.hpp"

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
            class sample_api : public object, public counted
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
                virtual size_t   count()  const throw() = 0; //!< number of points
                virtual void     setup(const accessible<ORDINATE> &)                 = 0; //!< prepare for a cycle and parameters
                virtual ORDINATE D2(sequential_type &, const accessible<ORDINATE> &) = 0; //!< compute D2 and all adjusted var
                
                
                //______________________________________________________________
                //
                // non-virtual inteface
                //______________________________________________________________
                //! key for sets and intr_ptr
                inline const string &key() const throw() { return name; }
                
                //! wrapper for regular function
                inline ORDINATE D2(sequential_func &f, const accessible<ORDINATE> &a)
                {
                    sequential_function<ABSCISSA,ORDINATE> F(f);
                    return D2(F,a);
                }
                
                //! fullD2 with curvature and gradient
                inline ORDINATE fullD2(matrix<ORDINATE>           &alpha,
                                       addressable<ORDINATE>      &beta,
                                       sequential_type            &F,
                                       v_gradient_type            &G,
                                       const accessible<ORDINATE> &A,
                                       const accessible<bool>     &used)
                {
                    const ORDINATE res = _D2(alpha,beta,F,G,A,used);
                    this->regularize(alpha,used);
                    return res;
                }
                
                //! access variables
                inline variables       & operator*()        throw() { return vars; }
                
                //! access variables
                inline const variables & operator*() const throw() { return vars; }
                
                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________
                
                //! make alpha symmetric and set diagonal term to 1 for unused
                static inline void regularize(matrix<ORDINATE>       &alpha,
                                              const accessible<bool> &used)
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
                            alpha[i][i] = 1;
                        }
                    }
                }
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string   name; //!< unique identifier
                variables      vars; //!< variables to pass to objective function
                const ORDINATE zero; //!< a zero ordinate
                
                
            protected:
                //! setup
                template <typename ID>
                inline explicit sample_api( const ID &id):
                object(),
                counted(),
                name(id),
                vars(),
                zero(0)
                {
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(sample_api);
                virtual ORDINATE _D2(matrix<ORDINATE>           &alpha,
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
