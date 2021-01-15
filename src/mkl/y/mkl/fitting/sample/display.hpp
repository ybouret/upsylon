//! \file

#ifndef Y_FITTING_SAMPLE_DISPLAY_INCLUDED
#define Y_FITTING_SAMPLE_DISPLAY_INCLUDED 1

#include "y/mkl/fitting/variable/display.hpp"
#include "y/mkl/fitting/sample/api.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! display formatted output
            //
            //__________________________________________________________________
            struct display_sample
            {
                //! display/save current session results
                template <typename OSTREAM, typename ABSCISSA, typename ORDINATE> static inline
                void results(OSTREAM                             &os,
                             const sample_api<ABSCISSA,ORDINATE> &s,
                             const accessible<ORDINATE>          &aorg,
                             const accessible<bool>              &used,
                             const accessible<ORDINATE>          &aerr,
                             const char                           comm = '%')
                {
                    const variables &vars      = s.vars;
                    const size_t     line_size = 64;
                    string           comm_line(line_size,as_capacity,false);
                    string           sz;
                    comm_line.fill(comm,line_size);

                    os << comm_line << '\n';
                    os << comm << " for '" << s.name << "'\n";
                    os << comm << " #data       = " << size2text(s.count())   << '\n';
                    os << comm << " #variables  = " << size2text(vars.size()) << '\n';


                    {
                        // format all results
                        display_variables::strings out;
                        display_variables::format_results(out,vars,aorg,used,aerr);

                        // sort out used and set
                        size_t nuse = 0;
                        size_t nprm = 0;
                        for(variables::const_iterator it=vars.begin();it!=vars.end();++it)
                        {
                            if( (**it)(used) )
                            {
                                ++nuse;
                            }
                            else
                            {
                                ++nprm;
                            }
                        }

                        // display set parameters
                        os << comm << " #parameters = " << size2text(nprm)  << '\n';
                        {
                            size_t j=1;
                            for(variables::const_iterator it=vars.begin();it!=vars.end();++it,++j)
                            {
                                if( (**it)(used) ) continue;
                                os << '\t' << out[j] << '\n';
                            }
                        }

                        // display used parameters
                        os << comm << " #active     = " << size2text(nuse) << '\n';
                        {
                            size_t j=1;
                            for(variables::const_iterator it=vars.begin();it!=vars.end();++it,++j)
                            {
                                if( ! (**it)(used) ) continue;
                                os << '\t' << out[j] << '\n';
                            }
                        }
                    }

                    os << comm << " indicators\n";
                    {
                        os << '\t' << "D2   = " << display_variables::real2text( double(s.last_D2) ) << '\n';
                    }

                    {
                        correlation<ORDINATE> corr;
                        os << '\t' << "corr = " << display_variables::real2text( double(s.compute_corr(corr)) ) << '\n';
                    }

                    {
                        os << '\t' << "R2   = " << display_variables::real2text( double(s.compute_R2()) ) << '\n';
                    }

                    os << comm_line << '\n';
                }

                //! helper
                static string size2text(const size_t n);


            };
        }

    }

}

#endif

