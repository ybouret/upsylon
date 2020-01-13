#include "y/utest/run.hpp"
#include "y/lua++/function.hpp"

using namespace upsylon;

Y_UTEST(vm)
{
    Lua::VM vm = new Lua::State();

    vm->doString("print(math.cos(0.1))");
    vm->doString("print(math.cos)");
    vm->doString("function f(x)   return math.cos(x) end");
    vm->doString("function g(x,y) return math.cos(x)+math.sin(y) end");

    //return 0;

    Lua::Function<double> f("f",vm);
    for(double x=0;x<=3;x+=0.2)
    {
        std::cerr << "F(" << x << ")=" << f(x) << std::endl;
    }

    Lua::Function<float> g("g",vm);
    std::cerr << "g(0.1,0.1)=" << g(0.1f,0.1f) << std::endl;

    if(argc>1)
    {
        const string cmd = vformat("print( y_file_exists(\"%s\") )", argv[1]);
        vm->doString(cmd);
    }
}
Y_UTEST_DONE()

