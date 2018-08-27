#include "y/math/fit/variables.hpp"

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

            Variable:: Variable(const string & __name, const size_t __indx ) :
            name( __name ),
            link( __name ),
            indx( __indx )
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

        }
    }

}

#include "y/exception.hpp"

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



            Variables & Variables:: operator<<(const string &name)
            {
                const Variable var(name,size()+1);
                if(!insert(var))
                {
                    throw exception("Fit::Variables(multiple '%s')", *name);
                }
                return *this;
            }

            Variables & Variables:: operator<<(const char   *name)
            {
                const string id(name);
                return (*this) << id;
            }

            Variables & Variables:: operator()(const string &name, const string &link)
            {
                const Variable var(name,link,size()+1);
                if(!insert(var))
                {
                    throw exception("Fit::Variables(multiple '%s' [link='%s'] )", *name, *link);
                }
                return *this;
            }

            Variables & Variables:: operator()(const char   *name, const char   *link)
            {
                const string __name(name);
                const string __link(link);
                return (*this)(__name,__link);
            }


        }
    }
}

