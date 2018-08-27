//! \file
#ifndef Y_MATH_FIT_VARIABLES_INCLUDED
#define Y_MATH_FIT_VARIABLES_INCLUDED 1

#include "y/string.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace math
    {

        namespace Fit
        {
            //! one variable, local and link to global
            class Variable
            {
            public:
                typedef set<string,Variable> Set;

                const string name; //!< local name to extract value
                const string link; //!< global name to link
                const size_t indx; //!< global index in array

                const string &key() const throw(); //!< name

                ~Variable() throw(); //!< desctructor
                Variable(const string & __name,const size_t __indx); //!< local name=global name, index
                Variable(const string & __name,const string &__link, const size_t __indx); //!< local name, global name, global index
                Variable(const Variable &other); //!< copy

                inline friend std::ostream & operator<<( std::ostream &os, const Variable &var )
                {
                    os << var.name;
                    if(var.name!=var.link) { os << ':' << var.link; }
                    os << '@' << var.indx;
                    return os;
                }

            private:
                Y_DISABLE_ASSIGN(Variable);
            };

            //! a set of variables
            class Variables : public Variable::Set
            {
            public:
                Variables(const size_t n=0);
                virtual ~Variables() throw();
                Variables(const Variables &other);
                Variables & operator=( const Variables &);

                //! new global variable
                Variables & operator<<(const string &name);

                //! new global variable
                Variables & operator<<(const char   *name);

                inline Variables & operator()(const string &name) { return (*this) << name; }
                inline Variables & operator()(const char   *name) { return (*this) << name; }
                Variables & operator()(const string &name, const string &link);
                Variables & operator()(const char   *name, const char   *link);


            };
        }
        
    }
}

#endif

