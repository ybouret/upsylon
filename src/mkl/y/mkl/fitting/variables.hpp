
//! \file
#ifndef Y_FITTING_VARIABLES_INCLUDED
#define Y_FITTING_VARIABLES_INCLUDED 1

#include "y/mkl/fitting/variable/primary.hpp"
#include "y/mkl/fitting/variable/replica.hpp"
#include "y/associative/hash/set.hpp"
#include "y/string/tokenizer.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"


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
                static char  separator; //!< default global separator


                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~variables() throw();             //!< cleanup
                explicit variables();                     //!< setup
                variables(const variables &);             //!< copy
                variables & operator=(const variables &); //!< assign

                variables(const char   *); //!< parse variables
                variables(const string &); //!< parse variables

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
                
                //______________________________________________________________
                //
                //! transfer
                //______________________________________________________________
                template <typename TARGET, typename SOURCE> inline
                void set(TARGET &target, SOURCE &source) const
                {
                    for(const_iterator it=begin();it!=end();++it)
                    {
                        const size_t j = ***it;
                        target[j] = source[j];
                    }
                }


                //______________________________________________________________
                //
                //! set all names, separated with separator, to the same value
                //______________________________________________________________
                template <typename T, typename ID> inline
                void make(addressable<T> &arr,
                          const ID       &names,
                          const T         value) const
                {
                    const variables &self = *this;
                    tokenizer<char>  t(names);
                    while( t.next_with(separator) )
                    {
                        const string id( t.token(), t.units() );
                        self(arr,id) = value;
                    }
                }

                //! turn on specific flags, leave others untouched
                template <typename ID> inline
                void on(addressable<bool> &flags,
                        const ID          &names) const
                {
                    make(flags,names,true);
                }

                //! turn on specific flags, make other 'off'
                template <typename ID> inline
                void only_on(addressable<bool> &flags,
                             const ID          &names) const
                {
                    for(size_t i=flags.size();i>0;--i) flags[i] = false;
                    on(flags,names);
                }


                //! turn off specific flags, leave others untouched
                template <typename ID> inline
                void off(addressable<bool> &flags,
                         const ID          &names) const
                {
                    make(flags,names,false);
                }

                //! turn off specific flags, make other 'on'
                template <typename ID> inline
                void only_off(addressable<bool> &flags,
                              const ID          &names) const
                {
                    for(size_t i=flags.size();i>0;--i) flags[i] = true;
                    off(flags,names,separator);
                }

                //______________________________________________________________
                //
                //! set all  to the same value
                //______________________________________________________________
                template <typename T> inline
                void make_all(addressable<T>                     &arr,
                              typename addressable<T>::param_type value) const
                {
                    for(const_iterator it=begin();it!=end();++it)
                    {
                        (**it)(arr) = value;
                    }
                }

                //! all matching flags to true
                void on(addressable<bool> &flags);

                //! all matching flags to false
                void off(addressable<bool> &flags);

                //______________________________________________________________
                //
                // other methods
                //______________________________________________________________
                size_t max_name_length() const throw();             //!< max of name lengths
                size_t sweep()           const throw();             //!< max of index values
                size_t used_in(const accessible<bool> &used) const; //!< check used
                


            private:
                void check_add(const shared_variable &v);
                void local_add(const shared_variable &v);

            };

        }
    }
}

#endif


