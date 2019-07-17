#include "y/math/fit/variable.hpp"
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

            std::ostream & operator<<( std::ostream &os, const Variable &var )
            {
                os << var.name << '@' << var.index();
                return os;
            }
        }

    }
}

