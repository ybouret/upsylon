
//! \file

#ifndef Y_MKL_ADJUST_VARIABLES_INCLUDED
#define Y_MKL_ADJUST_VARIABLES_INCLUDED 1

#include "y/mkl/fit/variable.hpp"
#include "y/mkl/kernel/errors.hpp"

#include "y/string/tokenizer.hpp"
#include "y/string/display.hpp"

namespace upsylon {

    namespace math {

        namespace Fit {


            //==================================================================
            //
            //
            //! named variables interface
            //
            //
            //==================================================================
            class Variables : public Variable::Set
            {
            public:
                typedef vector<string,memory::pooled> Strings;   //!< alias for I/O
                static  const  char                   Separator; //!< for multiple variables names
                static  const  char                   Equal[];   //!< " = ";
                static  const  char                   PM[];      //!< " \\pm ";

                explicit Variables() throw();                    //!< setup
                virtual ~Variables() throw();                    //!< cleanup
                Variables(const Variables &);                    //!< copy
                Variables & operator=( const Variables &other ); //!< assign

                Variables( const string &names); //!< create global variables from colon-separated names
                Variables( const char   *names); //!< create global variables from colon-separated names

                const Variable & operator[](const string &name) const; //!< get by name
                const Variable & operator[](const char   *name) const; //!< get by name

                Variables & decl(const string &name,size_t index=0);   //!< declare Global, must be unique
                Variables & decl(const char   *name,size_t index=0);   //!< declare Global, must be unique

                Variables & operator<<( const string &name ); //!< decl(name,0)
                Variables & operator<<( const char   *name ); //!< decl(name,0)

                Variables & operator()(const Variable &v, const string &name); //!< declare Linked
                Variables & operator()(const Variable &v, const char   *name); //!< declare Linked
                Variables & operator()(const Variable &v);                     //!< Linked with same name


                size_t maxNameLength() const throw(); //!< max( name.size() )

                //! generalized access operator
                template <typename T>
                inline T & operator()(addressable<T> &source, const string &id) const
                {
                    const  Variables &self = *this;
                    return self[id].get(source);
                }

                //! generalized access operator
                template <typename T>
                inline T & operator()(addressable<T> &source, const char *id) const
                {
                    const string _(id); return (*this)(source,_);
                }

                //! generalized access operator, const
                template <typename T>
                inline const T & operator()(const accessible<T> &source, const string &id) const
                {
                    const  Variables &self = *this;
                    return self[id].get(source);
                }

                //! generalized access operator, const
                template <typename T>
                inline const T & operator()(const accessible<T> &source, const char *id) const
                {
                    const string _(id); return (*this)(source,_);
                }

                //! bool -> int
                inline int flag( const accessible<bool> &source, const string &id ) const
                {
                    return (*this)(source,id) ? 1 : 0;
                }

                //! bool -> int, wrapper
                inline int flag( const accessible<bool> &source, const char *id ) const
                {
                    const  string _(id);
                    return flag(source,_);
                }

                //! look for a value
                template <typename T>
                inline T *lookFor(addressable<T> &aorg, const string &name) const
                {
                    const Variable::Pointer *ppV = search(name);
                    if(ppV)
                    {
                        return & (**ppV).get(aorg);
                    }
                    else
                    {
                        return NULL;
                    }
                }

                //! look for a value, wrapper
                template <typename T>
                inline T *lookFor(addressable<T> &aorg, const char *name) const
                {
                    const string _(name);
                    return lookFor(aorg,_);
                }

                //! look for a const value
                template <typename T>
                inline const T *lookFor(const accessible<T> &aorg, const string &name) const
                {
                    const Variable::Pointer *ppV = search(name);
                    if(ppV)
                    {
                        return & (**ppV).get(aorg);
                    }
                    else
                    {
                        return NULL;
                    }
                }

                //! look for a const value, wrapper
                template <typename T>
                inline const T *lookFor(const accessible<T> &aorg, const char *name) const
                {
                    const string _(name);
                    return lookFor(aorg,_);
                }



                //! display '[pfx]name  = data'
                template <typename T> inline
                void display(std::ostream &os, const accessible<T> &source, const char *pfx=NULL) const
                {
                    const Variables &self = *this;
                    const size_t     nmax = maxNameLength();
                    for( const_iterator it = begin(); it != end(); ++it )
                    {
                        const string &name = (**it).name;
                        if(pfx)
                        {
                            os << pfx;
                        }
                        string_display::align(os,name,nmax) << Equal << self(source,name) << std::endl;
                    }
                }

