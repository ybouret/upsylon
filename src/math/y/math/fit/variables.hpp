//! \file
#ifndef Y_MATH_FIT_VARIABLES_INCLUDED
#define Y_MATH_FIT_VARIABLES_INCLUDED 1

#include "y/string.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/intr.hpp"
#include "y/math/types.hpp"

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

                const   string  name;                      //!< unique name
                const   Type    type;                      //!< keep track
                const   string &key()   const throw();     //!< name
                virtual size_t  index() const throw() = 0; //!< global index
                virtual ~Variable() throw();               //!< destructor

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

                //! get max(name.size())
                size_t get_max_name_size() const throw();

                //! display named values of array
                template <typename T> inline
                void diplay(std::ostream &os, const array<T> &arr,const char *pfx=NULL) const
                {
                    const size_t sz = get_max_name_size();
                    for(const_iterator i=begin();i!=end();++i)
                    {
                        const string &name  = (**i).name;
                        const T      &value = (*this)(arr,name);
                        if(pfx) os << pfx;
                        os << name;
                        for(size_t j=sz;j>name.size();--j) os << ' ';
                        os << " = ";
                        os << value << std::endl;
                    }
                }

                //! evaluate relative error for display
                template <typename T>
                static inline
                T compute_relative_error( const T aorg, const T aerr ) throw()
                {
                    const T ratio = (__fabs(aerr)+numeric<T>::tiny)/(__fabs(aorg)+numeric<T>::tiny);
                    return 100 * ratio;
                }

                //! display named values and errors
                template <typename T> inline
                void diplay(std::ostream &os, const array<T> &aorg, const array<T> &aerr, const char *pfx=NULL) const
                {
                    const size_t sz = get_max_name_size();
                    const size_t nv = this->size();
                    vector<string,memory::pooled> astr(nv,as_capacity);
                    vector<string,memory::pooled> estr(nv,as_capacity);
                    size_t       am = 0;
                    size_t       em = 0;

                    for(const_iterator i=begin();i!=end();++i)
                    {
                        const string &name  = (**i).name;
                        {
                            const string  s     = vformat("%.15g",double( (*this)(aorg,name) ));
                            astr.push_back(s);
                            am = max_of(am,s.size());
                        }
                        {
                            const string  s     = vformat("%.15g",double( (*this)(aerr,name) ));
                            estr.push_back(s);
                            em = max_of(em,s.size());
                        }
                    }
                    size_t iv=1;
                    for(const_iterator i=begin();i!=end();++i,++iv)
                    {
                        const string &name  = (**i).name;
                        if(pfx) os << pfx;
                        os << name;
                        for(size_t j=sz;j>name.size();--j) os << ' ';
                        os << " = ";
                        {
                            const string &value = astr[iv];
                            os << value; for(size_t j=am;j>value.size();--j) os << ' ';
                            os << " +/- ";
                        }
                        {
                            const string &value = estr[iv];
                            os << value; for(size_t j=em;j>value.size();--j) os << ' ';
                            os << ' ';
                            const string percent = vformat( "%6.2f", double(compute_relative_error((*this)(aorg,name) , (*this)(aerr,name))) );
                            os << '(' << percent << '%' << ')';
                        }
                        os << std::endl;
                    }
                }


            };



        }
    }

}

#endif

