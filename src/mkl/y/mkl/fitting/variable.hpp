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

            //__________________________________________________________________
            //
            //
            //! base class for a variable
            //
            //__________________________________________________________________
            class variable : public object, public counted
            {
            public:
                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual       ~variable()        throw();      //!< cleanup
                virtual size_t operator*() const throw() = 0;  //!< final index

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                const string &key() const throw();           //!< key for sets

                //! get item in addressable
                template <typename T>
                T & operator()(addressable<T> &arr) const throw()
                {
                    assert( **this>0 ); assert( **this <= arr.size() );
                    return arr[ **this ];
                }

                //! get item in accessible
                template <typename T>
                const T & operator()(const accessible<T> &arr) const throw()
                {
                    assert( **this>0 ); assert( **this <= arr.size() );
                    return arr[ **this ];
                }



                //! display 'name' and get_index()
                friend std::ostream & operator<<(std::ostream &, const variable &);


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string name; //!< unique name


            protected:
                //! setup with name
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

