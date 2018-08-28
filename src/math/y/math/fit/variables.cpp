#include "y/math/fit/variables.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            Variable:: ~Variable() throw() {}
            
            Variable:: Variable(const string & __name, const Type __type) :
            name( __name ), type( __type ) {}

            const string & Variable:: key() const throw() { return name; }


            size_t Variable:: check_index( const size_t against_size ) const
            {
                const size_t indx = index();
                if(indx<=0||indx>against_size)
                {
                    throw exception("Invalid index %u/%u for Variable '%s'", unsigned(indx), unsigned(against_size), *name );
                }
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
            GlobalVariable:: GlobalVariable(const string & __name, const size_t __indx ) :
            Variable(__name,IsGlobal),
            indx(__indx)
            {
                if(indx<=0) throw exception("%s.indx<=0", *name );
            }

            GlobalVariable:: ~GlobalVariable() throw()
            {
            }

            size_t GlobalVariable:: index() const throw() { return indx; }

        }

    }
}

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            LocalVariable:: LocalVariable( const string & __name, const Variable::Pointer &__link ) :
            Variable( __name, IsLocal ),
            link(__link)
            {
            }

            LocalVariable:: ~LocalVariable() throw() {}
            
            size_t LocalVariable:: index() const throw() { return link->index(); }
        }
    }
}

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            Variables:: Variables(const size_t n) : Variable::Set(n,as_capacity)
            {

            }

            Variables:: ~Variables() throw() {}

            Variables:: Variables(const Variables &other) : dynamic(), Variable::Set(other) {}

            Variables & Variables:: operator=( const Variables &other )
            {
                Variable::Set &self = *this;
                self = other;
                return *this;
            }

            Variables & Variables:: create_global(const string &name)
            {
                const Variable::Pointer p = new GlobalVariable(name,size()+1);
                if( ! insert(p) )
                {
                    throw exception("Variables( Multiple Global '%s' )", *name );
                }
                return *this;
            }

            const Variable::Pointer & Variables:: operator[](const string &name) const
            {
                const Variable::Pointer *ppVar = search(name);
                if(!ppVar) throw exception("no Variables['%s']", *name);
                return *ppVar;
            }

            Variables & Variables:: operator()(const string &name, const Variable::Pointer &link)
            {
                const Variable::Pointer p = new LocalVariable(name,link);
                if(!insert(p))
                {
                    throw exception("Variables( Multiple Local '%s' )", *name );
                }
                return *this;
            }

        }

    }
}


