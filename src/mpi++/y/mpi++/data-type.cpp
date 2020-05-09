
#include "y/mpi++/mpi.hpp"
#include "y/type/aliasing.hpp"
#include "y/associative/be-key.hpp"
#include <typeinfo>

namespace upsylon
{
    
    mpi:: data_type:: data_type(const MPI_Datatype value) throw() : uuid(value)
    {
    }

    mpi:: data_type:: data_type(const data_type &_) throw() : uuid(_.uuid)
    {
    }

    mpi:: data_type:: ~data_type() throw()
    {
        aliasing::_(uuid) = MPI_DATATYPE_NULL;
    }
    
    void mpi:: build_data_types()
    {
        {
            const data_type       mdt = MPI_CHAR;
            const be_key          key = typeid(char);
            (void) data_types.insert_by(key,mdt);
        }
        
        {
            const data_type       mdt = MPI_INT;
            const be_key          key = typeid(int);
            (void) data_types.insert_by(key,mdt);
        }
        
        
        if(head)
        {
            data_types.get_root().graphViz("mpi-data-types.dot");
        }
    }
}

