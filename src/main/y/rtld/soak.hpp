//! \file

#ifndef Y_SOAK_INCLUDED
#define Y_SOAK_INCLUDED

#include "y/rtld/dll.h"
#include "y/exception.hpp"
#include <cstdio>

namespace upsylon
{
    //__________________________________________________________________________
    //
    //! try block...
    //______________________________________________________________________________
#define Y_SOAK_TRY(FUNC)                     \
/**/  do {                                   \
/**/    const char * const __soak_fn = FUNC; \
/**/    try
    
    //__________________________________________________________________________
    //
    //! ...to catch'm all
    //___________________________________________________________________________
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
    
    //__________________________________________________________________________
    //
    //! code on verbosity
    //__________________________________________________________________________
#define Y_SOAK_VERBOSE(CODE) do { if(soak::verbose) { CODE; } } while(false)
    
    //__________________________________________________________________________
    //
    //! success value
    //__________________________________________________________________________
#define Y_SOAK_SUCCESS ( 0)
    
    //__________________________________________________________________________
    //
    //! failure value
    //__________________________________________________________________________
#define Y_SOAK_FAILURE (-1)
    
    //__________________________________________________________________________
    //
    //
    //! Shared Object Application Kit
    //
    //__________________________________________________________________________
    struct soak
    {
        //______________________________________________________________________
        //
        // definition
        //______________________________________________________________________
        
        //! internal safe printf
        static void        print(FILE * stream, const char * format, ...) throw();
        static const char  unhandled_exception[]; //!< "unhandled exception"
        static bool        verbose;               //!< verbose flag
        static inline void nope() throw() {}      //!< nope for leave/enter in dll

        //______________________________________________________________________
        //
        //! class to handle one instance of an APPLICATION
        /**
         the APPLICATION must have a call_sign
         */
        //______________________________________________________________________
        template <typename APPLICATION>
        class app
        {
        public:
            //! cleanup
            inline virtual           ~app() throw() {}
            
            //! soname from call_sign
            static inline const char *soname() throw() { return APPLICATION::call_sign; }
            
            //! get initialized instance
            static inline APPLICATION &_() throw() {
                assert(instance);
                return *(APPLICATION *)instance;
            }
            
        protected:
            //! setup
            inline explicit app() throw() {}
            
            //! cleanup
            inline static void quit() throw()
            {
                Y_SOAK_VERBOSE(soak::print(stderr,"[%s] quit\n",soname()));
                if(instance)
                {
                    delete instance;
                    instance = 0;
                }
            }
            
            //! create/recall
            inline static
            APPLICATION *init() throw()
            {
                Y_SOAK_TRY(soname())
                {
                    if(instance)
                    {
                        Y_SOAK_VERBOSE(soak::print(stderr,"[%s] call\n",soname()));
                    }
                    else
                    {
                        Y_SOAK_VERBOSE(soak::print(stderr,"[%s] init\n",soname()));
                        instance = (volatile APPLICATION *) new APPLICATION();
                    }
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

    //__________________________________________________________________________
    //
    //! common code to initialize CLASS Init/Quit an call_sign
    //__________________________________________________________________________
#define Y_SOAK_INIT_(CLASS) \
/**/  {                                                                            \
/**/    public:                                                                    \
/**/      inline static void   Quit() throw() { soak::app<CLASS>::quit();        } \
/**/      inline static CLASS *Init() throw() { return soak::app<CLASS>::init(); } \
/**/      static const char call_sign[]
    
    //! no parameters macro
#define Y_SOAK_NO_PARAMS void
    
    //! no code macro
#define Y_SOAK_NO_LOADER (void)0
    
    //__________________________________________________________________________
    //
    //! finalize the code
    //__________________________________________________________________________
#define Y_SOAK_FINISH(CLASS,PARAMS,LOADER)                               \
/**/    private:                                                         \
/**/      Y_DISABLE_COPY_AND_ASSIGN(CLASS);                              \
/**/      inline virtual ~CLASS() throw() {}                             \
/**/      friend class soak::app<CLASS>;                                 \
/**/  };                                                                 \
/**/ Y_DLL_EXTERN()                                                      \
/**/ Y_EXPORT void Y_DLL_API CLASS##Quit() throw() { CLASS::Quit(); }    \
/**/ Y_EXPORT int  Y_DLL_API CLASS##Init(PARAMS) throw() {               \
/**/   do { LOADER; } while(false);                                      \
/**/   return (NULL != CLASS::Init()) ? Y_SOAK_SUCCESS : Y_SOAK_FAILURE; \
/**/ }                                                                   \
/**/ Y_DLL_FINISH()                                                      \
/**/ const char CLASS::call_sign[] = #CLASS
    
    //__________________________________________________________________________
    //
    //! direct finish class
    //__________________________________________________________________________
#define Y_SOAK_POLISH(CLASS) Y_SOAK_FINISH(CLASS,Y_SOAK_NO_PARAMS,Y_SOAK_NO_LOADER)

    //__________________________________________________________________________
    //
    //! common CLASS declaration
    //__________________________________________________________________________
#define Y_SOAK_DECL_(CLASS) class CLASS : public soak::app<CLASS>
    
    
    //__________________________________________________________________________
    //
    //! declare simple class
    //__________________________________________________________________________
#define Y_SOAK_DECLARE(CLASS)  \
/**/    Y_SOAK_DECL_(CLASS)    \
/**/    Y_SOAK_INIT_(CLASS)

    //__________________________________________________________________________
    //
    //! declare derived class
    //__________________________________________________________________________
#define Y_SOAK_DERIVED(CLASS,BASE)       \
/**/    Y_SOAK_DECL_(CLASS), public BASE \
/**/    Y_SOAK_INIT_(CLASS)
    
    //__________________________________________________________________________
    //
    //! declare derived class
    //__________________________________________________________________________
#define Y_SOAK_DERIVED2(CLASS,BASE1,BASE2)              \
/**/    Y_SOAK_DECL_(CLASS), public BASE1, public BASE2 \
/**/    Y_SOAK_INIT_(CLASS)
    

    
}

#endif
