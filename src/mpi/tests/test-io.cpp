#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"
#include "y/ios/imstream.hpp"
#include "y/sequence/list.hpp"
#include "support.hpp"

using namespace upsylon;


namespace
{
    template <typename T>
    void ioBinary( mpi &MPI )
    {

        mpi::vBlock sendBlock;
        mpi::vBlock recvBlock;
        list<T>     primary;

        if(MPI.isHead)
        {
            const size_t ns = alea.leq(20);
            for(size_t i=ns;i>0;--i)
            {
                const T tmp = support::get<T>();
                primary.push_back(tmp);
                sendBlock.write_nbo(tmp);
            }
            MPI.print0(stderr,"Sending %u items, bytes=%u\n", unsigned(ns), unsigned( sendBlock.size() ));
        }

        MPI.print(stderr,"Before: send=%u | recv=%u\n", unsigned( sendBlock.size() ), unsigned( recvBlock.size() ) );
        size_t nr = 0;
        if(MPI.parallel)
        {
            if(MPI.isHead)
            {
                for(int r=1;r<MPI.size;++r)
                {
                    MPI.vSend(comm_variable_size, sendBlock, r, mpi::io_tag);
                }
                list<T> received;
                for(int r=1;r<MPI.size;++r)
                {
                    received.free();
                    MPI.vRecv(comm_variable_size, recvBlock, r, mpi::io_tag);
                    ios::imstream fp(recvBlock);
                    while( fp.is_active() )
                    {
                        size_t done = 0;
                        T      temp = 0;
                        if(!fp.query_nbo(temp,done))
                        {
                            throw exception("missing data");
                        }
                        received.push_back(temp);
                    }
                    MPI.print0(stderr, "received from %d: #%u\n", r, unsigned(received.size()) );
                }
            }
            else
            {
                MPI.vRecv(comm_variable_size, recvBlock, 0, mpi::io_tag);
                list<T> local;
                ios::imstream fp(recvBlock);
                while( fp.is_active() )
                {
                    size_t done = 0;
                    T      temp = 0;
                    if(!fp.query_nbo(temp,done))
                    {
                        throw exception("missing data");
                    }
                    local.push_back(temp);
                    ++nr;
                }
                MPI.vSend(comm_variable_size, recvBlock, 0, mpi::io_tag);
            }
        }
        else
        {

        }
        MPI.print(stderr,"After : send=%u | recv=%u => (%u)\n", unsigned( sendBlock.size() ), unsigned( recvBlock.size() ), unsigned(nr) );

        alea.fill( *sendBlock, sendBlock.size() );
        MPI.BcastAll(sendBlock,0);
        


    }

    template <typename T>
    void ioSerialRead( sequence<T> &seq, const mpi::vBytes &data )
    {
        ios::imstream fp(data);
        while( fp.is_active() )
        {
            size_t done = 0;
            const T tmp = T::read(fp,done,"ioSerialRead");
            seq.push_back(tmp);
        }
    }

    template <typename T>
    void ioSerial( mpi &MPI )
    {
        const char *id = typeid(T).name();
        MPI.print0(stderr, "\tioSerial<%s>\n", id);
        mpi::vBlock sendBlock;
        mpi::vBlock recvBlock;

        list<T>     sendObjs;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            const T tmp =  support::get<T>();
            sendObjs.push_back(tmp);
            tmp.serialize(sendBlock);
        }
        MPI.print(stderr,"Before : send=%u | recv=%u \n", unsigned( sendBlock.size() ), unsigned( recvBlock.size() ) );
        list<T> recvObjs;
        size_t  ns = 0;
        if(MPI.parallel)
        {
            if(MPI.isHead)
            {
                for(int r=1;r<MPI.size;++r)
                {
                    MPI.vSendRecv(comm_variable_size,
                                  sendBlock, r, mpi::io_tag,
                                  recvBlock, r, mpi::io_tag);
                    ioSerialRead<T>(recvObjs,recvBlock);
                    ns += sendObjs.size();
                }
            }
            else
            {
                MPI.vSendRecv(comm_variable_size,
                              sendBlock, 0, mpi::io_tag,
                              recvBlock, 0, mpi::io_tag);
                ioSerialRead<T>(recvObjs,recvBlock);
                ns += sendObjs.size();
            }
        }
        else
        {
            MPI.vSendRecv(comm_variable_size,
                          sendBlock, 0, mpi::io_tag,
                          recvBlock, 0, mpi::io_tag);
            ioSerialRead<T>(recvObjs,recvBlock);
            ns += sendObjs.size();
        }

        MPI.print(stderr, "Objects: sent=%u | recv=%u\n", unsigned( ns ), unsigned( recvObjs.size() ) );

        const size_t nr     = recvObjs.size();
        const size_t ns_tot = MPI.Allreduce(ns,MPI_SUM);
        const size_t nr_tot = MPI.Allreduce(nr,MPI_SUM);

        MPI.print0(stderr, "#ns=%u | #nr=%u\n", unsigned(ns_tot), unsigned(nr_tot) );

        if(ns_tot!=nr_tot)
        {
            throw exception("I/O mismatch for %s", id);
        }

        MPI.print0(stderr, "\n");

    }

}


Y_UTEST(io)
{
    Y_MPI(SINGLE);
    ioBinary<double>(MPI);
    ioBinary<float>(MPI);
    ioBinary<int64_t>(MPI);
    ioSerial<string>(MPI);
    ioSerial<mpn>(MPI);
    ioSerial<mpq>(MPI);

}
Y_UTEST_DONE()

