
//! \file
#ifndef Y_FITTING_VARIABLES_INCLUDED
#define Y_FITTING_VARIABLES_INCLUDED 1

#include "y/mkl/fitting/variable/primary.hpp"
#include "y/mkl/fitting/variable/replica.hpp"
#include "y/associative/hash/set.hpp"

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
                    return *(*this)(id);
                }

                //______________________________________________________________
                //
                //! access item by name
                //______________________________________________________________
                template <typename ID,typename T> inline
                T & operator()(const ID &id, addressable<T> &arr) const
                {
                    return (*this)[id](arr);
                }

                //______________________________________________________________
                //
                //! access const item by name
                //______________________________________________________________
                template <typename ID,typename T> inline
                const T & operator()(const ID &id, const accessible<T> &arr) const
                {
                    return (*this)[id](arr);
                }

                //______________________________________________________________
                //
                // other methods
                //______________________________________________________________
                size_t max_name_length() const throw(); //!< max of name lengths
                size_t sweep()           const throw(); //!< max of index values


            private:
                void check_add(const shared_variable &v);
                void local_add(const shared_variable &v);

            };

        }
    }
}

#endif


