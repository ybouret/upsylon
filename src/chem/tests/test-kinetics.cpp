#include "y/chem/kinetics.hpp"
#include "y/chem/lua/io.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/utest/run.hpp"
using namespace upsylon;
using namespace Chemical;


namespace {
    using namespace math;

    class Schema
    {
    public:
        vector<double> C;

        inline Schema(const size_t M) : C(M)
        {

        }

        inline void injection( array<double> &dCdt, double t, const array<double> & )
        {

            tao::_ld(dCdt,0);
            if(t>=0&&t<=60)
            {
                tao::mulset(dCdt,0.01,C);
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Schema);

    };
}

Y_UTEST(kinetics)
{
    Lua::VM vm = new Lua::State();
    if(argc>1)
    {
        vm->doFile(argv[1]);
    }
    Integrator intg;

    Library lib;
    __luaIO::add(lib,vm,"species");

    Equilibria eqs;
    __luaIO::add(eqs,vm,"eqs",lib);
    
    eqs.compile_for(lib);
    const size_t M = eqs.M;

    vector<double> C0(M);
    Schema         schema(M);

    eqs.computeK(0);

    if(!__luaIO::boot(C0,vm,"ini",eqs,lib))
    {
        throw exception("cannot boot ini solution");
    }
    std::cerr << "C0:" << std::endl;
    lib.display(std::cerr,C0);

    if(!__luaIO::boot(schema.C,vm,"inject",eqs,lib))
    {
        throw exception("cannot boot injected solution");
    }
    std::cerr << "C1:" << std::endl;
    lib.display(std::cerr,schema.C);

    Kinetics kin( &schema, & Schema::injection );
    intg.driver.start(M);

    const double dt     = 1;
    const double dt_max = 0.01;
    for(double t=0;t<=90;t+=dt)
    {
        (std::cerr << '.').flush();
        intg.solve(C0,t,t+dt,dt_max,kin,eqs);
    }
    std::cerr << std::endl;

}
Y_UTEST_DONE()


