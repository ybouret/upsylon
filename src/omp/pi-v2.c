#define PI_OPTIMIZED
#include "pi.h"

static double *partial = 0;
static int     plength = 0;

/* cleaning up stuff */
static void cleanup(void)
{
    if( partial )
    {
        free(partial);
        partial = 0;
        plength = 0;
    }
}

int main(int argc, const char *argv[])
{

    /*--------------------------------------------------------------------------
     *
     * cleanup at exit
     *
     -------------------------------------------------------------------------*/
    if( 0 != atexit(cleanup) )
    {
        fprintf( stderr, "couldn't register cleanup at exit\n");
        return EXIT_FAILURE;
    }


    /*--------------------------------------------------------------------------
     *
     * checking arguments
     *
     -------------------------------------------------------------------------*/
    if(argc<=1)
    {
        fprintf(stderr,"usage: %s required_num_threads [sampling time]\n", argv[0]);
        return EXIT_FAILURE;
    }
    else
    {
        double  res     = 0;          /* global result                */
        int     required_num_threads; /* will parse from command line */
        int     num_threads=0;        /* will read from master thread */
        double  sampling=0;           /* max sampling time            */
        int     cycles  =0;           /* number of cycles             */
        double  sum=0,sum2=0;         /* times                        */
        int     i=0;


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
         * acquire resources
         *
         ---------------------------------------------------------------------*/
        plength = num_threads * sizeof(double);
        partial = (double *)malloc(plength);
        if(!partial)
        {
            fprintf(stderr, "couldn't allocate partial\n");
            return EXIT_FAILURE;
        }

        /*----------------------------------------------------------------------
         *
         * ok, do it now!
         *
         ---------------------------------------------------------------------*/
        do
        {
            double       ellapsed = 0;
            const double startime = omp_get_wtime();

#pragma omp parallel
            {
                double tmp = 0.0;
#pragma     omp for
                for(i=0;i<400000000;i++)
                {
                    tmp += pi_term(i);
                }
                partial[ omp_get_thread_num() ] = tmp;
            }

            res = 0;
            for(i=0;i<num_threads;++i) res += partial[i];
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



