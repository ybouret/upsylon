//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED 1

#include "y/math/fit/sample/info.hpp"
#include "y/math/fit/sequential/function.hpp"
#include "y/math/fit/sequential/gradient.hpp"
#include "y/math/kernel/atom.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace math {

        namespace Fit {

            
            //==================================================================
            //
            //
            //! interface for adjustable sample
            //
            //
            //==================================================================
            template <typename T>
            class SampleType : public SampleInfo
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef typename Type<T>::Matrix     Matrix;       //!< alias
                typedef typename Type<T>::Function   Function;     //!< alias

                //==============================================================
                //
                // virtual interface
                //
                //==============================================================

                //! cleanup
                inline virtual ~SampleType() throw() {}

                //! compute least squares with a sequential function
                virtual T compute(Sequential<T> &F, const accessible<T> &aorg) const = 0;

                //! full computation with a sequential function
                virtual T  computeAndUpdate(Matrix                 &alpha,
                                            addressable<T>         &beta,
                                            Sequential<T>          &F,
                                            const accessible<T>    &aorg,
                                            const accessible<bool> &used,
                                            Gradient<T>            &grad) const = 0;

                //! flags activation
                virtual void activate(addressable<bool>      &target,
                                      const accessible<bool> &source) const = 0;

                //! computing quantities for goodness of fit
                virtual void addToSumOfSquares(T &total,
                                               T &residual) const throw() = 0;
                
                //! upper abscissa, after a ready()!
                virtual T upper() const throw() = 0;
                
                
                //! upper abscissa, after a ready()!
                virtual T lower() const throw() = 0;

                //! extraneous info
                virtual void extraneous( ios::ostream &fp, const char comment ) const = 0;

                //==============================================================
                //
                // non virtual interface
                //
                //==============================================================

                //! initialize matrix/vector from used statius
                inline void initialize(Matrix                 &alpha,
                                       addressable<T>         &beta,
                                       const accessible<bool> &used ) const throw()
                {
                    assert( alpha.rows  == alpha.cols  );
                    assert( beta.size() == alpha.rows );
                    assert( used.size() == beta.size() );
                    assert( variables.sweep() <= used.size() );

                    // assuming no one is used
                    atom::ld(beta,0); // beta=0
                    alpha.Id();       // alpha=identity

                    // cleanup used variables
                    size_t                    nv = variables.size();
                    Variables::const_iterator it = variables.begin();
                    while(nv>0)
                    {
                        const size_t i = (**it).index();
                        if( used[i] )
                        {
                            alpha[i][i] = 0;
                        }
                        --nv;
                        ++it;
                    }
                }

                //! wrapper to use a C++ function
                inline T computeD2( Function &F, const accessible<T>  &aorg ) const
                {
                    SequentialFunction<T> call(F);
                    return compute(call,aorg);
                }

                //! compute D2 overloaded
                inline T computeD2(Sequential<T>       &F,
                                   const accessible<T> &aorg) const
                {
                    return compute(F,aorg);
                }



                //! compute D2 overloaded
                inline T computeD2(Matrix                  &alpha,
                                   addressable<T>          &beta,
                                   Sequential<T>           &F,
                                   const accessible<T>     &aorg,
                                   const accessible<bool>  &used,
                                   Gradient<T>             &grad) const
                {
                    initialize(alpha,beta,used);
                    const T D2 = computeAndUpdate(alpha,beta,F,aorg,used,grad);
                    Type<T>::Regularize(alpha);
                    return D2;
                }

                //! compute coefficient of determination R2, after a computeD2
                inline T computeR2() const throw()
                {
                    T SStot = 0, SSres = 0;
                    addToSumOfSquares(SStot, SSres);
                    return T(1) - SSres/(SStot+numeric<T>::tiny);
                }

                //! write useful fit information
                void writeLog(ios::ostream           &fp,
                              const accessible<T>    &aorg,
                              const accessible<bool> &used,
                              const accessible<T>    &aerr,
                              const char              comment = '%' ) const
                {
                    static const size_t nsep = 72;
                    const Variables    &vars   = variables;
                    const size_t        nvar   = vars.size();
                    const size_t        nmax   = vars.maxNameLength();
                    const size_t        ndat   = this->count();
                    size_t              active = 0;
                    size_t              fixed  = 0;

                    fp.repeat(nsep,comment) << '\n';
                    fp("%c #data      = %u\n", comment, unsigned(ndat));
                    fp("%c #variables = %u\n", comment, unsigned(ndat));

                    Variables::Strings astr(nvar,as_capacity);
                    const size_t       apad = vars.fillStrings(astr,aorg);
                    Variables::Strings estr(nvar,as_capacity);
                    const size_t       epad = vars.fillStrings(estr,aerr);

                    for( Variables::const_iterator it=vars.begin();it!=vars.end();++it)
                    {
                        if( vars(used,(**it).name) )
                        {
                            ++active;
                        }
                        else
                        {
                            ++fixed;
                        }
                    }
                    fp("%c #fixed     = %u\n", comment, unsigned(fixed)  );
                    fp("%c #active    = %u\n", comment, unsigned(active) );
                    if(active>ndat) throw exception("too many variables for data");
                    const size_t ndof = ndat - active;
                    fp("%c #d.o.f     = %u\n", comment, unsigned(ndof) );

                    if(fixed>0)
                    {
                        fp("%c parameters\n",comment);
                        size_t ivar=1;
                        for( Variables::const_iterator it=vars.begin();it!=vars.end();++it,++ivar)
                        {
                            const string &name = (**it).name;
                            if( !vars(used,name) )
                            {
                                fp.align(name,nmax) << Variables::Equal << astr[ivar] << '\n';
                            }
                        }
                    }

                    if(active>0)
                    {
                        fp("%c variables\n",comment);
                        size_t ivar=1;
                        for( Variables::const_iterator it=vars.begin();it!=vars.end();++it,++ivar)
                        {
                            const string &name = (**it).name;
                            if( vars(used,name) )
                            {
                                fp.align(name,nmax)       <<   Variables::Equal;
                                fp.align(astr[ivar],apad) <<   Variables::PM;
                                fp.align(estr[ivar],epad) << " \\\\";
                                const T p = errors::percent(aerr[ivar], aorg[ivar]);
                                const string per = vformat("%6.2lf",double(p));
                                fp << " (" << per << "%)";
                                fp << '\n';
                            }
                        }
                    }

                    fp("%c extraneous information\n",comment);
                    this->extraneous(fp,comment);
                    fp("R2      = %.15g\n", double(this->computeR2() ) );
                    fp.repeat(nsep,comment) << '\n';

                }


            protected:
                //! setup
                inline explicit SampleType() throw() : SampleInfo() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleType);

            };

        }

    }
}


#endif

