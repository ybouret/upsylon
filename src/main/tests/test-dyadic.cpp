#include "y/memory/dyadic.hpp"
#include "y/memory/pooled.hpp"
#include "y/memory/global.hpp"

#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
namespace
{
    template <typename SEQ>
    void display( SEQ &seq )
    {
        std::cerr << std::endl;
        std::cerr << "seq.size=" << seq.size() << "/" << seq.capacity() << std::endl;

        seq.free();
        seq.ensure(100);
        std::cerr << "seq.size=" << seq.size() << "/" << seq.capacity() << std::endl;

    }

}
Y_UTEST(dyadic)
{

    vector<char,memory::global> vg(1); display(vg);
    vector<char,memory::pooled> vp(1); display(vp);
    vector<char,memory::dyadic> vd(1); display(vd);

}
Y_UTEST_DONE()
