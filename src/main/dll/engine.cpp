
#include "y/rtld/soak.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/exception.hpp"
#include <cmath>
#include <cstdio>
 
using namespace upsylon;


namespace  {

    
    class App : public soak::app<App>, public concurrent::simt
    {
    public:
        static const char call_sign[];
        
        static inline void Quit() throw()
        {
            soak::app<App>::quit();
        }
        
        static inline App *Init(const unsigned np)
        {
            return (np<=0) ? soak::app<App>::init() : soak::app<App>::init<unsigned>(np);
        }
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(App);
        inline explicit App(const unsigned np) : concurrent::simt(0,np,1) {}
        inline explicit App( ) : concurrent::simt( ) {}

        inline virtual ~App() throw() {}
        friend class soak::app<App>;
    };
    
    const char App::call_sign[] = "App";
    


    
}


static inline void enter() throw()
{
    soak::print(stderr,"Enter Engine DLL\n");
    soak::verbose = true;
}



static inline void leave() throw()
{
    soak::print(stderr, "Leave Engine DLL\n");
}


Y_DLL_SETUP(enter,leave);

Y_DLL_EXTERN()

Y_EXPORT int Y_DLL_API EngineInit(unsigned np) throw()
{
    soak::print(stderr,"EngineStart(%u)\n",np);
    return App::Init(np) != 0 ? 0 : -1;
}

Y_EXPORT void Y_DLL_API EngineQuit() throw()
{
    App::Quit();
}



Y_DLL_FINISH()
