
//! \file

#ifndef Y_MATH_ADJUST_VARIABLES_INCLUDED
#define Y_MATH_ADJUST_VARIABLES_INCLUDED 1

#include "y/math/adjust/variable.hpp"
#include "y/string/display.hpp"
#include "y/math/kernel/errors.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {


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
                typedef vector<string,memory::pooled> Strings; //!< alias for I/O
                
                explicit Variables() throw();                    //!< setup
                virtual ~Variables() throw();                    //!< cleanup
                Variables(const Variables &);                    //!< copy
                Variables & operator=( const Variables &other ); //!< assign

                Variables( const string &names);
                Variables( const char   *names);

                const Variable & operator[](const string &name) const; //!< get by name
                const Variable & operator[](const char   *name) const; //!< get by name

                Variables & decl(const string &name,size_t index=0);   //!< declare Global, must be unique
                Variables & decl(const char   *name,size_t index=0);   //!< declare Global, must be unique

                Variables & operator<<( const string &name ); //!< decl(name,0)
                Variables & operator<<( const char   *name ); //!< decl(name,0)

                Variables & operator()(const Variable &v, const string &name); //!< declare Linked
                Variables & operator()(const Variable &v, const char   *name); //!< declare Linked
                Variables & operator()(const Variable &v);                     //!< Linked with same name


                size_t MaxLength() const throw(); //!< max( name.size() )

                //! generalized access operator
                template <typename T>
                inline T & operator()( addressable<T> &source, const string &id ) const
                {
                    const  Variables &self = *this;
                    return self[id].get(source);
                }

                //! generalized access operator, const
                template <typename T>
                inline const T & operator()( const accessible<T> &source, const string &id ) const
                {
                    const  Variables &self = *this;
                    return self[id].get(source);
                }

                //! display '[pfx]name  = data'
                template <typename T> inline
                void display( std::ostream &os, const accessible<T> &source, const char *pfx=NULL) const
                {
                    const Variables &self = *this;
                    const size_t     nmax = MaxLength();
                    for( const_iterator it = begin(); it != end(); ++it )
                    {
                        const string &name = (**it).name;
                        if(pfx)
                        {
                            os << pfx;
                        }
                        string_display::align(os,name,nmax) << " = " << self(source,name) << std::endl;
                    }
                }

                //! display '[pfx]name = aorg +/- aerr'
                template <typename T> inline
                void display( std::ostream &os, const accessible<T> &aorg, const accessible<T> &aerr, const char *pfx=NULL) const
                {
                    const Variables &self = *this;
                    const size_t     nvar = self.size();
                    Strings          sorg(nvar,as_capacity);
                    const size_t     zorg = fillStrings(sorg,aorg);

                    Strings          serr(nvar,as_capacity);
                    const size_t     zerr = fillStrings(serr,aerr);

                    Strings          sper(nvar,as_capacity);

                    const size_t        nmax = MaxLength();
                    size_t              ivar = 1;
                    for( const_iterator it = begin(); it != end(); ++it, ++ivar)
                    {
                        const Variable &v    = **it;
                        const string   &name = v.name;
                        if(pfx)
                        {
                            os << pfx;
                        }
                        string_display::align(os,name,nmax) << " = ";
                        string_display::align(os,sorg[ivar],zorg) << " \\pm ";
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

                //! setting all on/off
                Variables & set_all( addressable<bool> &used, const bool flag);
                
                //! setting only named variables on/off
                Variables & set_only( addressable<bool> &used, const string &names, const bool flag );
                
                
                //! turning all named variables to on, other are left untouched
                Variables & on( addressable<bool> &used, const string &names)
                {
                   return set_only(used,names,true);
                }
                
                //! turning all variables to off, then all named to on
                Variables & only_on( addressable<bool> &used, const string &names)
                {
                   return  set_all(used,false).on(used,names);
                }
                
            private:
                void   update();
                string toString( const double value ) const;
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

                void buildFrom( const string &names );

            };
        }

    }
}

#endif

