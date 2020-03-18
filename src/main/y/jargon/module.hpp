//! \file
#ifndef Y_JARGON_MODULE_INCLUDED
#define Y_JARGON_MODULE_INCLUDED 1


#include "y/jargon/token.hpp"
#include "y/jargon/input.hpp"

namespace upsylon {
 
    namespace Jargon {
        
        //! convert a source of char into a source of Char
        class Module : public CountedObject, public Context
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef arc_ptr<Module> Handle; //!< alias

            //! keep track of module type
            enum OpenType
            {
                OpenWithFile, //!< dealing with a file
                OpenWithData  //!< dealing with data
            };
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            virtual ~Module()  throw();            //!< cleanup
            void     newLine() throw();            //!< update line count, reset column
            Char    *read();                       //!< read next Char, NULL on End of Data
            void     unread(Char *) throw();       //!< unread a Char
            void     unread(Char::List &) throw(); //!< unread a Token
            void     uncopy(const Char::List &);   //!< uncopy a Token
            
            
            //------------------------------------------------------------------
            //
            // static methods
            //
            //------------------------------------------------------------------
            
            //__________________________________________________________________
            //
            // files
            //__________________________________________________________________
            static Module * OpenFile(const Char::Cache &, const string &fileName); //!< open a file
            static Module * OpenFile(const Char::Cache &, const char   *fileName); //!< open a file
          
            //__________________________________________________________________
            //
            // data
            //__________________________________________________________________
            static Module * OpenData(const Char::Cache &, const string &dataName,const void *data,const size_t size); //!< open some data with dataName
            static Module * OpenData(const Char::Cache &, const char   *dataName,const void *data,const size_t size); //!< open some data with dataName
            static Module * OpenData(const Char::Cache &, const char   *data, const size_t size);                     //!< open some data with same name
            static Module * OpenData(const Char::Cache &, const char   *data);                                        //!< open some data with same name
            static Module * OpenData(const Char::Cache &, const string &data);                                        //!< open some data with same name

           
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            explicit Module(const Stamp       &,
                            const Input       &,
                            const Char::Cache &,
                            const OpenType      ) throw();
            
            Input          input;
            Token          iobuf;
        public:
            const OpenType type; //!< what we are dealing with
            
        };
        
        
    }
    
}

#endif

