
//! \file
#ifndef Y_JARGON_SOURCE_INCLUDED
#define Y_JARGON_SOURCE_INCLUDED 1


#include "y/jargon/module.hpp"
#include "y/sequence/pipe.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Source : public ios::istream
        {
        public:
            explicit Source(Module *module) throw();
            virtual ~Source() throw();
            
            void load(Module *module);
            
            Char *get();
            void  unget(Char *)        throw();
            void  unget(Char::List &l) throw();
            void  uncpy(const Char::List &l);
            void  skip(const size_t n) throw();
            
            virtual bool query(char &C);
            virtual void store(char  C);
            
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

