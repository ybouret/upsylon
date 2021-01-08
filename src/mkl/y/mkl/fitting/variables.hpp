
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


