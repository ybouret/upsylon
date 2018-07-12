// !\file
#ifndef Y_FFT_INCLUDED
#define Y_FFT_INCLUDED 1

#include "y/fft/xbitrev.hpp"
#include "y/type/utils.hpp"
#include <cmath>

namespace upsylon
{

    //! Fast Fourier Transform routines
    template <typename real_t>
    struct fft
    {
        static const real_t sin_table[]; //!< table of precomputed sine fractions

        //______________________________________________________________________
        //
        //! single FFT of data[1..2*size]
        //______________________________________________________________________
        static inline
        void forward(real_t      *data,
                     const size_t size) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            xbitrev::run(data,size);

            //==================================================================
            // Lanczos Algorithm
            //==================================================================
            {
                const size_t n    = size << 1;
                size_t       mmax = 2;
                size_t       mln2 = 1;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    real_t       wtemp = sin_table[isln2]; //sin(0.5*theta);
                    const real_t wsq   = wtemp*wtemp;
                    real_t wpr         = -(wsq+wsq);
                    real_t wpi         = sin_table[mln2]; //sin(theta);
                    real_t wr          = 1;
                    real_t wi          = 0;

                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t tempr = wr*d_j[0]-wi*d_j[1];
                            const real_t tempi = wr*d_j[1]+wi*d_j[0];

                            d_j[0]  = d_i[0]-tempr;
                            d_j[1]  = d_i[1]-tempi;
                            d_i[0] += tempr;
                            d_i[1] += tempi;
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                    mln2=isln2;
                }
            }
        }

        //______________________________________________________________________
        //
        //!  inverse FFT of data[1..2*size]
        //______________________________________________________________________
        static  inline
        void reverse(real_t      *data,
                     const size_t size) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );
            
            //==================================================================
            // bit reversal algorithm
            //==================================================================
            xbitrev::run(data, size);

            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                const size_t n    = size << 1;
                size_t       mmax = 2;
                size_t       mln2 = 1;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    real_t       wtemp = -sin_table[isln2]; //sin(0.5*theta);
                    const real_t wsq   = wtemp*wtemp;
                    real_t wpr         = -(wsq+wsq);
                    real_t wpi         = -sin_table[mln2];
                    real_t wr          = 1;
                    real_t wi          = 0;

                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t tempr = wr*d_j[0]-wi*d_j[1];
                            const real_t tempi = wr*d_j[1]+wi*d_j[0];

                            d_j[0]  = d_i[0]-tempr;
                            d_j[1]  = d_i[1]-tempi;
                            d_i[0] += tempr;
                            d_i[1] += tempi;
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                    mln2=isln2;
                }
            }
        }
    };



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
    template <>
    const double fft<double>:: sin_table[32] =
    {
        Y_FFT_TABLE()
    };

#undef  Y_FFT_SIN
    //! compute a fraction of sine, float version
#define Y_FFT_SIN(LN2) sinf(6.28318530717958623199592693708837032318115234375f/(size_t(1)<<(LN2)))

    //! instance for  table of float
    template <>
    const float fft<float>:: sin_table[32] =
    {
        Y_FFT_TABLE()
    };

#undef Y_FFT_SIN
#undef Y_FFT_TABLE

}

#endif

