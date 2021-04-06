
#include "y/fft/fft1.hpp"

namespace upsylon
{

    //! compute a fraction of size, double version
#define Y_FFT_SIN(LN2) sin(6.28318530717958623199592693708837032318115234375/(size_t(1)<<(LN2)))
    //! generate a table of sine fractions
#define Y_FFT_TABLE()                                                     \
Y_FFT_SIN(0),  Y_FFT_SIN(1),  Y_FFT_SIN(2),  Y_FFT_SIN(3),  Y_FFT_SIN(4), \
Y_FFT_SIN(5),  Y_FFT_SIN(6),  Y_FFT_SIN(7),  Y_FFT_SIN(8),  Y_FFT_SIN(9), \
Y_FFT_SIN(10), Y_FFT_SIN(11), Y_FFT_SIN(12), Y_FFT_SIN(13), Y_FFT_SIN(14),\
Y_FFT_SIN(15), Y_FFT_SIN(16), Y_FFT_SIN(17), Y_FFT_SIN(18), Y_FFT_SIN(19),\
Y_FFT_SIN(20), Y_FFT_SIN(21), Y_FFT_SIN(22), Y_FFT_SIN(23), Y_FFT_SIN(24),\
Y_FFT_SIN(25), Y_FFT_SIN(26), Y_FFT_SIN(27), Y_FFT_SIN(28), Y_FFT_SIN(29),\
Y_FFT_SIN(30), Y_FFT_SIN(31)



    //! instance for table of double
    const double fft1:: sin_table[sin_table_size] =
    {
        Y_FFT_TABLE()
    };

}
