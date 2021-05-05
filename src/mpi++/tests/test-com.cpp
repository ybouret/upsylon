

#include "y/mpi++/mpi.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{
    template <typename T> static inline
    void check(const mpi &MPI)
    {
        const rtti             &info = rtti::of<T>();
        const mpi::system_type &st   = MPI.systemTypeFor<T>();
        MPI.Printf0(stderr, "%10s using %10s\n", info.text(), st.flux.info.text() );
        if(!MPI.parallel) return;

        T data = support::get<T>();
        if(MPI.head)
        {
            for(int r=1;r<MPI.size;++r)
            {
                MPI.Send<T>(data,r);
            }
        }
        else
        {
            data = MPI.Recv<T>(0);
        }


    }

    
}

Y_UTEST(com)
{
    Y_MPI(SINGLE);

    check<char>(MPI);
    check<short>(MPI);
    check<int>(MPI);
    check<long>(MPI);
    check<long long>(MPI);

    check<unsigned char>(MPI);
    check<unsigned short>(MPI);
    check<unsigned int>(MPI);
    check<unsigned long>(MPI);
    check<unsigned long long>(MPI);

    check<int8_t>(MPI);
    check<int16_t>(MPI);
    check<int32_t>(MPI);
    check<int64_t>(MPI);

    check<uint8_t>(MPI);
    check<uint16_t>(MPI);
    check<uint32_t>(MPI);
    check<uint64_t>(MPI);

    check<float>(MPI);
    check<double>(MPI);
    
    const string rep = MPI.report();
    MPI.Printf(stderr,"%s\n",*rep);
}
Y_UTEST_DONE()
