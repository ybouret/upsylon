#include "y/mpi/mpi.hpp"

namespace upsylon
{

    namespace
    {
        int  *   __mpi_argc = 0;
        char *** __mpi_argv = 0;
        int      __mpi_levl = 0;
        bool     __mpi_call = false;

        static inline void __cleanup() throw()
        {
            __mpi_argc=0;
            __mpi_argv=0;
            __mpi_levl=0;
            __mpi_call=false;
        }

        
    }

    mpi:: ~mpi() throw()
    {
        finalizeAndCleanUp();
    }

    mpi & mpi:: init( int * argc, char ***argv, int requestedThreadLevel )
    {
        if( exists() )
        {
            throw upsylon::exception("mpi.init( already exist )");
        }
        __mpi_argc = argc;
        __mpi_argv = argv;
        __mpi_levl = requestedThreadLevel;
        __mpi_call = true;
        try
        {
            mpi &MPI = instance();
            __cleanup();
            return MPI;
        }
        catch(...)
        {
            __cleanup();
            throw;
        }
    }

#define Y_MPI_CLEAN(FIELD) memset( (void *) &FIELD, 0, sizeof(FIELD) )

    void mpi:: finalizeAndCleanUp() throw()
    {
        (void)MPI_Finalize();
        Y_MPI_CLEAN(size);
        Y_MPI_CLEAN(rank);
        Y_MPI_CLEAN(last);
        Y_MPI_CLEAN(parallel);
        Y_MPI_CLEAN(isHead);
        Y_MPI_CLEAN(isTail);
        Y_MPI_CLEAN(isBulk);
        Y_MPI_CLEAN(threadLevel);
        Y_MPI_CLEAN(fullCommTicks);
        Y_MPI_CLEAN(lastCommTicks);
    }

    namespace
    {
        template <typename T>
        void __register(mpi::data_type::db &types,
                        MPI_Datatype        v )
        {
            const std::type_info & info = typeid(T);
            const mpi::data_type   dt(info,v);
            (void) types.insert(dt);
        }

        struct __dtinfo
        {
            MPI_Datatype type;
            size_t       size;
        };

    }


    mpi:: mpi() :
    size(0),
    rank(0),
    last(0),
    parallel(false),
    isHead(false),
    isTail(false),
    isBulk(false),
    threadLevel(-1),
    fullCommTicks(0),
    lastCommTicks(0),
    processorName(),
    nodeName(),
    send_pack(),
    recv_pack(),
    send_(),
    recv_(),
    types(32,as_capacity),
    bytes(),
    dtidx(1),
    send( send_ ),
    recv( recv_ )
    {
        //______________________________________________________________________
        //
        // System level init
        //______________________________________________________________________
        if(!__mpi_call) throw upsylon::exception("please call mpi.init(...)");
        Y_MPI_CHECK(MPI_Init_thread(__mpi_argc,__mpi_argv,__mpi_levl, (int *)&threadLevel));

        try
        {

            //__________________________________________________________________
            //
            // Get topology
            //__________________________________________________________________
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

            //__________________________________________________________________
            //
            // fill in database of sizes
            //__________________________________________________________________

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
        catch(...)
        {
            finalizeAndCleanUp();
            throw;
        }
    }

    void mpi:: displayTypes( FILE *fp ) const
    {
        if(0==rank)
        {
            {
                //data_type_hasher H;
                fprintf(fp,"<MPI::DataTypes count=\"%u\">\n", unsigned( types.size() ));
                for(data_type::db::iterator i=types.begin();i!=types.end();++i)
                {
                    const data_type    &t = *i;
                    fprintf(fp,"\t<%s>:\n", t.label.name());
                }
                fprintf(fp,"<MPI::DataTypes>\n");
            }

        }
    }



#define Y_MPI_THR(ID) case MPI_THREAD_##ID: return "MPI_THREAD_" #ID
    const char * mpi:: threadLevelText() const throw()
    {
        switch(threadLevel)
        {
                Y_MPI_THR(SINGLE);
                Y_MPI_THR(FUNNELED);
                Y_MPI_THR(SERIALIZED);
                Y_MPI_THR(MULTIPLE);

            default: break;
        }
        return "MPI_THREAD_UNKNOWN";
    }


    const MPI_Datatype & mpi:: get_data_type( const std::type_info &t ) const
    {
        data_type::db   &db = types;
        const data_type *p  = db.search(t);
        if(!p) throw upsylon::exception("mpi: unregistered type <%s>", t.name() );
        return p->value;
    }

}





