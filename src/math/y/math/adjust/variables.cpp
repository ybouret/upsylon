
#include "y/math/adjust/variables.hpp"
#include "y/math/adjust/variable/global.hpp"
#include "y/math/adjust/variable/linked.hpp"

#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/string/tokenizer.hpp"
namespace upsylon {
    
    namespace math {
        
        namespace Adjust  {
            
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
                while( tkn.next_with(':') )
                {
                    const string name = tkn.to_string();
                    self << name;
                }
            }
            
            Variables & Variables:: set_all( addressable<bool> &used, const bool flag)
            {
                for( iterator i=begin();i!=end();++i)
                {
                    const Variable &v    = **i;
                    const string   &name = v.name;
                    (*this)(used,name)   = flag;
                }
                return *this;
            }
            
            Variables & Variables:: set_only( addressable<bool> &used, const string &names, const bool flag )
            {
                tokenizer<char> tkn( names );
                while( tkn.next_with(':') )
                {
                    const string name = tkn.to_string();
                    (*this)(used,name) = flag;
                }
                return *this;
            }


            
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
            
            
            size_t Variables:: MaxLength() const throw()
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



