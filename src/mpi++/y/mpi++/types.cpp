
#include "y/mpi++/mpi.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/rtti.hpp"
#include "y/associative/suffix/node-to.hpp"
#include "y/associative/native-key.hpp"

namespace upsylon
{

    typedef native_key<int64_t> internal_key;
    
    mpi::system_type:: system_type(const MPI_Datatype dt,
                                   commFlux          &fx,
                                   const rtti        &id) throw() :
    type(dt),
    flux(fx),
    info(id)
    {
    }

    mpi::system_type:: system_type(const system_type &other) throw() :
    type(other.type),
    flux(other.flux),
    info(other.info)
    {
    }

    mpi:: system_type:: ~system_type() throw()
    {
        aliasing::_(type) = MPI_DATATYPE_NULL;
    }


    template <typename T>
    static inline void __register(mpi::system_type::store  &db,
                                  const MPI_Datatype        dt,
                                  mpi::commFlux            &fx)
    {
        const rtti            &key = rtti::of( typeid(T) );
        const mpi::system_type mdt(dt,fx,key);
        (void) db.insert(key,mdt);
    }

    unsigned mpi:: index_of(const MPI_Datatype dt) const
    {
        const internal_key        k = internal_key::type(dt);
        const unsigned            i = dataHash(k);
        if(mphash::invalid==i) throw upsylon::exception("%s(invalid index_of datatype)",call_sign);
        return i;
    }

#define Y_MPI_SYS_FIND(TYPE) do {\
/**/ const rtti &key = rtti::of<TYPE>();\
/**/   if(key.size==tid.size) {\
/**/     const mpi::system_type *p = db.search(key); assert(p);\
/**/     if(!db.insert(tid,*p))\
/**/       throw upsylon::exception("%s(failure to register <%s>)",mpi::call_sign,tid.text());\
/**/     return;\
/**/   }\
/**/ } while(false)

    template <typename T>
    static inline void __register_signed(mpi::system_type::store &db)
    {
        const rtti &tid = rtti::of<T>();
        if(db.search(tid)) return;

        Y_MPI_SYS_FIND(char);
        Y_MPI_SYS_FIND(short);
        Y_MPI_SYS_FIND(int);
        Y_MPI_SYS_FIND(long);
        Y_MPI_SYS_FIND(long long);

        throw upsylon::exception("%s(cannot match <%s>)", mpi::call_sign, tid.text() );
    }

    template <typename T>
    static inline void __register_unsigned(mpi::system_type::store &db)
    {
        const rtti &tid = rtti::of<T>();
        if(db.search(tid)) return;

        Y_MPI_SYS_FIND(unsigned char);
        Y_MPI_SYS_FIND(unsigned short);
        Y_MPI_SYS_FIND(unsigned);
        Y_MPI_SYS_FIND(unsigned long);
        Y_MPI_SYS_FIND(unsigned long long);

        throw upsylon::exception("%s(cannot match <%s>)", mpi::call_sign, tid.text() );
    }


    void mpi:: build_data_types()
    {
        //----------------------------------------------------------------------
        //
        // create table of MPI_Datatype => index in 1..num
        //
        //----------------------------------------------------------------------

#define Y_MPI_IDATA(TYPE) \
/**/ do {\
/**/   const internal_key k = internal_key::type(TYPE);\
/**/   if(!tdb.search(k)) tdb(k,++idx);\
/**/ } while(false)

        {
            size_t   idx = 0;
            mphash  &tdb = aliasing::_(dataHash);
            Y_MPI_IDATA(MPI_CHAR);
            Y_MPI_IDATA(MPI_SHORT);
            Y_MPI_IDATA(MPI_INT);
            Y_MPI_IDATA(MPI_LONG);
            Y_MPI_IDATA(MPI_LONG_LONG);
            Y_MPI_IDATA(MPI_UNSIGNED_CHAR);
            Y_MPI_IDATA(MPI_UNSIGNED_SHORT);
            Y_MPI_IDATA(MPI_UNSIGNED);
            Y_MPI_IDATA(MPI_UNSIGNED_LONG);
            Y_MPI_IDATA(MPI_UNSIGNED_LONG_LONG);
            Y_MPI_IDATA(MPI_FLOAT);
            Y_MPI_IDATA(MPI_DOUBLE);
        }

        //----------------------------------------------------------------------
        //
        // build comms
        //
        //----------------------------------------------------------------------
        const size_t       ncom = dataHash.size();
        crates<commFlux>  &coms = aliasing::_(ioFluxes); assert(0==coms.size());
        if(ncom>ioFluxes.capacity())
            throw upsylon::exception("%s(not enough fluxes to hold MPI_Datatypes)",call_sign);
        coms.vbuild(ncom);
        assert(coms.size() == ncom);

        //----------------------------------------------------------------------
        //
        // register system types
        //
        //----------------------------------------------------------------------
#define Y_MPI_SYSREG(TYPE,MPI_TYPE) \
/**/  do { \
/**/    __register<TYPE>(aliasing::_(sysTypes),MPI_TYPE,coms[index_of(MPI_TYPE)]);\
/**/  } while(false)
        

        Y_MPI_SYSREG(char,MPI_CHAR);
        Y_MPI_SYSREG(short,MPI_SHORT);
        Y_MPI_SYSREG(int,MPI_INT);
        Y_MPI_SYSREG(long,MPI_LONG);
        Y_MPI_SYSREG(long long,MPI_LONG_LONG);

        Y_MPI_SYSREG(unsigned char,MPI_UNSIGNED_CHAR);
        Y_MPI_SYSREG(unsigned short,MPI_UNSIGNED_SHORT);
        Y_MPI_SYSREG(unsigned,MPI_UNSIGNED);
        Y_MPI_SYSREG(unsigned long,MPI_UNSIGNED_LONG);
        Y_MPI_SYSREG(unsigned long long,MPI_UNSIGNED_LONG_LONG);

        Y_MPI_SYSREG(float,MPI_FLOAT);
        Y_MPI_SYSREG(double,MPI_DOUBLE);

#define Y_MPI_SYSREG_I(TYPE) __register_signed<TYPE>(aliasing::_(sysTypes))

        Y_MPI_SYSREG_I(int8_t);
        Y_MPI_SYSREG_I(int16_t);
        Y_MPI_SYSREG_I(int32_t);
        Y_MPI_SYSREG_I(int64_t);
        Y_MPI_SYSREG_I(ptrdiff_t);

#define Y_MPI_SYSREG_U(TYPE) __register_unsigned<TYPE>(aliasing::_(sysTypes))

        Y_MPI_SYSREG_U(uint8_t);
        Y_MPI_SYSREG_U(uint16_t);
        Y_MPI_SYSREG_U(uint32_t);
        Y_MPI_SYSREG_U(uint64_t);
        Y_MPI_SYSREG_U(size_t);


       
        
        
#define Y_MPI_CHK(type) do {\
assert( sysTypes.search( rtti::of<type>() ) );\
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

    const mpi::system_type & mpi:: system_type_for(const std::type_info &tid) const
    {
        const rtti        &k = rtti::of(tid);
        const system_type *p = sysTypes.search(k);
        if(!p) throw upsylon::exception("%s(missing system_type_for<%s>)", call_sign, k.text());
        return *p;
    }

    void mpi:: display_types() const
    {
        for( system_type::store::const_iterator it=sysTypes.begin(); it != sysTypes.end(); ++it)
        {
            const void           *ptr =  suffix_node_::to_address( system_type::store::iter_node(it) );
            const rtti           &tid = *static_cast<const rtti *>(ptr);
            fprintf(stderr,"\t%16s\n",tid.text());
        }
    }

}

