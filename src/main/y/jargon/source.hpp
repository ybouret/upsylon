//! \file

#ifndef Y_JARGON_SOURCE_INCLUDED
#define Y_JARGON_SOURCE_INCLUDED 1


#include "y/jargon/module.hpp"
#include "y/sequence/pipe.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! a source is a smart pointer and cache for modules
        class Source : public ios::istream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit     Source(const Char::Cache &,Module *) throw(); //!< setup with first module
            virtual     ~Source() throw();                       //!< cleanup
       
            //__________________________________________________________________
            //
            // modules management
            //__________________________________________________________________
            void         load(Module *module);           //!< load a new module and push current into history
            size_t       depth() const throw();          //!< history size
            void         newLine() throw();              //!< propagate newLine to current module
            
            //__________________________________________________________________
            //
            // Char management
            //__________________________________________________________________
            Char        *get();                          //!< NULL => End Of Module
            void         unget(Char *)        throw();   //!< unget a Char
            void         unget(Char::List &l) throw();   //!< unget a Token
            void         uncpy(const Char::List &l);     //!< unget a copy of a Token
          
            
            //__________________________________________________________________
            //
            // I/O  management
            //__________________________________________________________________
            void         skip(const size_t n) throw();   //!< skip iobuf
            size_t       prefetch(size_t n);             //!< prefetch at most n Char
            bool         alive();                        //!< check if get() shall return a valid Char
            size_t       bufferSize() const throw();     //!< number of chars in buffer
            Char::Cache &cache()      const throw();     //!< shared cache access

            //__________________________________________________________________
            //
            // istream behavior, will drop contexts
            //__________________________________________________________________
            virtual bool query(char &C);                 //!< ios::istream interface
            virtual void store(char  C);                 //!< ios::istream interface
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Source);
            typedef lstack<const Module::Handle> History;
            Module::Handle       handle;
            Token                iobuf;
            History              history;
            Char *tryGet();
            
        };
        
    }
    
}

#endif

