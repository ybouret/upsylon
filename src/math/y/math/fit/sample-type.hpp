
//! \file
#ifndef Y_MATH_FIT_SAMPLE_TYPE_INCLUDED
#define Y_MATH_FIT_SAMPLE_TYPE_INCLUDED 1

#include "y/math/fit/sample-info.hpp"
#include "y/math/fit/types.hpp"
#include "y/math/stat/metrics.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            ////////////////////////////////////////////////////////////////////
            //
            //! Interface to sample with local data
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            class SampleType : public SampleInfo
            {
            public:
                //______________________________________________________________
                //
                // types
                //______________________________________________________________
                typedef typename Type<T>::Function   Function;   //!< alias
                typedef typename Type<T>::Sequential Sequential; //!< alias
                typedef typename Type<T>::Array      Array;      //!< alias
                typedef typename Type<T>::Matrix     Matrix;     //!< alias
                typedef typename Type<T>::Gradient   Gradient;   //!< alias

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                //! destructor
                inline virtual ~SampleType() throw() {}

                //! compute D2 only, for a prepared sample
                /**
                 Yf must be computed along
                 */
                virtual T computeD2(typename Type<T>::Sequential &F,
                                    const Array                  &aorg) = 0;


                //! compute D2, beta and alpha, beta and alpha initialy empty, for a prepared sample
                /**
                 this is a time consuming step, and the Gradient should be well
                 parametrized (check scaling)
                 */
                virtual T computeD2(Sequential        &F,
                                    const Array       &aorg,
                                    Array             &beta,
                                    Matrix            &alpha,
                                    Gradient          &grad,
                                    const array<bool> &used) = 0;

                //! add Sum of Squared Errors (SSE) and Sum of Square Residuals (SSR)
                virtual void add_SSE_SSR( T &SSE, T &SSR ) const = 0;

                //! compute correlation
                virtual T compute_correlation( correlation<T> &corr ) const = 0;

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                //! compute D2 only, wrapper for function
                inline T computeD2_(Function     &F,
                                    const Array  &aorg)
                {
                    typename Type<T>::SequentialFunction SF(F);
                    return computeD2(SF,aorg);
                }

                //! compute R2 after a D2 is computed
                inline T computeR2() const
                {
                    T SSE=0, SSR=0;
                    add_SSE_SSR(SSE,SSR);
                    return SSR/(SSR+SSE+numeric<T>::tiny);
                }

                //! save to logfile with optional comment char
                inline void writeLog(ios::ostream      &fp,
                                     const Array       &aorg,
                                     const Array       &aerr,
                                     const array<bool> &used,
                                     const char         com = '#') const
                {
                    static const size_t nsep = 72;

                    //__________________________________________________________
                    //
                    // compute correlation
                    //__________________________________________________________
                    T cf = 0;
                    {
                        correlation<T> corr;
                        cf = this->compute_correlation(corr);
                    }

                    //__________________________________________________________
                    //
                    // preparing strings to display
                    //__________________________________________________________
                    Variables::Strings  astr;
                    Variables::Strings  estr;
                    const size_t        am = variables.fillStrings(astr,aorg);
                    const size_t        em = variables.fillStrings(estr,aerr);
                    const size_t        nm = variables.get_max_name_size();

                    //__________________________________________________________
                    //
                    // degrees of freedom
                    //__________________________________________________________
                    const size_t  ndat = this->count();
                    long          ndof = ndat;
                    size_t        nprm = 0;
                    const  size_t nvar = variables.size();

                    for( Variables::const_iterator i=variables.begin();i!=variables.end();++i)
                    {
                        const Variable &v = **i;
                        if( used[v.check_index(used.size()) ] )
                        {
                            --ndof;
                            ++nprm;
                        }
                    }
                    assert(nprm<=nvar);

                    fp.repeat(nsep,com) << '\n';
                    //__________________________________________________________
                    //
                    // header
                    //__________________________________________________________
                    fp << com << " #data      = "; fp("%u",unsigned(ndat))       << '\n';
                    fp << com << " #parameter = "; fp("%u",unsigned(nprm))       << '\n';
                    fp << com << " #dof       = "; fp("%ld",ndof)                << '\n';
                    fp << com << "       corr = "; fp("%.15g",cf)                << '\n';
                    fp << com << "         R2 = "; fp("%.15g",this->computeR2()) << '\n';

                    //__________________________________________________________
                    //
                    // save fixed variables
                    //__________________________________________________________
                    fp << com << " <fixed variables> : "; fp("%u/%u\n",unsigned(nvar-nprm),unsigned(nvar));
                    {
                        size_t iv = 0;
                        for( Variables::const_iterator i=variables.begin();i!=variables.end();++i )
                        {
                            ++iv;
                            const Variable &v    = **i;
                            const string   &name = v.name;
                            if(variables(used,name)) continue;
                            (fp << '\t').align(name,nm) << " = " << astr[iv] << '\n';
                        }
                    }

                    //__________________________________________________________
                    //
                    // save used parameters
                    //__________________________________________________________
                    fp << com << " <used parameters> : "; fp("%u/%u\n",unsigned(nprm),unsigned(nvar));
                    {
                        size_t iv = 0;
                        for( Variables::const_iterator i=variables.begin();i!=variables.end();++i )
                        {
                            ++iv;
                            const Variable &v    = **i;
                            const string   &name = v.name;
                            if(!variables(used,name)) continue;

                            (fp << '\t').align(name,nm) << " = ";
                            fp.align(astr[iv],am)       << " \\pm ";
                            fp.align(estr[iv],em);
                            fp(" (%6.2f%%)",variables.compute_relative_error(variables(aorg,name),variables(aerr,name))) << '\n';
                        }
                    }
                    fp.repeat(nsep,com) << '\n';
                }

            protected:
                //! initialize
                inline explicit SampleType(const size_t nvar_max) :
                SampleInfo(nvar_max), rc(nvar_max,as_capacity) { }

                vector<T>    rc; //!< resources


            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleType);

            };

        }
    }

}

#endif