                //! display '[pfx]name = aorg +/- aerr'
                template <typename T> inline
                void display( std::ostream &os, const accessible<T> &aorg, const accessible<T> &aerr, const char *pfx=NULL) const
                {
                    const Variables   &self = *this;
                    const size_t       nvar = self.size();
                    Strings            sorg(nvar,as_capacity);
                    const size_t       zorg = fillStrings(sorg,aorg);
                    Strings            serr(nvar,as_capacity);
                    const size_t       zerr = fillStrings(serr,aerr);
                    Strings            sper(nvar,as_capacity);
                    const size_t       nmax = maxNameLength();
                    size_t             ivar = 1;
                    for(const_iterator iter = begin(); iter != end(); ++iter, ++ivar)
                    {
                        const Variable &v    = **iter;
                        const string   &name = v.name;
                        if(pfx)
                        {
                            os << pfx;
                        }
                        string_display::align(os,name,nmax)       << Equal;
                        string_display::align(os,sorg[ivar],zorg) << PM;
                        string_display::align(os,serr[ivar],zerr);
                        const T p = errors::percent(aerr[ivar], aorg[ivar]);
                        const string per = vformat("%6.2lf",double(p));
                        os << ' ' << '(' << per << '%' << ')';
                        os << std::endl;
                    }
                }



                size_t sweep() const throw(); //!< maximun index
                
                //! activate all matching targets
                void   activate( addressable<bool> &target, const accessible<bool> &source ) const;

                //! just to debug
                void   chkdbg() const;


                //! load all variables
                template <typename T>
                const Variables &ld(addressable<T>                         &param,
                                    typename type_traits<T>::parameter_type value) const
                {
                    const Variables    &self = *this;
                    for(const_iterator i=self.begin();i!=end();++i)
                    {
                        self(param,(**i).name) = value;
                    }
                    return self;
                }

                //! load only on named variables
                template <typename T>
                const Variables &ld(addressable<T>                         &param,
                                    const string                           &names,
                                    typename type_traits<T>::parameter_type value) const
                {
                    const Variables    &self = *this;
                    tokenizer<char>     tkn(names);
                    while( tkn.next_with( Separator ) )
                    {
                        const string id = tkn.to_string();
                        self(param,id)  = value;
                    }
                    return self;
                }

                //! load only on named variables, wrapper
                template <typename T>
                const Variables &ld(addressable<T>                         &param,
                                    const char                             *names,
                                    typename type_traits<T>::parameter_type value) const
                {
                    const string _(names); return ld(param,_,value);
                }

                //! set all used flags
                const Variables &set_flags(addressable<bool> &used, const bool flag) const;
                //! set list of variables to flag
                const Variables &set_flags(addressable<bool> &used, const string &names, const bool flag) const;
                //! set list of variables to flag, wrapper
                const Variables &set_flags(addressable<bool> &used, const char   *names, const bool flag) const;

                //! set all used to OFF
                const Variables &off(addressable<bool> &used) const;
                //! set list of used to OFF
                const Variables &off(addressable<bool> &used, const string &names) const;
                //! set list of used to OFF, wrapper
                const Variables &off(addressable<bool> &used, const char   *names) const;

                //! set all used to ON, then turn OFF list of named variables
                const Variables &only_off(addressable<bool> &used, const string &names) const;
                //! set all used to ON, then turn OFF list of named variables, wrapper
                const Variables &only_off(addressable<bool> &used, const char   *names) const;

                //! set all variables to ON
                const Variables &on(addressable<bool> &used) const;
                //! set list of named variables to ON
                const Variables &on(addressable<bool> &used,const string &names) const;
                //! set list of named variables to ON, wrapper
                const Variables &on(addressable<bool> &used,const char   *names) const;
                //! set all used to OFF, then turn ON named variables
                const Variables &only_on(addressable<bool> &used,const string &names) const;
                //! set all used to OFF, then turn ON named variables, wrapper
                const Variables &only_on(addressable<bool> &used,const char   *names) const;

                //! display value/status for earch variable
                template <typename T>
                void displayStatus( std::ostream &os, const accessible<T> &aorg, const accessible<bool> &used, const char *pfx ) const
                {
                    const Variables &self = *this;
                    const size_t     nvar = self.size();
                    Strings          sorg(nvar,as_capacity);
                    const size_t     zorg = fillStrings(sorg,aorg);
                    const size_t     nmax = maxNameLength();
                    size_t           ivar = 1;
                    for( const_iterator it = begin(); it != end(); ++it,++ivar )
                    {
                        const Variable &v    = **it;
                        const string   &name = v.name;
                        if(pfx)
                        {
                            os << pfx;
                        }
                        string_display::align(os,name,nmax) << Equal;
                        string_display::align(os,sorg[ivar],zorg);
                        outputStatus(os,self(used,name));
                        os << std::endl;
                    }
                }

                //! convert real values intro string, return max length of strings
                template <typename T> inline
                size_t fillStrings( Strings &strings, const accessible<T> &values ) const
                {
                    size_t ans = 0;
                    for( const_iterator it = begin(); it != end(); ++it )
                    {
                        const Variable &v = **it;
                        const string    s = toString( double( (*this)(values,v.name) ) );
                        strings.push_back_(s);
                        ans = max_of(ans,s.size());
                    }
                    return ans;
                }

                //! get variable by its index, NULL if not found
                const Variable *searchIndex( const size_t index ) const throw();

                //! get variable at a given index
                const Variable &at(const size_t index) const;

            private:
                void   update();
                string toString( const double value ) const;


                void buildFrom( const string &names );
                void outputStatus(std::ostream &os, const bool flag) const;

            };
        }

    }
}

#endif

