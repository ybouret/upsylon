//! \file
#ifndef Y_MATH_FIT_VARIABLES_INCLUDED
#define Y_MATH_FIT_VARIABLES_INCLUDED 1

#include "y/string.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/intr.hpp"
#include "y/math/types.hpp"
#include "y/string/tokenizer.hpp"
#include "y/memory/pooled.hpp"

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
                typedef vector<string,memory::pooled> Strings;

                //! constructor, may reserve memory
                Variables(const size_t n=0);
                //! constructor from list
                Variables( const string &var_list);
                //! constructor form list
                Variables( const char   *var_list );
                //! destructor
                virtual ~Variables() throw();
                //! copy
                Variables(const Variables &);
                //!assign
                Variables & operator=( const Variables &other );

                //! create a global variable, assigning current size+1 as index
                Variables        & create_global(const string &name);

                //! create a global variable, wrapper
                Variables & create_global(const char *name);

                //! quick create a global variable
                Variables & operator<<( const string &name);

                //! quick create a global variable
                Variables & operator<<( const char   *name);

                //! access
                const Variable::Pointer & operator[](const string &name) const;
                
                //! access
                const Variable::Pointer & operator[](const char *name) const;

                //! create a global variable
                Variables & operator()(const string &name);

                //! create a global variable
                Variables & operator()(const char   *name);

                //! create a local variable linking to a global variable link
                Variables  & operator()(const string &name, const Variable::Pointer &link);

                //! create a local variable from a global variable with the same name
                Variables        & operator()(const string &name, const Variables &global);

                //! create a local variable linking to a global variable link
                Variables & operator()(const char   *name, const Variable::Pointer &link);

                //! create a local variable from a global variable with the same name
                Variables & operator()(const char   *name, const Variables &global);

                //! create "prefix" in this local variables, and "prefix#index" for global
                Variables & operator()(const string &prefix, Variables &global, const size_t index);

                //!create "prefix" in this local variables, and "prefix_index" for global, wrapper
                Variables & operator()(const char *prefix, Variables &global, const size_t index);


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
                
                //! used to parse variables list
                static inline bool is_colon( const char C )  { return ':' == C; }

                //! load same value in a list of colon separated variables
                template <typename T>
                inline void ld(array<T>     &arr,
                               const string &id,
                               typename type_traits<T>::parameter_type value ) const
                {
                    tokenizer<char>  tknz(id);
                    const Variables &self = *this;
                    const size_t     nvar = arr.size();
                    bool           (*proc)(const char) = is_colon;
                    while( tknz.next(proc) )
                    {
                        const string var_name( tknz.token(), tknz.units() );
                        arr[ self[var_name]->check_index(nvar) ] = value;
                    }
                }

                //! load same value in a list of colon separated variables
                template <typename T>
                inline void ld( array<T>  &arr,
                               const char *id,
                               typename type_traits<T>::parameter_type value ) const
                {
                    const string _(id); ld<T>(arr,_,value);
                }

                //! boolean helpers, set true
                inline void on( array<bool> &flags, const string &id ) const
                {
                    ld<bool>(flags,id,true);
                }

                //! boolean helpers, set true
                inline void on( array<bool> &flags, const char *id ) const
                {
                    ld<bool>(flags,id,true);
                }



                //! boolean helpers, reset and set true
                inline void only_on(array<bool> &flags, const string &id ) const
                {
                    flags.ld(false); on(flags,id);
                }

                //! boolean helpers, reset and set true, wrapper
                inline void only_on(array<bool> &flags, const char *id ) const
                {
                    flags.ld(false); on(flags,id);
                }

                //! boolean helpers, set false
                inline void off( array<bool> &flags, const string &id ) const
                {
                    ld<bool>(flags,id,false);
                }

                //! boolean helpers, set false
                inline void off( array<bool> &flags, const char *id ) const
                {
                    ld<bool>(flags,id,false);
                }


                //! boolean helpers, all true then set false
                inline void only_off( array<bool> &flags, const string &id ) const
                {
                    flags.ld(true); off(flags,id);
                }

                //! boolean helpers, all true then set false, wrapper
                inline void only_off( array<bool> &flags, const char *id ) const
                {
                    flags.ld(true); off(flags,id);
                }

                //! get max(name.size())
                size_t get_max_name_size() const throw();

                //! display named values of array
                template <typename T> inline
                void display(std::ostream &os, const array<T> &arr,const char *pfx=NULL) const
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
                    const T ratio = (fabs_of(aerr)+numeric<T>::tiny)/(fabs_of(aorg)+numeric<T>::tiny);
                    return 100 * ratio;
                }

                //! convert reals to strings, return max string lenght
                template <typename T>
                size_t fillStrings( Strings &strings, const array<T> &arr ) const
                {
                    const Variables &self = *this;
                    strings.free();
                    strings.ensure( self.size() );
                    size_t max_length = 0;
                    for(const_iterator i=begin();i!=end();++i)
                    {
                        const string &name  = (**i).name;
                        const string  s     = vformat( "%.15g", double( self(arr,name) ) );
                        strings.push_back_(s);
                        max_length = max_of(max_length,s.size());
                    }
                    return max_length;
                }

                //! display named values and errors
                template <typename T> inline
                void display(std::ostream &os, const array<T> &aorg, const array<T> &aerr, const char *pfx=NULL) const
                {
                    const size_t sz = get_max_name_size();
                    const size_t nv = this->size();
                    Strings astr(nv,as_capacity);
                    Strings estr(nv,as_capacity);
                    const size_t am = fillStrings( astr, aorg);
                    const size_t em = fillStrings( estr, aerr);
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

            private:
                void create_from_list(const string &ids );
            };



        }
    }

}

#endif

