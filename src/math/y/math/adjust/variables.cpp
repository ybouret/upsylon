
#include "y/math/adjust/variables.hpp"
#include "y/math/adjust/variable/global.hpp"
#include "y/math/adjust/variable/linked.hpp"

#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Adjust  {
     
        Variables:: ~Variables() throw()
        {
        }
        
        Variables:: Variables() throw() : Variable::Set()
        {
        }
        
        Variables:: Variables( const Variables &other ) :
        Variable::Set(other)
        {
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
            const Variable::Pointer v = new GlobalVariable(name,index);
            if(!insert(v))
            {
                throw exception("multiple global variable '%s'", *name);
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
                if(len>ans)  ans = len;
            }
            return ans;
        }
        
        static inline int compare_indices(const Variable::Pointer &lhs,
                                          const Variable::Pointer &rhs)
        {
            const size_t L = lhs->index();
            const size_t R = rhs->index();
            return comparison::increasing(L,R);
        }
        
        void Variables:: update()
        {
            sort_data( compare_indices );
        }
    }
}


