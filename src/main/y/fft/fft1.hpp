
//! \file
#ifndef Y_FFT1_INCLUDED
#define Y_FFT1_INCLUDED 1

#include "y/fft/xbra.hpp"
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
        static const double positive_sinus[ sin_table_size ]; //!< precomputed sinus table
        static const double negative_sinus[ sin_table_size ]; //!< precomputed sinus table


        //______________________________________________________________________
        //
        //! generic forward FFT of data[1..2*size], unscaled
        //______________________________________________________________________
        template <typename real_t> static inline
        void forward_(real_t      *data,
                     const size_t size) throw()
        {
            process(data,size,positive_sinus);
        }

        //______________________________________________________________________
        //
        //! specific forward FFT of data[1..2*size], unscaled
        //______________________________________________________________________
        template <typename real_t, const size_t size> static inline
        void forward(real_t *data) throw()
        {
            process<real_t,size>(data,positive_sinus);
        }


#define Y_FFT1_FWD(SIZE) case SIZE: process<real_t,SIZE>(data,positive_sinus); break

#define Y_FFT1_REPEAT(MACRO) \
MACRO(1);   MACRO(2);   MACRO(4);   MACRO(8);    MACRO(16);   MACRO(32); MACRO(64);\
MACRO(128); MACRO(256); MACRO(512); MACRO(1024); MACRO(2048); MACRO(4096); MACRO(8192)

        //______________________________________________________________________
        //
        //! dispatched forward FFT of data[1..2*size], unscaled
        //______________________________________________________________________
        template <typename real_t> static inline
        void forward(real_t      *data,
                     const size_t size) throw()
        {
            switch(size)
            {
                case  0: break; Y_FFT1_REPEAT(Y_FFT1_FWD);
                default: process<real_t>(data,size,positive_sinus);
            }
         }



        //______________________________________________________________________
        //
        //! reverse FFT of data[1..2*size], unscaled
        //______________________________________________________________________
        template <typename real_t> static inline
        void reverse_(real_t      *data,
                     const size_t size) throw()
        {
            process(data,size,negative_sinus);
        }

        //______________________________________________________________________
        //
        //! reverse FFT of data[1..2*size], unscaled
        //______________________________________________________________________
        template <typename real_t, const size_t size> static inline
        void reverse(real_t *data) throw()
        {
            process<real_t,size>(data,negative_sinus);
        }

#define Y_FFT1_REV(SIZE) case SIZE: process<real_t,SIZE>(data,negative_sinus); break


        //______________________________________________________________________
        //
        //! dispatched reverse FFT of data[1..2*size], unscaled
        //______________________________________________________________________
        template <typename real_t> static inline
        void reverse(real_t      *data,
                     const size_t size) throw()
        {
            switch(size)
            {
                case  0: break; Y_FFT1_REPEAT(Y_FFT1_REV);
                default: process<real_t>(data,size,negative_sinus);
            }
        }
        

    private:
#define Y_FFT1_LOOP() \
/**/size_t mmax = 2;\
/**/size_t mln2 = 1;\
/**/while (n > mmax) {\
/**/  const size_t istep = mmax << 1;\
/**/  const size_t isln2 = mln2+1;\
/**/  double       wtemp = table[isln2]; \
/**/  const double wsq   = wtemp*wtemp; \
/**/  double wpr         = -(wsq+wsq); \
/**/  double wpi         = table[mln2]; \
/**/  double wr          = 1.0;\
/**/  double wi          = 0.0;\
/**/  \
/**/  for (size_t m=1; m<mmax; m+=2)\
/**/  {\
/**/    for (size_t i=m; i<=n; i+=istep)\
/**/    {\
/**/      real_t      *d_i   = data+i;\
/**/      const size_t j     = i+mmax;\
/**/      real_t      *d_j   = data+j;\
/**/      const real_t rwr   = real_t(wr);\
/**/      const real_t rwi   = real_t(wi);\
/**/      const real_t tempr = rwr*d_j[0]-rwi*d_j[1];\
/**/      const real_t tempi = rwr*d_j[1]+rwi*d_j[0];\
/**/      \
/**/      d_j[0]  = d_i[0]-tempr;\
/**/      d_j[1]  = d_i[1]-tempi;\
/**/      d_i[0] += tempr;\
/**/      d_i[1] += tempi;\
/**/    }\
/**/    wr=(wtemp=wr)*wpr-wi*wpi+wr;\
/**/    wi=wi*wpr+wtemp*wpi+wi;\
/**/  }\
/**/  mmax=istep;\
/**/  mln2=isln2;\
/**/}

        //______________________________________________________________________
        //
        //! process FFT of data[1..2*size] with a sin table
        //______________________________________________________________________
        template <typename real_t> static inline
        void process(real_t              *data,
                     const size_t         size,
                     const double * const table) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            xbra::run_(data,size);

            //==================================================================
            // Lanczos Algorithm
            //==================================================================
            const size_t n    = size << 1;
            Y_FFT1_LOOP()
        }

        //______________________________________________________________________
        //
        //! process FFT of data[1..2*SIZE] with a sin table
        //______________________________________________________________________
        template <typename real_t, const size_t size> static inline
        void process(real_t              *data,
                     const double * const table) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            xbra_for<real_t,size>::run(data);

            //==================================================================
            // Lanczos Algorithm
            //==================================================================
            static const size_t n = size << 1;
            Y_FFT1_LOOP()
        }


    };

}


#endif
