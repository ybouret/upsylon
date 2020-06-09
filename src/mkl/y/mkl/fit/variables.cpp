
#include "y/math/fit/variables.hpp"
#include "y/math/fit/variable/global.hpp"
#include "y/math/fit/variable/linked.hpp"

#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace Fit  {

            const char Variables::Separator = ':';
            const char Variables::Equal[]   = " = ";
            const char Variables::PM[]      = " \\pm ";

            Variables:: ~Variables() throw()
            {
            }
            
            Variables:: Variables() throw() : Variable::Set()
            {
            }
            
            Variables:: Variables( const Variables &other ) :
            collection(),
            Variable::Set(other)
            {
            }


            Variables:: Variables(const string &names)
            {
                buildFrom(names);
            }

            Variables:: Variables(const char *names )
            {
                const string _(names);
                buildFrom(_);
            }

            void Variables:: buildFrom(const string &names)
            {
                Variables &self = *this;
                tokenizer<char> tkn( names );
                while( tkn.next_with(Separator) )
                {
                    const string name = tkn.to_string();
                    self << name;
                }
            }


            void Variables:: outputStatus(std::ostream &os, const bool flag) const
            {
                os << " : [ " << ( flag ? "USED " : "FIXED") << " ]";
            }


            ////////////////////////////////////////////////////////////////////
            // flags

            const Variables & Variables:: set_flags( addressable<bool> &used, const bool flag) const
            {
                //std::cerr << "set_flags(" << flag << ")" << std::endl;
                return ld(used,flag);
            }
            
            const Variables & Variables:: set_flags( addressable<bool> &used, const string &names, const bool flag ) const
            {
                //std::cerr << "set_flags(" << flag << "@" << names << ")" << std::endl;
                return ld(used,names,flag);
            }


            const Variables & Variables:: set_flags( addressable<bool> &used, const char *names, const bool flag ) const
            {
                const string _(names);
                return set_flags(used,_,flag);
            }


            const Variables & Variables:: off(addressable<bool> &used) const
            {
                //std::cerr << "off()" << std::endl;
                return set_flags(used,false);
            }

            const Variables & Variables:: off(addressable<bool> &used,const string &names) const
            {
               // std::cerr << "off(" << names << ")" << std::endl;
                return set_flags(used,names,false);
            }

            const Variables & Variables:: off(addressable<bool> &used,const char *names) const
            {
                const string _(names);
                return off(used,_);
            }



            const Variables & Variables:: only_off(addressable<bool> &used,const string &names) const
            {
                //std::cerr << "only_off(" << names << ")" << std::endl;
                return on(used).off(used,names);
            }

            const Variables & Variables:: only_off(addressable<bool> &used,const char *names) const
            {
                const string _(names);
                return only_off(used,_);
            }

            const Variables & Variables:: on(addressable<bool> &used) const
            {
                //std::cerr << "on()" << std::endl;
                return set_flags(used,true);
            }

            const Variables & Variables:: on(addressable<bool> &used,const string &names) const
            {
                //std::cerr << "on(" << names << ")" << std::endl;
                return set_flags(used,names,true);
            }

            const Variables & Variables:: on(addressable<bool> &used,const char *names) const
            {
                const string _(names);
                return on(used,_);
            }



            const Variables & Variables:: only_on(addressable<bool> &used,const string &names) const
            {
                //std::cerr << "only_on(" << names << ")" << std::endl;
                return off(used).on(used,names);
            }

            const Variables & Variables:: only_on(addressable<bool> &used,const char *names) const
            {
                const  string _(names);
                return only_on(used,_);
            }

            ////////////////////////////////////////////////////////////////////

            
            Variables & Variables:: operator=( const Variables &other )
            {
                Variable::Set &self = *this;
                self = other;
                return *this;
            }
            
            
            const Variable & Variables:: operator[](const string &name) const
            {
                const Variable::Pointer *ppV = search(name);
                if( !ppV ) {
                    throw exception("no variable[%s]",*name);
                }
                return **ppV;
            }
            
            const Variable & Variables:: operator[](const char   *name) const
            {
                const string _(name);
                return (*this)[_];
            }
            
            Variables & Variables:: decl(const string &name,size_t index)
            {
                if(index<=0) index = size()+1;
                {
                    const iterator last = end();
                    for( iterator i=begin(); i != last; ++i)
                    {
                        const Variable &v = **i;
                        if( v.index() == index )
                        {
                            throw exception("<%s> index=%u already for <%s>", *name, unsigned(index), *v.name );
                        }
                    }
                }
                const Variable::Pointer V = new GlobalVariable(name,index);
                if(!insert(V))
                {
                    throw exception("multiple global variable <%s>", *name);
                }
                update();
                return *this;
            }
            
            Variables & Variables:: decl(const char *name,size_t index)
            {
                const string _(name);
                return decl(_,index);
            }
            
            
            Variables & Variables:: operator<<( const string &name )
            {
                return decl(name,0);
            }
            
            Variables & Variables:: operator<<( const char *name )
            {
                return decl(name,0);
            }
            
            
            Variables & Variables:: operator()(const Variable &v, const string &name)
            {
                const Variable::Pointer link = & aliasing::_(v);
                const Variable::Pointer l    = new LinkedVariable(name,link);
                if( !insert(l) )
                {
                    throw exception("multiple linked variable '%s'", *name);
                }
                update();
                return *this;
            }
            
            Variables & Variables:: operator()(const Variable &v, const char *name)
            {
                const string _(name);
                return (*this)(v,_);
            }
            
            Variables & Variables:: operator()(const Variable &v)
            {
                return (*this)(v,v.name);
            }
            
            
            size_t Variables:: maxNameLength() const throw()
            {
                size_t ans = 0;
                for( const_iterator it=begin(); it != end(); ++it)
                {
                    const size_t len = (**it).name.size();
                    if(len>ans)  { ans = len; }
                }
                return ans;
            }

            void Variables:: activate( addressable<bool> &target, const accessible<bool> &source ) const
            {
                for( const_iterator it=begin(); it != end(); ++it)
                {
                    (**it).activate(target,source);
                }
            }


            static inline
            int __compare_indices(const Variable::Pointer &lhs,
                                  const Variable::Pointer &rhs)
            {
                const size_t L = lhs->index();
                const size_t R = rhs->index();
                return comparison::increasing(L,R);
            }
            
            void Variables:: update()
            {
                sort_data( __compare_indices );
            }
            
            void Variables:: chkdbg() const
            {
                if( size() > 1 )
                {
                    const size_t imax = size() - 1;
                    for(size_t i=0;i<imax;++i)
                    {
                        const Variable &lhs = *fetch(i);
                        const Variable &rhs = *fetch(i+1);
                        if( lhs.index() >= rhs.index() )
                        {
                            throw exception("Variables Invalid indices <%s@%u> <%s@%u>",
                                            *lhs.name, unsigned(lhs.index()),
                                            *rhs.name, unsigned(rhs.index()));
                        }
                    }
                }
            }

            size_t Variables:: sweep() const throw()
            {
#if !defined(NDEBUG)
                chkdbg();
#endif
                if( size() > 0 )
                {
                    return tail()->index();
                }
                else
                {
                    return 0;
                }
            }

            string Variables:: toString( const double value ) const
            {
                return vformat("%.15g",value);
            }
        }

    }
}

#include "y/container/query.hpp"
namespace upsylon {

    namespace math {

        namespace Fit {

            static inline bool byIndex( const Variable::Pointer &v, const size_t index) throw()
            {
                return v->index() == index;
            }

            const Variable * Variables:: searchIndex( const size_t index ) const throw()
            {
                const Variable::Pointer *pV = container_query::first( *this, byIndex, index);
                if(pV)
                {
                    return & (**pV) ;
                }
                else
                {
                    return 0;
                }
            }

            const Variable & Variables:: at(const size_t index) const
            {
                const Variable::Pointer *pV = container_query::first( *this, byIndex, index);
                if(!pV) throw exception("No Fit::Variable@%u", unsigned(index) );
                return **pV;
            }


        }
    }
}



