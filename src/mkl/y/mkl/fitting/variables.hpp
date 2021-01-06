
//! \file
#ifndef Y_FITTING_VARIABLES_INCLUDED
#define Y_FITTING_VARIABLES_INCLUDED 1

#include "y/mkl/fitting/variable/primary.hpp"
#include "y/mkl/fitting/variable/replica.hpp"
#include "y/associative/hash/set.hpp"
#include "y/string/tokenizer.hpp"
#include "y/ios/align.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/kernel/errors.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            //! base class for variables
            typedef hash_set<string,shared_variable> variables_;

            //__________________________________________________________________
            //
            //
            //! set of variables
            //
            //__________________________________________________________________
            class variables : public variables_
            {
            public:
                //______________________________________________________________
                //
                // definition
                //______________________________________________________________
                static const char default_separator = ':'; //!< for groups of variables


                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~variables() throw();             //!< cleanup
                explicit variables();                     //!< setup
                variables(const variables &);             //!< copy
                variables & operator=(const variables &); //!< assign

                //______________________________________________________________
                //
                //! create primary variables
                //______________________________________________________________
                template <typename ID> inline
                variables &operator()(const ID &id, const size_t indx)
                {
                    const shared_variable v = new primary_variable(id,indx);
                    check_add(v);
                    return *this;
                }

                //______________________________________________________________
                //
                //! append primary variable
                //______________________________________________________________
                template <typename ID> inline
                variables & operator<<( const ID &id )
                {
                    (*this)(id,size()+1);
                    return *this;
                }

                //______________________________________________________________
                //
                //! create replica variable
                //______________________________________________________________
                template <typename ID> inline
                variables &operator()(const ID &id, const shared_variable &parent)
                {
                    const shared_variable v = new replica_variable(id,parent);
                    check_add(v);
                    return *this;
                }

                //______________________________________________________________
                //
                //! access by id
                //______________________________________________________________
                const shared_variable & operator()(const string &id) const;

                //______________________________________________________________
                //
                //! access by id, wrapper
                //______________________________________________________________
                const shared_variable & operator()(const char   *id) const;

                //______________________________________________________________
                //
                //! direct variable access
                //______________________________________________________________
                template <typename ID> inline
                const variable & operator[](const ID &id) const
                {
                    const variables &self = *this;
                    return * self(id);
                }

                bool has(const string &id) const throw(); //!< query name
                bool has(const char   *id) const;         //!< query name

                //______________________________________________________________
                //
                //! access item by name
                //______________________________________________________________
                template <typename T,typename ID> inline
                T & operator()(addressable<T> &arr, const ID &id) const
                {
                    return (*this)[id](arr);
                }

                //______________________________________________________________
                //
                //! access const item by name
                //______________________________________________________________
                template <typename T,typename ID> inline
                const T & operator()(const accessible<T> &arr, const ID &id) const
                {
                    return (*this)[id](arr);
                }

                //! set all names, separated with separator, to the same value
                template <typename T, typename ID> inline
                void make(addressable<T> &arr,
                          const ID       &names,
                          const T         value,
                          const char      separator = default_separator)
                {
                    variables      &self = *this;
                    tokenizer<char> t(names);
                    while( t.next_with(separator) )
                    {
                        const string id( t.token(), t.units() );
                        self(arr,id) = value;
                    }
                }

                //! turn on specific flags, leave others untouched
                template <typename ID> inline
                void on(addressable<bool> &flags,
                        const ID          &names,
                        const char         separator = default_separator )
                {
                    make(flags,names,true,separator);
                }

                //! turn on specific flags, make other 'off'
                template <typename ID> inline
                void only_on(addressable<bool> &flags,
                             const ID          &names,
                             const char         separator = default_separator )
                {
                    for(size_t i=flags.size();i>0;--i) flags[i] = false;
                    make(flags,names,true,separator);
                }


                //! turn off specific flags, leave others untouched
                template <typename ID> inline
                void off(addressable<bool> &flags,
                         const ID          &names,
                         const char         separator = default_separator )
                {
                    make(flags,names,false,separator);
                }

                //! turn off specific flags, make other 'on'
                template <typename ID> inline
                void only_off(addressable<bool> &flags,
                              const ID          &names,
                              const char         separator = default_separator )
                {
                    for(size_t i=flags.size();i>0;--i) flags[i] = true;
                    make(flags,names,false,separator);
                }



                //______________________________________________________________
                //
                // other methods
                //______________________________________________________________
                size_t max_name_length() const throw(); //!< max of name lengths
                size_t sweep()           const throw(); //!< max of index values

                //! display var = value
                template <typename ARRAY>
                std::ostream & display( std::ostream &os, ARRAY &arr, const char *pfx ) const
                {
                    const size_t width = max_name_length();
                    if(!pfx) pfx="";
                    for(const_iterator it=begin();it!=end();++it)
                    {
                        const variable &v = **it;
                        os << pfx << ios::align(v.name,ios::align::left,width) << " = " << v(arr) << std::endl;
                    }
                    return os;
                }

                //! values -> strings, return max length
                template <typename ARRAY>   inline
                size_t to_strings(sequence<string> &strings, ARRAY &arr) const
                {
                    size_t ans = 0;
                    strings.free();
                    for(const_iterator it=begin();it!=end();++it)
                    {
                        const variable &v = **it;
                        string          s = vformat("%.15g",v(arr));
                        ans = max_of(ans,s.size());
                        strings.push_back(s);
                    }
                    return ans;
                }

                //! name = pfx arr sep brr sfx
                template <typename ARR, typename BRR>
                std::ostream & display(std::ostream &os,
                                       ARR          &arr,
                                       BRR          &brr,
                                       const char   *sep,
                                       const char   *pfx,
                                       const char   *sfx) const
                {
                    assert(sep!=NULL);
                    const size_t width = max_name_length();
                    if(!pfx) pfx="";
                    if(!sfx) sfx="";
                    vector<string>  data(arr.size(),as_capacity);
                    const size_t    alen   = to_strings(data,arr);
                    size_t          j      = 1;
                    for(const_iterator it=begin();it!=end();++it,++j)
                    {
                        const variable &v = **it;
                        os << pfx;
                        os << ios::align(v.name,ios::align::left,width);
                        os << " = ";
                        os << ios::align(data[j],ios::align::left,alen);
                        os << sep;
                        os << v(brr);
                        os << sfx;
                        os << std::endl;
                    }
                    return os;
                }

                //! format var = value +/- error (percent)
                template <typename ARR>
                inline void format_results(sequence<string>       &results,
                                           ARR                    &aorg,
                                           const accessible<bool> &used,
                                           ARR                    &aerr) const
                {
                    // initialize
                    const size_t width = max_name_length();
                    results.free();

                    // prepare fields
                    vector<string> aorg_field(aorg.size(),as_capacity);
                    const size_t   aorg_width = to_strings(aorg_field,aorg);
                    vector<string> aerr_field(aerr.size(),as_capacity);
                    const size_t   aerr_width = to_strings(aerr_field,aerr);

                    // loop over variables
                    size_t         j=1;
                    for(const_iterator it=begin();it!=end();++it,++j)
                    {
                        const variable &v = **it;
                        string output     = v.name; output.fill(' ',width-v.name.size());
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

                template <typename OSTREAM, typename ARR>
                inline void display(OSTREAM                &os,
                                    ARR                    &aorg,
                                    const accessible<bool> &used,
                                    ARR                    &aerr,
                                    const char             *pfx = NULL) const
                {
                    if(!pfx) pfx="";
                    vector<string> results;
                    format_results(results,aorg,used,aerr);
                    for(size_t i=1;i<=results.size();++i)
                    {
                        os << pfx << results[i] << '\n';
                    }
                }


            private:
                void check_add(const shared_variable &v);
                void local_add(const shared_variable &v);

            };

        }
    }
}

#endif


