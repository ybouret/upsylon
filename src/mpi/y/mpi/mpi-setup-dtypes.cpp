#include "y/mpi/mpi.hpp"

namespace upsylon
{
    namespace
    {
        template <typename T> static inline
        void __register(mpi::data_type::db &types,
                        MPI_Datatype        v )
        {
            const mpi::data_type   dt( type_spec_of<T>(),v);
            (void) types.insert(dt);
        }
    }
    void mpi::setup_dtypes()
    {
        //__________________________________________________________________
        //
        // fill in database of type
        //__________________________________________________________________
#define Y_MPI_REG_CASE(PFX,SFX,COM_TYPE) case (sizeof(PFX##SFX)): __register< PFX##SFX >(types,COM_TYPE); break
        
#define Y_MPI_REG(PFX,STD_TYPE,COM_TYPE)            \
do { __register<STD_TYPE>(types,COM_TYPE);          \
if(sizeof(STD_TYPE)==usr_size) usr_type = COM_TYPE; \
switch(sizeof(STD_TYPE))\
{\
Y_MPI_REG_CASE(PFX,8_t, COM_TYPE);\
Y_MPI_REG_CASE(PFX,16_t,COM_TYPE);\
Y_MPI_REG_CASE(PFX,32_t,COM_TYPE);\
Y_MPI_REG_CASE(PFX,64_t,COM_TYPE);\
default: break;\
}\
} while(false)
        {
            static const size_t usr_size = sizeof(ptrdiff_t);
            MPI_Datatype        usr_type = MPI_DATATYPE_NULL;

            Y_MPI_REG(int,char,      MPI_CHAR);
            Y_MPI_REG(int,short,     MPI_SHORT);
            Y_MPI_REG(int,int,       MPI_INT);
            Y_MPI_REG(int,long,      MPI_LONG);
            Y_MPI_REG(int,long long, MPI_LONG_LONG);

            if(MPI_DATATYPE_NULL==usr_type) throw upsylon::exception("couldn't find mpi matching type for ptrdiff_t");
            __register<ptrdiff_t>(types,usr_type);
        }

        {
            static const size_t usr_size = sizeof(size_t);
            MPI_Datatype        usr_type = MPI_DATATYPE_NULL;

            Y_MPI_REG(uint,unsigned char,      MPI_UNSIGNED_CHAR);
            Y_MPI_REG(uint,unsigned short,     MPI_UNSIGNED_SHORT);
            Y_MPI_REG(uint,unsigned int,       MPI_UNSIGNED);
            Y_MPI_REG(uint,unsigned long,      MPI_UNSIGNED_LONG);
            Y_MPI_REG(uint,unsigned long long, MPI_UNSIGNED_LONG_LONG);

            if(MPI_DATATYPE_NULL==usr_type) throw upsylon::exception("couldn't find mpi matching type for size_t");
            __register<size_t>(types,usr_type);
        }

        {
            __register<float >(types,MPI_FLOAT);
            __register<double>(types,MPI_DOUBLE);
        }
    }

}


