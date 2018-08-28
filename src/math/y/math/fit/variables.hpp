//! \file
#ifndef Y_MATH_FIT_VARIABLES_INCLUDED
#define Y_MATH_FIT_VARIABLES_INCLUDED 1

#include "y/string.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/array.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {


            //! Variable Interface
            class Variable : public counted_object
            {
            public:
                //! type of variables
                enum Type
                {
                    IsGlobal, //!< attached to global parameters
                    IsLocal   //!< attached to a global variable for local fit
                };
                typedef intr_ptr<string,Variable>   Pointer; //!< shared pointer
                typedef set<string,Pointer>         Set;     //!< for database

                const   string  name; //!< unique name
                const   Type    type; //!< keep track
                const   string &key()   const throw();     //!< name
                virtual size_t  index() const throw() = 0; //!< global index
                virtual ~Variable() throw(); //!< desctrutor

                //! display
                inline friend std::ostream & operator<<( std::ostream &os, const Variable &var )
                {
                    os << var.name << '@' << var.index();
                    return os;
                }

                //! check index() is valid
                size_t check_index( const size_t against_size ) const;

            protected:
                //! constructor
                explicit Variable(const string &__name,
                                  const Type    __type);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Variable);
            };

            //! a global,top level variable
            class GlobalVariable : public Variable
            {
            public:
                //! constructor, index must be > 0
                explicit GlobalVariable(const string &__name,
                                        const size_t  __indx);
                virtual ~GlobalVariable() throw();    //!< destructor
                virtual size_t index() const throw(); //!< return indx

                const size_t indx; //!< global index

            private:
                Y_DISABLE_COPY_AND_ASSIGN(GlobalVariable);
            };

            //! a local variable, linked to another variable
            class LocalVariable : public Variable
            {
            public:
                //! constructor, attach to a link
                explicit LocalVariable( const string & __name, const Variable::Pointer &__link );
                //! desctrutor
                virtual ~LocalVariable() throw();

                const Variable::Pointer link;         //!< the link
                virtual size_t index() const throw(); //!< return link->index()

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LocalVariable);
            };

            //! Variables management
            class Variables : public Variable::Set
            {
            public:
                //! constructor, may reserve memory
                Variables(const size_t n=0);
                //! destructor
                virtual ~Variables() throw();
                //! copy
                Variables(const Variables &);
                //!assign
                Variables & operator=( const Variables &other );

                //! create a global variable, assigning current size+1 as index
                Variables        & create_global(const string &name);
                //! create a global variable, wrapper
                inline Variables & create_global(const char *name)
                {
                    const string __name = name;
                    return create_global(__name);
                }

                //! quick create a global variable
                inline Variables & operator<<( const string &name) { return create_global(name); }
                //! quick create a global variable
                inline Variables & operator<<( const char   *name) { return create_global(name); }

                //! access
                const Variable::Pointer & operator[](const string &name) const;
                //! access
                inline const Variable::Pointer & operator[](const char *name) const
                {
                    const string _ = name; return (*this)[_];
                }

                //! create a global variable
                inline Variables & operator()(const string &name) { return create_global(name); }
                //! create a global variable
                inline Variables & operator()(const char   *name) { return create_global(name); }
                //! create a local variable
                Variables        & operator()(const string &name, const Variable::Pointer &link);
                //! create a local variable
                inline Variables & operator()(const char   *name, const Variable::Pointer &link)
                {
                    const string __name = name; return (*this)(__name,link);
                }

                //! access named variables
                template <typename T>
                inline T & operator()( array<T> &arr, const string &name ) const
                {
                    const Variables &self = *this;
                    return arr[ self[name]->check_index(arr.size()) ];
                }

                //! access named variables
                template <typename T>
                inline T & operator()( array<T> &arr, const char *name ) const
                {
                    const Variables &self = *this;
                    return arr[ self[name]->check_index(arr.size()) ];
                }

                //! access named variable, CONST
                template <typename T>
                inline const T & operator()( const array<T> &arr, const string &name ) const
                {
                    const Variables &self = *this;
                    return arr[ self[name]->check_index(arr.size()) ];
                }

                //! access named variable, CONST
                template <typename T>
                inline const T & operator()( const array<T> &arr, const char *name ) const
                {
                    const Variables &self = *this;
                    return arr[ self[name]->check_index(arr.size()) ];
                }


            };



        }
    }

}

#endif

