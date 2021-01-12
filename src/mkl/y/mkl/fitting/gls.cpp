
#include "y/mkl/fitting/gls.hpp"
#include "y/mkl/fitting/least-squares.hpp"

namespace upsylon
{

    namespace mkl
    {
        namespace fitting
        {

            namespace {

                typedef least_squares<double,double> gls_type;

            }

            gls:: ~gls() throw()
            {
                assert(impl);
                delete static_cast<gls_type *>(impl);
                impl = 0;
            }

            gls:: gls(const bool verb) :
            impl( new gls_type(verb) ),
            verbose( static_cast<gls_type *>(impl)->verbose )
            {
            }
        }

    }

}

