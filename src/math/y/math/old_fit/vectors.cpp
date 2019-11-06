
#include "y/math/fit/vectors.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace math
    {

        namespace Fit
        {

            VectorsInfo:: ~VectorsInfo() throw()
            {
            }

            VectorsInfo:: VectorsInfo(const char *id)  : counted_object(), name(id)
            {
            }

            VectorsInfo:: VectorsInfo(const string &id)  : counted_object(), name(id)
            {
            }

            const string & VectorsInfo:: key() const throw()
            {
                return name;
            }

            void VectorsInfo:: CheckSamesSizes( const container &x, const container &y)
            {
                if( x.size() != y.size() )
                {
                    throw exception("Fit::Vectors data sizes mismatch!");
                }
            }

            void VectorsInfo:: MultipleVectorsException(const string &id)
            {
                throw exception("Vectors DataBase: multiple '%s'", *id );
            }

        }

    }

}

