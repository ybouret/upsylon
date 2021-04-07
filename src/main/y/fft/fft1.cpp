
#include "y/fft/fft1.hpp"

namespace upsylon
{

    //! compute a fraction of size, double version
#define Y_FFT_SIN(S,LN2) (S sin(6.28318530717958623199592693708837032318115234375/(size_t(1)<<(LN2))))

    //! generate a table of sine fractions
#define Y_FFT_TABLE(S)                                                     \
Y_FFT_SIN(S,0),  Y_FFT_SIN(S,1),  Y_FFT_SIN(S,2),  Y_FFT_SIN(S,3),  Y_FFT_SIN(S,4), \
Y_FFT_SIN(S,5),  Y_FFT_SIN(S,6),  Y_FFT_SIN(S,7),  Y_FFT_SIN(S,8),  Y_FFT_SIN(S,9), \
Y_FFT_SIN(S,10), Y_FFT_SIN(S,11), Y_FFT_SIN(S,12), Y_FFT_SIN(S,13), Y_FFT_SIN(S,14),\
Y_FFT_SIN(S,15), Y_FFT_SIN(S,16), Y_FFT_SIN(S,17), Y_FFT_SIN(S,18), Y_FFT_SIN(S,19),\
Y_FFT_SIN(S,20), Y_FFT_SIN(S,21), Y_FFT_SIN(S,22), Y_FFT_SIN(S,23), Y_FFT_SIN(S,24),\
Y_FFT_SIN(S,25), Y_FFT_SIN(S,26), Y_FFT_SIN(S,27), Y_FFT_SIN(S,28), Y_FFT_SIN(S,29),\
Y_FFT_SIN(S,30), Y_FFT_SIN(S,31)



    //! instance for table of double
    const double fft1:: positive_sinus[sin_table_size] =
    {
        Y_FFT_TABLE(+)
    };

    //! instance for table of double
    const double fft1:: negative_sinus[sin_table_size] =
    {
        Y_FFT_TABLE(-)
    };


}
