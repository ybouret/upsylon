
#include "y/mpi++/mpi.hpp"
#include "y/type/aliasing.hpp"
#include "y/associative/be-key.hpp"
#include <typeinfo>

namespace upsylon
{
    
    mpi:: data_type:: data_type(const MPI_Datatype value, const size_t bytes) throw() :
    uuid(value),
    size(bytes)
    {
    }
    
    mpi:: data_type:: data_type(const data_type &_) throw() :
    uuid(_.uuid),
    size(_.size)
    {
    }
    
    mpi:: data_type:: ~data_type() throw()
    {
        aliasing::_(uuid) = MPI_DATATYPE_NULL;
        aliasing::_(size) = 0;
    }
    
    template <typename T>
    static inline void __register(suffix_tree<mpi::data_type> &db,
                                  const MPI_Datatype           dt)
    {
        const be_key         key = typeid(T);
        const mpi::data_type mdt(dt,sizeof(T));
        (void) db.insert_by(key,mdt);
    }
 
#define Y_MPI_FIND(type) do { \
const be_key          key = typeid(type);\
const mpi::data_type *ptr = db.search_by(key);\
assert(ptr);\
if(sz==ptr->size) return ptr->uuid;\
} while(false)

    static inline
    MPI_Datatype __ifind(const suffix_tree<mpi::data_type> &db,
                         const unsigned                     sz)
    {
        Y_MPI_FIND(char);
        Y_MPI_FIND(short);
        Y_MPI_FIND(int);
        Y_MPI_FIND(long);
        Y_MPI_FIND(long long);

        throw exception("mpi: no registered sizeof(signed data type)=%u",sz);
    }
    
    static inline
    MPI_Datatype __ufind(const suffix_tree<mpi::data_type> &db,
                         const unsigned                     sz)
    {
        Y_MPI_FIND(unsigned char);
        Y_MPI_FIND(unsigned short);
        Y_MPI_FIND(unsigned);
        Y_MPI_FIND(unsigned long);
        Y_MPI_FIND(unsigned long long);
        
        throw exception("mpi: no registered sizeof(unsigned data type)=%u",sz);
    }
    
    
    void mpi:: build_data_types()
    {
#define Y_MPI_REG(type,TYPE) do {\
__register<type>(types,TYPE);\
assert(TYPE         == data_type_for<type>().uuid);\
assert(sizeof(type) == data_type_for<type>().size);\
} while(false)

        
        Y_MPI_REG(char,MPI_CHAR);
        Y_MPI_REG(short,MPI_SHORT);
        Y_MPI_REG(int,MPI_INT);
        Y_MPI_REG(long,MPI_LONG);
        Y_MPI_REG(long long,MPI_LONG_LONG);
        
#define Y_MPI_IREG(type) __register<type>(types,__ifind(types,sizeof(type)) );
        
        Y_MPI_IREG(int8_t);
        Y_MPI_IREG(int16_t);
        Y_MPI_IREG(int32_t);
        Y_MPI_IREG(int64_t);

        
         
        Y_MPI_REG(unsigned char,MPI_UNSIGNED_CHAR);
        Y_MPI_REG(unsigned short,MPI_UNSIGNED_SHORT);
        Y_MPI_REG(unsigned int,MPI_UNSIGNED);
        Y_MPI_REG(unsigned long,MPI_UNSIGNED_LONG);
        Y_MPI_REG(unsigned long long,MPI_UNSIGNED_LONG_LONG);
        
        
#define Y_MPI_UREG(type) __register<type>(types,__ufind(types,sizeof(type)) );
        
        Y_MPI_UREG(uint8_t);
        Y_MPI_UREG(uint16_t);
        Y_MPI_UREG(uint32_t);
        Y_MPI_UREG(uint64_t);
        
        __register<float>(types,MPI_FLOAT);
        __register<double>(types,MPI_DOUBLE);
        


        if(head)
        {
            types.get_root().graphViz("mpi-data-types.dot");

#define Y_MPI_CHK(type) do {\
assert( sizeof(type) == data_type_for<type>().size );\
} while(false)
            
            Y_MPI_CHK(char);
            Y_MPI_CHK(unsigned char);
            Y_MPI_CHK(short);
            Y_MPI_CHK(unsigned short);
            Y_MPI_CHK(int);
            Y_MPI_CHK(unsigned int);
            Y_MPI_CHK(long);
            Y_MPI_CHK(unsigned long);
            Y_MPI_CHK(long long);
            Y_MPI_CHK(unsigned long long);
            
            Y_MPI_CHK(float);
            Y_MPI_CHK(double);
            
            Y_MPI_CHK(int8_t);
            Y_MPI_CHK(int16_t);
            Y_MPI_CHK(int32_t);
            Y_MPI_CHK(int64_t);
            
            Y_MPI_CHK(uint8_t);
            Y_MPI_CHK(uint16_t);
            Y_MPI_CHK(uint32_t);
            Y_MPI_CHK(uint64_t);
            
            Y_MPI_CHK(size_t);
            Y_MPI_CHK(ptrdiff_t);
        }
    }
    
    const mpi::data_type &mpi:: data_type_for( const std::type_info &tid ) const
    {
        const be_key     key = tid;
        const data_type *ptr = types.search_by(key);
        if(!ptr) throw upsylon::exception("missing mpi::data_type for <%s>", tid.name() );
        return *ptr;
    }
    
}

