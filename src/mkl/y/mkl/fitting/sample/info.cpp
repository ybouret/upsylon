
#include "y/mkl/fitting/sample/info.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            sample_info:: ~sample_info() throw()
            {
            }

            const string & sample_info:: key() const throw()
            {
                return name;
            }

            
            void sample_info:: save_triplet( ios::ostream &fp, const double x, const double y, const double z)
            {
                fp("%.15g",x) << ' ';
                fp("%.15g",y) << ' ';
                fp("%.15g",z) << '\n';
            }

            const variables & sample_info:: bulk() const throw()
            {
                return vars;
            }
        }

    }

}
