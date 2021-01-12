
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
            verbose( static_cast<gls_type *>(impl)->verbose  ),
            scaling( static_cast<gls_type *>(impl)->grad().h )
            {
            }


            bool gls:: operator()(adjustable            &s,
                                  sequential_type        &F,
                                  v_gradient_type        &G,
                                  addressable<double>    &A,
                                  const accessible<bool> &U,
                                  addressable<double>    &E)
            {
                return static_cast<gls_type *>(impl)->fit(s,F,G,A,U,E);
            }

            bool gls:: operator()(adjustable             &s,
                                  sequential_type        &F,
                                  addressable<double>    &A,
                                  const accessible<bool> &U,
                                  addressable<double>    &E)
            {
                return static_cast<gls_type *>(impl)->fit(s,F,A,U,E);
            }

            bool gls:: operator()(adjustable             &s,
                                  function               &f,
                                  addressable<double>    &A,
                                  const accessible<bool> &U,
                                  addressable<double>    &E)
            {
                return static_cast<gls_type *>(impl)->fit(s,f,A,U,E);
            }

        }

    }

}

