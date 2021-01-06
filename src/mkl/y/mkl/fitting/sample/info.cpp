
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

            variables & sample_info:: operator*() throw()
            {
                return vars;
            }


            const variables & sample_info:: operator*() const throw()
            {
                return vars;
            }


        }

    }

}
