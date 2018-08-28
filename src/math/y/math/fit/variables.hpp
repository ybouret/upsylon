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
                enum Type
                {
                    IsGlobal,
                    IsLocal
                };
                typedef intr_ptr<string,Variable>   Pointer;
                typedef set<string,Pointer>         Set;

                const   string  name;
                const   Type    type;
                const   string &key() const throw();
                virtual size_t index() const throw() = 0;
                virtual ~Variable() throw();
                
                inline friend std::ostream & operator<<( std::ostream &os, const Variable &var )
                {
                    os << var.name << '@' << var.index();
                    return os;
                }
            protected:
                explicit Variable(const string &__name,
                                  const Type    __type);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Variable);
            };

            //! a global,top level variable
            class GlobalVariable : public Variable
            {
            public:
                explicit GlobalVariable(const string &__name,
                                        const size_t  __indx);
                virtual ~GlobalVariable() throw();
                virtual size_t index() const throw(); //!< return indx

                const size_t indx; //!< global index

            private:
                Y_DISABLE_COPY_AND_ASSIGN(GlobalVariable);
            };

            //! a local variable, linked to another variable
            class LocalVariable : public Variable
            {
            public:
                explicit LocalVariable( const string & __name, const Variable::Pointer &__link );
                virtual ~LocalVariable() throw();

                const Variable::Pointer link;
                virtual size_t index() const throw(); //!< return link->index()

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LocalVariable);
            };

            // Variables management
            class Variables : public Variable::Set
            {
            public:
                Variables(const size_t n=0);
                virtual ~Variables() throw();
                Variables(const Variables &);
                Variables & operator=( const Variables &other );

                Variables        & create_global(const string &name);
                inline Variables & create_global(const char *name)
                {
                    const string __name = name;
                    return create_global(__name);
                }

                Variables & operator<<( const string &name) { return create_global(name); }
                Variables & operator<<( const char   *name) { return create_global(name); }

            };



        }
    }

}

#endif

