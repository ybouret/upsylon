
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
            typedef hash_set<string,shared_variable> variables_;

            class variables : public variables_
            {
            public:
                // C++
                virtual ~variables() throw();
                explicit variables();
                variables(const variables &);
                variables & operator=(const variables &);

                template <typename ID> inline
                variables &operator()(const ID &id, const size_t indx)
                {
                    const shared_variable v = new primary_variable(id,indx);
                    check_add(v);
                    return *this;
                }

                template <typename ID> inline
                variables & operator<<( const ID &id )
                {
                    (*this)(id,size()+1);
                    return *this;
                }

                template <typename ID> inline
                variables &operator()(const ID &id, const shared_variable &parent)
                {
                    const shared_variable v = new replica_variable(id,parent);
                    check_add(v);
                    return *this;
                }

                const shared_variable & operator()(const string &id) const;
                const shared_variable & operator()(const char   *id) const;

                template <typename ID> inline
                const variable & operator[](const ID &id) const
                {
                    return *(*this)(id);
                }


                template <typename ID,typename T> inline
                T & operator()(const ID &id, addressable<T> &arr) const
                {
                    return (*this)[id](arr);
                }

                template <typename ID,typename T> inline
                const T & operator()(const ID &id, const accessible<T> &arr) const
                {
                    return (*this)[id](arr);
                }

                size_t max_name_length() const throw();
                size_t sweep()           const throw();


            private:
                void check_add(const shared_variable &v);
                void local_add(const shared_variable &v);

            };

        }
    }
}

#endif


