

//! \file

#ifndef Y_JIVE_MODULE_INCLUDED
#define Y_JIVE_MODULE_INCLUDED 1

#include "y/jive/input.hpp"
#include "y/jive/char.hpp"

namespace upsylon {

    namespace Jive
    {

        //! convert an input of char into an input of Char with Contexts
        class Module : public Object, public counted, public Context
        {
        public:
            typedef arc_ptr<Module> Handle;
            
            enum Type
            {
                FromFile,
                FromData
            };

            virtual ~Module() throw();

            template <typename FILENAME> static inline
            Module * OpenFile(const FILENAME &filename)
            {
                const Tag   tag = new string(filename);
                const Input inp = MakeInput::FromFile(*tag);
                return new Module(tag,inp,FromFile);
            }

            Char *getChar();         //!< try to get next char, increase column
            void  newLine() throw(); //!< increase line, reset column


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            Input input;
            explicit Module(const Tag   &tag,
                            const Input &inp,
                            const Type   tid) throw();

        public:
            const Type type;
        };

    }

}

#endif
