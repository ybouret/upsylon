//! \file
#ifndef Y_LANG_MODULE_INCLUDED
#define Y_LANG_MODULE_INCLUDED 1

#include "y/lang/char.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! will convert input char into Char
        class Module : public CharInfo, public counted
        {
        public:
            typedef arc_ptr<Module> Pointer; //!< pointer for Source
            virtual ~Module() throw();       //!< destructor
            Char *get();                     //!< convert char into Char
            void  newLine() const throw();   //!< if user recognizes a new line
            
            static Module        *OpenSTDIN(); //!< stdin
            static Module        *OpenFile(const string &filename ); //!< regular file
            //! regular file
            inline
            static Module *OpenFile(const char   *filename) { const string _(filename); return OpenFile(_); }
            //! data
            static Module *OpenData( const string &name, const char *data, const size_t size);
            //! data
            inline
            static Module *OpenData( const char *name, const char *data, const size_t size)
            {
                const string _(name); return OpenData(_,data,size);
            }
            //! data
            inline
            static Module *OpenData(const string &name, const memory::ro_buffer &buff)
            {
                return OpenData(name, (char *)buff.ro(), buff.length() );
            }
            //! data
            inline
            static Module *OpenData(const char *name, const memory::ro_buffer &buff)
            {
                return OpenData(name, (char *)buff.ro(), buff.length() );
            }



        private:
            Input          input;
            Char::Pool    &cache;
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            explicit Module( const Origin &org, const Input &inp );


        };

    }
}

#endif
