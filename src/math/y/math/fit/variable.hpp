//! \file
#ifndef Y_MATH_FIT_VARIABLE_INCLUDED
#define Y_MATH_FIT_VARIABLE_INCLUDED 1

#include "y/string.hpp"
#include "y/associative/set.hpp"
#include "y/memory/pooled.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            ////////////////////////////////////////////////////////////////////
            //
            //! Variable Interface
            //
            ////////////////////////////////////////////////////////////////////
            class Variable : public counted_object
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________

                //! type of variables
                enum Type
                {
                    IsGlobal, //!< attached to global parameters
                    IsLocal   //!< attached to a global variable for local fit
                };
                typedef memory::pooled                       Memory;     //!< handling memory
                typedef intr_ptr<string,Variable>            Pointer;    //!< shared pointer
                typedef key_hasher<string>                   KeyHasher;  //!< for database
                typedef set<string,Pointer,KeyHasher,Memory> Set;        //!< database type

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const   string &key()   const throw();     //!< name
                virtual size_t  index() const throw() = 0; //!< global index
                virtual        ~Variable() throw();        //!< destructor

                //! check index() is valid against a sequence size
                size_t          check_index( const size_t against_size) const;

                //! display
                friend std::ostream & operator<<( std::ostream &, const Variable &);

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const   string  name;                      //!< unique name
                const   Type    type;                      //!< keep track

            protected:
                //! constructor
                explicit Variable(const string &__name,
                                  const Type    __type);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Variable);
            };
        }

    }

}

#endif

