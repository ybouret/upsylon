#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, const char *argv[])
{

    /*--------------------------------------------------------------------------
     *
     * checking arguments
     *
     -------------------------------------------------------------------------*/
    if(argc<=1)
    {
        fprintf(stderr,"usage: %s required_num_threads [sampling time]", argv[0]);
        return EXIT_FAILURE;
    }
    else
    {
        double res     = 0;          /* global result                */
        int    required_num_threads; /* will parse from command line */
        int    num_threads=0;        /* will read from master thread */
        double sampling=0;           /* max sampling time            */
        int    cycles  =0;           /* number of cycles             */
        double sum=0,sum2=0;         /* times                        */
        int    i=0;

        /*----------------------------------------------------------------------
         *
         * parse the required number of threads
         *
         ---------------------------------------------------------------------*/
        required_num_threads = atoi(argv[1]);
        if(required_num_threads<1)
        {
            fprintf(stderr,"invalid required_num_threads=%d\n",required_num_threads);
            return EXIT_FAILURE;
        }

        /*----------------------------------------------------------------------
         *
         * parse optional sampling
         *
         ---------------------------------------------------------------------*/
        if(argc>2)
        {
            sampling = atof(argv[2]);
            if(sampling<0)
            {
                fprintf(stderr,"invalid sampling=%g\n",sampling);
                return EXIT_FAILURE;
            }
        }

        /*----------------------------------------------------------------------
         *
         * set this number and perform a quick block to check
         *
         ---------------------------------------------------------------------*/
        omp_set_num_threads(required_num_threads);

#pragma omp parallel
        {
            if( 0 == omp_get_thread_num() )
            {
                num_threads = omp_get_num_threads();
            }
        }
        if( num_threads != required_num_threads )
        {
            fprintf(stderr,"something went wrong!\n");
            return EXIT_FAILURE;
        }


        /*----------------------------------------------------------------------
         *
         * ok, do it now!
         *
         ---------------------------------------------------------------------*/
        do
        {
            const double startime = omp_get_wtime();
            double       ellapsed = 0;
            res = 0;
#pragma omp parallel for reduction(+:res)
            for(i=0;i<400000000;i++)
            {
                res=res+4.0*pow(-1.0,i)/(2.0*i+1.0);
            }
            ellapsed = omp_get_wtime()-startime;
            sum  += ellapsed;
            sum2 += ellapsed*ellapsed;
            ++cycles;
        } while( sum < sampling );

        /*----------------------------------------------------------------------
         *
         * statistics
         *
         ---------------------------------------------------------------------*/
        {
            const double average  = sum/cycles;
            const double std_dev  = sqrt(sum2*cycles - sum*sum)/cycles;
            const double std_err  = std_dev/sqrt( (double)cycles );
            fprintf(stdout,"%d %.15g %.15g %.15g\n", num_threads, 1000.0 * average, 1000.0 * std_err, res);
        }


        return EXIT_SUCCESS;
    }





}



