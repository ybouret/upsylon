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
            void  newLine() throw();         //!< if user recognizes a new line
            static Module        *OpenSTDIN(); //!< stdin
            static Module        *OpenFile(const string &filename ); //!< regular file
            //! regular file
            static inline Module *OpenFile(const char   *filename) { const string _(filename); return OpenFile(_); }

        private:
            Input          input;
            Char::Pool    &cache;
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            explicit Module( const Origin &org, const Input &inp );


        };

    }
}

#endif
