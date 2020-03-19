
#include "y/jargon/pattern/basic/any1.hpp"
#include "y/jargon/pattern/basic/single.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(jargon_pattern)
{
    auto_ptr<Pattern> p = Any1::Create();
    p = Single::Create('a');
}
Y_UTEST_DONE()

