#include "y/math/fit/variables.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            Variable:: ~Variable() throw() {}

            Variable:: Variable(const Variable &other) :
            name(other.name),
            link(other.link),
            indx(other.indx)
            {
                assert(indx>0);
            }

            Variable:: Variable(const string & __name, const string & __link, const size_t __indx ) :
            name( __name ),
            link( __link ),
            indx( __indx )
            {
                assert(indx>0);
            }

            const string & Variable:: key() const throw()
            {
                return name;
            }

            size_t Variable:: check_index(const size_t against_size) const
            {
                assert(indx>0);
                if(indx>against_size) throw exception("Fit::Variable '%s' invalid index=%u against %u", *name, unsigned(indx), unsigned(against_size) );
                return indx;
            }
            

        }
    }

}


namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            Variables:: ~Variables() throw()
            {
            }
            
            Variables:: Variables(const size_t n) : Variable::Set(n,as_capacity)
            {
            }

            Variables:: Variables(const Variables &other) : Variable::Set(other) {}

            Variables & Variables:: operator=(const Variables &other)
            {
                Variable::Set &self = *this;
                self = other;
                return *this;
            }

            Variables & Variables:: create_global(const string &name)
            {
                const Variable var(name,name,size()+1);
                if(!insert(var))
                {
                    throw exception("Fit::Variables(multiple '%s')", *name);
                }
                return *this;
            }

            const Variable & Variables:: operator[]( const string &name ) const
            {
                const Variable *pVar = search(name);
                if(!pVar) throw exception("NO Fit::Variables['%s']", *name);
                return *pVar;
            }

            
            Variables & Variables:: operator()(const string &name, const Variable &global)
            {
                if(!global.is_global()) throw exception("Fit:: linking to a local variables: '%s'->'%s'",*global.name,*global.link);
                const Variable var(name,global.name,global.indx);
                if(!insert(var))
                {
                    throw exception("Fit::Variables(multiple '%s' [link='%s'] )", *name, *global.link);
                }
                return *this;
            }

        }
    }
}

