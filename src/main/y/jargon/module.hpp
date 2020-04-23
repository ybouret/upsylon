//! \file
#ifndef Y_JARGON_MODULE_INCLUDED
#define Y_JARGON_MODULE_INCLUDED 1


#include "y/jargon/token.hpp"
#include "y/jargon/input.hpp"

namespace upsylon {
 
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! convert a source of char into a source of Char
        /**
         the role of the module is to set a proper context to read chars
         */
        //----------------------------------------------------------------------

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
            enum Type
            {
                FileStream, //!< dealing with a file
                DataStream  //!< dealing with data
            };
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            virtual ~Module()  throw(); //!< cleanup
            void     newLine() throw(); //!< update line count, reset column
            Char    *getChar();         //!< get next Char
            
            
            //------------------------------------------------------------------
            //
            // static methods
            //
            //------------------------------------------------------------------
            
            //__________________________________________________________________
            //
            // opening file stream
            //__________________________________________________________________
            static Module * OpenFile(const string &fileName); //!< open a file
            static Module * OpenFile(const char   *fileName); //!< open a file
          
            //__________________________________________________________________
            //
            // opening data stream
            //__________________________________________________________________
            static Module * OpenData(const string &dataName,const void *data,const size_t size); //!< open some data with dataName
            static Module * OpenData(const char   *dataName,const void *data,const size_t size); //!< open some data with dataName
            static Module * OpenData(const char   *data, const size_t size);                     //!< open some data with same name
            static Module * OpenData(const char   *data);                                        //!< open some data with same name
            static Module * OpenData(const string &data);                                        //!< open some data with same name

            template <typename DATANAME>
            static inline Module *Open( const DATANAME &dataName, const void *data, const size_t size)
            {
                return OpenData(dataName,data,size);
            }
            
            template <typename DATANAME>
            static inline Module *Open( const DATANAME &dataName, const char *text)
            {
                return OpenData(dataName,text,length_of(text));
            }
            
            template <typename DATANAME>
            static inline Module *Open( const DATANAME &dataName, const memory::ro_buffer &buff)
            {
                return OpenData(dataName,buff.ro(),buff.length());
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            explicit Module(const Tag   &,
                            const Input &,
                            const Type ) throw();
            Input          input;
        public:
            const Type type;  //!< what we are dealing with
            
        };
        
        
    }
    
}

#endif

