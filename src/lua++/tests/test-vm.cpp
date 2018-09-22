#include "y/utest/run.hpp"
#include "y/lua++/function.hpp"

using namespace upsylon;

Y_UTEST(vm)
{
    Lua::VM vm = new Lua::State();

    vm->doString("print(math.cos(0.1))");
    vm->doString("print(math.cos)");
    vm->doString("function f(x) return math.cos(x) end");
    //return 0;

    Lua::Function<double> f("f",vm);
    for(double x=0;x<=3;x+=0.2)
    {
        std::cerr << "F(" << x << ")=" << f(x) << std::endl;
    }
}
Y_UTEST_DONE()

