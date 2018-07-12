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

    template <> const double fft<double>::sin_table[];
    template <> const float  fft<float>::sin_table[];



}

#endif

