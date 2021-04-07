
//! \file
#ifndef Y_FFT1_INCLUDED
#define Y_FFT1_INCLUDED 1

#include "y/fft/xbitrev.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include <cmath>

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! 1D Fast Fourier Transform
    //
    //__________________________________________________________________________
    struct fft1
    {
        static const size_t sin_table_size = 32;         //!< precomputed sinus count
        static const double sin_table[ sin_table_size ]; //!< precomputed sinus table

        //______________________________________________________________________
        //
        //! single FFT of data[1..2*size]
        //______________________________________________________________________
        template <typename real_t> static inline
        void forward_(real_t      *data,
                      const size_t size) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            xbitrev::run_(data,size);

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
                    double       wtemp = sin_table[isln2]; //sin(0.5*theta);
                    const double wsq   = wtemp*wtemp;
                    double wpr         = -(wsq+wsq);
                    double wpi         = sin_table[mln2]; //sin(theta);
                    double wr          = 1.0;
                    double wi          = 0.0;

                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t rwr   = real_t(wr);
                            const real_t rwi   = real_t(wi);
                            const real_t tempr = rwr*d_j[0]-rwi*d_j[1];
                            const real_t tempi = rwr*d_j[1]+rwi*d_j[0];

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
        
      
        //! hard-wired fwd
        template <typename real_t, const size_t size> static inline
        void forward_(real_t *data) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            xbitrev_for<real_t,size>::run(data);

            //==================================================================
            // Lanczos Algorithm
            //==================================================================
            {
                static const size_t n    = size << 1;
                size_t              mmax = 2;
                size_t              mln2 = 1;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    double       wtemp = sin_table[isln2]; //sin(0.5*theta);
                    const double wsq   = wtemp*wtemp;
                    double wpr         = -(wsq+wsq);
                    double wpi         = sin_table[mln2]; //sin(theta);
                    double wr          = 1.0;
                    double wi          = 0.0;

                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t rwr   = real_t(wr);
                            const real_t rwi   = real_t(wi);
                            const real_t tempr = rwr*d_j[0]-rwi*d_j[1];
                            const real_t tempi = rwr*d_j[1]+rwi*d_j[0];

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
        template <typename real_t> static inline
        void reverse_(real_t      *data,
                      const size_t size) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            xbitrev::run_(data, size);

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
                    double       wtemp = -sin_table[isln2]; //sin(0.5*theta);
                    const double wsq   = wtemp*wtemp;
                    double wpr         = -(wsq+wsq);
                    double wpi         = -sin_table[mln2];
                    double wr          = 1.0;
                    double wi          = 0.0;

                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t rwr   = real_t(wr);
                            const real_t rwi   = real_t(wi);
                            const real_t tempr = rwr*d_j[0]-rwi*d_j[1];
                            const real_t tempi = rwr*d_j[1]+rwi*d_j[0];

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
        template <typename real_t, const size_t size> static inline
        void reverse_(real_t *data) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );
            
            //==================================================================
            // bit reversal algorithm
            //==================================================================
            xbitrev_for<real_t,size>::run(data);
            
            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                static const size_t n    = size << 1;
                size_t              mmax = 2;
                size_t              mln2 = 1;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    double       wtemp = -sin_table[isln2]; //sin(0.5*theta);
                    const double wsq   = wtemp*wtemp;
                    double wpr         = -(wsq+wsq);
                    double wpi         = -sin_table[mln2];
                    double wr          = 1.0;
                    double wi          = 0.0;
                    
                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t rwr   = real_t(wr);
                            const real_t rwi   = real_t(wi);
                            const real_t tempr = rwr*d_j[0]-rwi*d_j[1];
                            const real_t tempi = rwr*d_j[1]+rwi*d_j[0];
                            
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

}


#endif
