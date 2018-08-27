//! \file
#ifndef Y_MATH_FIT_VARIABLES_INCLUDED
#define Y_MATH_FIT_VARIABLES_INCLUDED 1

#include "y/string.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/array.hpp"

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
                Variable(const string & __name,const string &__link, const size_t __indx); //!< local name, global name, global index
                Variable(const Variable &other); //!< copy

                inline bool is_global() const throw() { return name==link; }

                inline friend std::ostream & operator<<( std::ostream &os, const Variable &var )
                {
                    os << var.name;
                    if(!var.is_global()) { os << ':' << var.link; }
                    os << '@' << var.indx;
                    return os;
                }

                size_t check_index(const size_t against_size) const;

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
                Variables        & create_global(const string &name);

                //! new global variable
                inline Variables & create_global(const char   *name)
                {
                    const string __name = name;
                    return create_global(__name);
                }

                //! new global variable : flux operator
                inline Variables & operator<<(const string &name) { return create_global(name); }

                //! new global variable : flux operator
                inline Variables & operator<<(const char   *name) { return create_global(name); }

                //! functional operator for global variable
                inline Variables & operator()(const string &name) { return create_global(name); }
                //! functional operator for glboal variable
                inline Variables & operator()(const char   *name) { return create_global(name); }

                //! link to a global variable
                Variables        & operator()(const string &name, const Variable &global);

                //! link to a global variable
                inline Variables & operator()(const char   *name, const Variable &global)
                {
                    const string __name(name); return (*this)(__name,global);
                }

                //! access to variable
                const Variable & operator[]( const string &name ) const;

                //! access to variable
                inline const Variable & operator[]( const char *name) const
                {
                    const string __name(name);
                    return (*this)[__name];
                }

                template <typename T>
                inline T & operator()(array<T> &arr,const string &name) const
                {
                    return arr[ (*this)[name].check_index(arr.size()) ];
                }

                template <typename T>
                inline T & operator()(array<T> &arr,const char *name) const
                {
                    return arr[ (*this)[name].check_index(arr.size()) ];
                }

                template <typename T>
                inline const T & operator()(const array<T> &arr, const string &name) const
                {
                    return arr[ (*this)[name].check_index(arr.size()) ];
                }

                template <typename T>
                inline const T & operator()(const array<T> &arr, const char *name) const
                {
                    return arr[ (*this)[name].check_index(arr.size()) ];
                }
            };
            
        }
        
    }
}

#endif

