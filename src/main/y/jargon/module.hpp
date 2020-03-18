//! \file
#ifndef Y_JARGON_MODULE_INCLUDED
#define Y_JARGON_MODULE_INCLUDED 1


#include "y/jargon/token.hpp"
#include "y/jargon/input.hpp"

namespace upsylon {
 
    namespace Jargon {
        
        //! convert a source of char into a source of Char
        /**
         
         */
        class Module : public CountedObject, public Context
        {
        public:
            typedef arc_ptr<Module> Handle;

            enum OpenType
            {
                OpenWithFile,
                OpenWithData
            };
            
            virtual ~Module() throw();
            
            void newLine() throw();
            
            static Module * OpenFile(const Char::Cache &, const string &fileName);
            static Module * OpenFile(const Char::Cache &, const char   *fileName);
            static Module * OpenData(const Char::Cache &, const string &dataName,const void *data,const size_t size);
            static Module * OpenData(const Char::Cache &, const char   *dataName,const void *data,const size_t size);
            static Module * OpenData(const Char::Cache &, const char   *data, const size_t size);
            static Module * OpenData(const Char::Cache &, const char   *data);
            static Module * OpenData(const Char::Cache &, const string &data);

            Char *read();
            void  unread(Char *) throw();
            void  unread(Char::List_ &) throw();
            void  uncopy(const Char::List_ &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            explicit Module(const Stamp       &,
                            const Input       &,
                            const Char::Cache &,
                            const OpenType      ) throw();
            
            Input          input;
            Token          iobuf;
        public:
            const OpenType type;
            
        };
        
        
    }
    
}

#endif

