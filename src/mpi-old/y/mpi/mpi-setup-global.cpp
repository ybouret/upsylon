#include "y/mpi/mpi.hpp"

namespace upsylon
{

    void mpi::setup_global()
    {
        Y_MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD,(int*)&size));
        Y_MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD,(int*)&rank));
        (int  &)last          = size-1;
        (bool &)parallel      = (size>1);
        (bool &)isHead        = (0==rank);
        (bool &)isTail        = (last==rank);
        (bool &)isBulk        = (!isHead) && (!isTail);

        {
            char pname[MPI_MAX_PROCESSOR_NAME+1] = { 0 };
            int  psize=0;
            Y_MPI_CHECK(MPI_Get_processor_name(pname,&psize));
            string tmp(pname,psize);
            tmp.swap_with( (string &)processorName );
        }

        {
            const char *fmt = "<%d.%d> ";
            if(size>=10)
            {
                fmt = "<%2d.%02d> ";
            }

            if(size>=100)
            {
                fmt = "<%3d.%03d> ";
            }

            string tmp = vformat(fmt,size,rank);
            tmp.swap_with( (string&)nodeName );
        }

    }

}
