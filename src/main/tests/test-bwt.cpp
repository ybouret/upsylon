#include "y/information/bwt.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"

#include "y/information/delta.hpp"
#include "y/information/mtf.hpp"

#include "y/string/convert.hpp"

using namespace upsylon;

static inline
void do_bwt(const string &input)
{
    const size_t n = input.size();
    string output(n,as_capacity,true);
    //for(size_t i=n;i>0;--i) output << '\0';
    assert(output.size()==input.size());
    vector<size_t> indices(n,0);
    const size_t   pidx = information::bwt::encode(*output,*input,n,*indices);
    std::cerr << "input   = [" << input  << "]" << std::endl;
    std::cerr << "output  = [" << output << "]@" << pidx << std::endl;

    {
        information::delta_encoder enc;
        string delta_out = output;
        enc.apply(delta_out);
        const string s = string_convert::to_printable(delta_out);
        std::cerr << " |_delta: [" << s << "]" << std::endl;
    }

    {
        information::mtf_encoder enc;
        string delta_out = output;
        enc.apply(delta_out);
        const string s = string_convert::to_printable(delta_out);
        std::cerr << " |_mtf  : [" << s << "]" << std::endl;
    }


    string recover(n,as_capacity,true);
    //for(size_t i=n;i>0;--i) recover << '\0';
    assert(recover.size()==input.size());
    information::bwt::decode(*recover, *output, n, *indices, pidx);
    std::cerr << "recover = [" << recover << "]" << std::endl;

}

Y_UTEST(bwt)
{
    for(int iarg=1;iarg<argc;++iarg)
    {
        const string input = argv[iarg];
        do_bwt(input);
    }
}
Y_UTEST_DONE()

