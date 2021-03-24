//! \file

#ifndef Y_SOAK_INCLUDED
#define Y_SOAK_INCLUDED

#include "y/rtld/dll.h"
#include "y/exception.hpp"
#include <cstdio>

namespace upsylon
{
 
    
#define Y_SOAK_TRY(FUNC)                     \
/**/  do {                                   \
/**/    const char * const __soak_fn = FUNC; \
/**/    try
    
#define Y_SOAK_CATCH()                                                       \
/**/    catch(const upsylon::exception &e)                                   \
/**/    {                                                                    \
/**/      soak::print(stderr,"[%s] %s: %s\n",__soak_fn,e.what(),e.when());   \
/**/    }                                                                    \
/**/    catch(...)                                                           \
/**/    {                                                                    \
/**/      soak::print(stderr,"[%s] %s",__soak_fn,soak::unhandled_exception); \
/**/    }                                                                    \
/**/  } while(false)
    
#define Y_SOAK_VERBOSE(CODE) do { if(soak::verbose) { CODE; } } while(false)
    
    struct soak
    {
        static void       print(FILE * stream, const char * format, ...) throw();
        static const char unhandled_exception[];
        static const char multi_template_init[];
        static bool       verbose;
        
        template <typename APPLICATION>
        class app
        {
        public:
            inline virtual ~app() throw() {}
            
            static inline const char *soname() throw() { return APPLICATION::call_sign; }
            
          
            
            
        protected:
            inline explicit app() throw() {}
            
            inline static void quit() throw()
            {
                Y_SOAK_VERBOSE(soak::print(stderr,"[%s] quit\n",soname()));
                if(instance)
                {
                    delete instance;
                    instance = 0;
                }
            }
            
            inline static
            APPLICATION *init() throw()
            {
                Y_SOAK_VERBOSE(soak::print(stderr,"[%s] init\n",soname()));
                Y_SOAK_TRY(soname())
                {
                    return (APPLICATION *)(instance ? instance : (instance = (volatile APPLICATION *) new APPLICATION() ));
                }
                Y_SOAK_CATCH();
                return 0;
            }
            
            template <typename T>
            static APPLICATION *init(const T &args)
            {
                Y_SOAK_VERBOSE(soak::print(stderr,"[%s] init<...>\n",soname()));
                Y_SOAK_TRY(soname())
                {
                    if(instance) throw exception("%s",multi_template_init);
                    instance = (volatile APPLICATION *) new APPLICATION(args);
                    return (APPLICATION *)instance;
                }
                Y_SOAK_CATCH();
                return 0;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(app);
            static volatile APPLICATION *instance;
        };

        
    };
    
    template <typename T>  volatile T *  soak::app<T>::instance = 0;

    
}

#endif

