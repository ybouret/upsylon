
//! \file

#ifndef Y_MATH_ADJUST_VARIABLES_INCLUDED
#define Y_MATH_ADJUST_VARIABLES_INCLUDED 1

#include "y/math/adjust/variable.hpp"
#include "y/string/display.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            class Variables : public Variable::Set
            {
            public:
                typedef vector<string,memory::pooled> Strings;
                
                explicit Variables() throw();
                virtual ~Variables() throw();
                Variables(const Variables &);
                Variables & operator=( const Variables &other );

                const Variable & operator[](const string &name) const;
                const Variable & operator[](const char   *name) const;

                Variables & decl(const string &name,size_t index=0);
                Variables & decl(const char   *name,size_t index=0);

                Variables & operator<<( const string &name );
                Variables & operator<<( const char   *name );

                Variables & operator()(const Variable &v, const string &name);
                Variables & operator()(const Variable &v, const char   *name);
                Variables & operator()(const Variable &v);


                size_t MaxLength() const throw();

                template <typename T>
                inline T & operator()( addressable<T> &source, const string &id ) const
                {
                    const  Variables &self = *this;
                    return self[id].get(source);
                }

                template <typename T>
                inline const T & operator()( const accessible<T> &source, const string &id ) const
                {
                    const  Variables &self = *this;
                    return self[id].get(source);
                }

                template <typename T>
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

                template <typename T>
                void display( std::ostream &os, const accessible<T> &aorg, const accessible<T> &aerr, const char *pfx=NULL) const
                {
                    const Variables &self = *this;
                    const size_t     nvar = self.size();
                    Strings          sorg(nvar,as_capacity);
                    const size_t     zorg = fillStrings(sorg,aorg);

                    Strings          serr(nvar,as_capacity);
                    const size_t     zerr = fillStrings(serr,aerr);

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
                        os << std::endl;
                    }
                }



                size_t sweep() const throw(); //!< maximun index

                void   activate( addressable<bool> &target, const accessible<bool> &source ) const;

                void   chkdbg() const;

            private:
                //! order by increasing index
                void update();

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


            };
        }

    }
}

#endif

