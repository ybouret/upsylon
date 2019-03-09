//! \file
#ifndef Y_LANG_MODULE_INCLUDED
#define Y_LANG_MODULE_INCLUDED 1

#include "y/lang/char.hpp"

namespace upsylon
{
    namespace Lang
    {



        //! convert an istream of 'char' into a strean of tagged 'Char'
        class Module : public CharInfo, public counted
        {
        public:
            //! opening mode
            enum Type
            {
                FromSTDIN, //!< stdin was open
                FromFile,  //!< a file was open
                FromData   //!< some data where provided
            };
            typedef arc_ptr<Module> Pointer; //!< pointer for Source

            virtual ~Module() throw();       //!< destructor
            Char *get();                     //!< convert char into Char
            void  newLine() const throw();   //!< if user recognizes a new line

            //__________________________________________________________________
            //
            // opening files
            //__________________________________________________________________
            static Module *OpenSTDIN();                       //!< open stdin
            static Module *OpenFile(const string &filename ); //!< open a regular file
            static Module *OpenFile(const char   *filename);  //!< open a regular file

            //__________________________________________________________________
            //
            // opening data streams
            //__________________________________________________________________
            static Module *OpenData( const string &name, const void *data, const size_t size);  //!< open a data stream, create a tag 'name'
            static Module *OpenData( const char   *name, const void *data, const size_t size);  //!< open a data stream, create a tag 'name'
            static Module *OpenData( const string &name, const memory::ro_buffer &buff);        //!< open a data stream, create a tag 'name'
            static Module *OpenData(const char *name, const memory::ro_buffer &buff);           //!< open a data stream, create a tag 'name'

        private:
            Input          input;
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            explicit Module( const Tag &org, const Input &inp, const Type t);

        public:
            const Type type; //!< keep track of the opening mode

        };

    }
}

#endif
