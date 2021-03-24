
#include "y/rtld/dll.h"
#include "y/concurrent/loop/simt.hpp"
#include "y/exception.hpp"
#include <cmath>
#include <cstdio>
#include <cstdarg>

using namespace upsylon;


namespace  {

    static inline void Print(FILE * stream, const char * format, ...)
    {
        if(stream)
        {
            va_list args;
            va_start (args, format);
            vfprintf (stream, format, args);
            va_end (args);
        }
    }


    class App : public concurrent::simt
    {
    public:
        static App *Impl;

        explicit App() : concurrent::simt()
        {

        }

        explicit App(const unsigned np) : concurrent::simt(0,np,1)
        {

        }

        virtual ~App() throw()
        {
        }

        static void Quit() throw()
        {
            Print(stderr,"[engine] quit\n");
            if(Impl)
            {
                Print(stderr,"[engine] delete...\n");
                delete Impl;
                Impl=0;
            }
        }

        static App *Init(unsigned np=0)
        {
            try
            {
                if(!Impl)
                {
                    if(np<=0)
                    {
                        Impl = new App(); // default
                    }
                    else
                    {
                        Impl = new App(np);
                    }
                    Print(stderr,"[engine] created with <%u>\n", unsigned(Impl->size()));

                }
                else
                {
                    Print(stderr,"[engine] already with <%u>\n", unsigned(Impl->size()));
                }
            }
            catch(const exception &e)
            {
                Print(stderr,"%s:%s\n",e.what(),e.when());

            }
            catch(...)
            {
                Print(stderr,"Unhandled Exception!\n");
            }
            return Impl;
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(App);
    };

    App * App::Impl = 0;
    
}


static inline void enter() throw()
{
    Print(stderr,"Enter Engine DLL\n");
}



static inline void leave() throw()
{
    Print(stderr, "Leave Engine DLL\n");
}


Y_DLL_SETUP(enter,leave);

Y_DLL_EXTERN()

Y_EXPORT int Y_DLL_API EngineInit(unsigned np) throw()
{
    Print(stderr,"EngineStart(%u)\n",np);
    return (App::Init(np) != 0) ?  0 : -1;
}

Y_EXPORT void Y_DLL_API EngineQuit() throw()
{
    App::Quit();
}



Y_DLL_FINISH()
