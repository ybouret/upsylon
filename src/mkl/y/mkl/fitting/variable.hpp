//! \file
#ifndef Y_FITTING_VARIABLE_INCLUDED
#define Y_FITTING_VARIABLE_INCLUDED 1

#include "y/string.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            class variable : public object, public counted
            {
            public:

                // virtual interface
                virtual       ~variable()        throw();
                virtual size_t get_index() const throw() = 0;

                // non-virtual interface
                const string &key() const throw();


                template <typename T>
                T & operator()(addressable<T> &arr) const throw()
                {
                    assert( get_index()>0 ); assert( get_index()<= arr.size() );
                    return arr[ get_index() ];
                }

                template <typename T>
                const T & operator()(const accessible<T> &arr) const throw()
                {
                    assert( get_index()>0 ); assert( get_index()<= arr.size() );
                    return arr[ get_index() ];
                }

                const string name;

                friend std::ostream & operator<<(std::ostream &, const variable &);

            protected:
                template <typename ID> inline
                explicit variable(const ID &id): name(id)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(variable);
                
            };


        }

    }
}

#endif

