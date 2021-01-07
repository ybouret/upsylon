//! \file

#ifndef Y_FITTING_VARIABLE_DISPLAY_INCLUDED
#define Y_FITTING_VARIABLE_DISPLAY_INCLUDED 1

#include "y/mkl/fitting/variables.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/ios/align.hpp"
#include "y/mkl/kernel/errors.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            //__________________________________________________________________
            //
            //
            //! display formatted variables
            //
            //__________________________________________________________________
            struct display_variables
            {
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static  const char * const             empty_separator; //!< ""
                static  const char * const             equal_separator; //!< " = "
                typedef variables::const_iterator      iterator; //!< alias
                typedef vector<string,memory::pooled>  strings;  //!< aluas


                //! os << prefix << var << equal_separator << value << suffix << '\n'
                template <typename OSTREAM, typename ARR> static inline
                OSTREAM & values(OSTREAM &os, const char *prefix, const variables &vars, ARR &arr, const char *suffix)
                {
                    if(!prefix)  prefix = empty_separator;
                    if(!suffix)  suffix = empty_separator;
                    const size_t width  = vars.max_name_length();
                    for(iterator it=vars.begin();it!=vars.end();++it)
                    {
                        const variable &v = **it;
                        const string    s = vformat("%.15g",double( v(arr) ) );
                        os << prefix << ios::align(v.name,ios::align::left,width) << equal_separator << s << suffix << '\n';
                    }
                    return os;
                }

                //! os << prefix << var << equal_separator << a << separator << b << suffix << '\n'
                template <typename OSTREAM, typename ARR, typename BRR> static inline
                OSTREAM & values(OSTREAM &os, const char *prefix, const variables &vars, ARR &arr, const char *separator, BRR &brr, const char *suffix)
                {
                    if(!prefix)    prefix = empty_separator;
                    if(!suffix)    suffix = empty_separator;
                    if(!separator) separator = empty_separator;

                    const size_t v_width  = vars.max_name_length();
                    strings      a_field(vars.size(),as_capacity);
                    const size_t a_width  = make_strings(a_field,vars,arr);
                    strings      b_field(vars.size(),as_capacity);
                    const size_t b_width  = make_strings(b_field,vars,brr);

                    size_t        j=1;
                    for(iterator it=vars.begin();it!=vars.end();++it,++j)
                    {
                        const variable &v = **it;
                        os << prefix << ios::align(v.name,ios::align::left,v_width);
                        os << equal_separator << ios::align(a_field[j],ios::align::left,a_width);
                        os << separator       << ios::align(b_field[j],ios::align::left,b_width);
                        os << suffix << '\n';
                    }
                    return os;
                }

                //! os << prefix << var << equal_separator << aorg << +/- << aerr << (%) << '\n'
                template <typename OSTREAM, typename ARR> static inline
                OSTREAM & errors(OSTREAM &os, const char *prefix, const variables &vars, ARR &aorg, const accessible<bool> &used, ARR &aerr)
                {
                    strings results(vars.size(),as_capacity);
                    format_results(results,vars,aorg,used,aerr);
                    if(!prefix) prefix = empty_separator;
                    for(size_t i=1;i<=results.size();++i)
                    {
                        os << prefix << results[i] << '\n';
                    }
                    return os;
                }


                //! format var = value +/- error (percent)
                template <typename ARR> static inline
                void format_results(sequence<string>       &results,
                                    const variables        &vars,
                                    ARR                    &aorg,
                                    const accessible<bool> &used,
                                    ARR                    &aerr)
                {
                    // initialize
                    results.free();
                    const size_t   vars_width = vars.max_name_length();
                    strings        aorg_field(aorg.size(),as_capacity);
                    const size_t   aorg_width = make_strings(aorg_field,vars,aorg);
                    strings        aerr_field(aerr.size(),as_capacity);
                    const size_t   aerr_width = make_strings(aerr_field,vars,aerr);

                    // loop over variables
                    size_t        j=1;
                    for(iterator it=vars.begin();it!=vars.end();++it,++j)
                    {
                        const variable &v = **it;
                        string output     = v.name; output.fill(' ',vars_width-v.name.size());
                        output << " = " << aorg_field[j];
                        if(v(used))
                        {
                            output.fill(' ',aorg_width-aorg_field[j].size());
                            output << " \\pm ";
                            output << aerr_field[j];
                            output.fill(' ',aerr_width-aerr_field[j].size());
                            output << ' ' << '(';
                            output << vformat("%6.2lf", errors::percent<double>(v(aerr),v(aorg)) );
                            output << '%' << ')';
                        }
                        results.push_back(output);
                    }
                }

            private:
                //! make strings from values
                template <typename ARR> static inline
                size_t make_strings(strings &results, const variables &vars, ARR &arr)
                {
                    results.free();
                    size_t width = 0;
                    for(iterator it=vars.begin();it!=vars.end();++it)
                    {
                        const variable &v = **it;
                        const string    s = vformat("%.15g",double( v(arr) ) );
                        width             = max_of(width,s.size());
                        results << s;
                    }
                    return width;
                }


            };

        }

    }

}

#endif
