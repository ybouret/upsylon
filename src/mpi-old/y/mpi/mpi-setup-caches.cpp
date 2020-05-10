
#include "y/mpi/mpi.hpp"

namespace upsylon
{
    namespace
    {
        struct __dtinfo
        {
            MPI_Datatype type;
            size_t       size;
        };
    }

    void mpi::setup_caches()
    {

#define Y_MPI_SZ(mpi_type,type) { mpi_type, sizeof(type) }

        const __dtinfo dtinfo_arr[] =
        {
            Y_MPI_SZ(MPI_CHAR,char),
            Y_MPI_SZ(MPI_UNSIGNED_CHAR,unsigned char),
            Y_MPI_SZ(MPI_BYTE,uint8_t),
            Y_MPI_SZ(MPI_SHORT,short),
            Y_MPI_SZ(MPI_UNSIGNED_SHORT,unsigned short),
            Y_MPI_SZ(MPI_INT,int),
            Y_MPI_SZ(MPI_UNSIGNED,unsigned int),
            Y_MPI_SZ(MPI_LONG,long),
            Y_MPI_SZ(MPI_UNSIGNED_LONG,unsigned long),
            Y_MPI_SZ(MPI_LONG_LONG,long),
            Y_MPI_SZ(MPI_UNSIGNED_LONG_LONG,unsigned long),
            Y_MPI_SZ(MPI_FLOAT,float),
            Y_MPI_SZ(MPI_DOUBLE,double)
        };

        const size_t dtinfo_num = sizeof(dtinfo_arr)/sizeof(dtinfo_arr[0]);
        bytes.ensure(dtinfo_num);
        for(size_t i=0;i<dtinfo_num;++i)
        {
            const __dtinfo  &dti = dtinfo_arr[i];
            assert( dti.type != MPI_DATATYPE_NULL );
            assert( dti.size >  0 );
            dtidx<< uint64_t(dti.type);
            bytes.push_back_(dti.size);
            assert(i+1==dtidx[ uint64_t(dti.type) ]);
        }

    }

}



