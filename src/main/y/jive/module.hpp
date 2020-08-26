

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


            Char *getChar();         //!< try to get next char, increase column
            void  newLine() throw(); //!< increase line, reset column

            
            template <typename FILENAME> static inline
            Module * OpenFile(const FILENAME &filename)
            {
                const Tag   tag = new string(filename);
                const Input inp = MakeInput::FromFile(*tag);
                return new Module(tag,inp,FromFile);
            }
            
            template <typename DATANAME> static inline
            Module * OpenData(const DATANAME &dataname, const char *buffer, const size_t buflen)
            {
                const Tag   tag = new string(dataname);
                const Input inp = MakeInput::FromData(buffer,buflen);
                return new Module(tag,inp,FromData);
            }
            
            template <typename DATANAME> static inline
            Module * OpenData(const DATANAME &dataname, const char *buffer )
            {
                return OpenData(dataname,buffer,length_of(buffer));
            }
            
            template <typename DATANAME> static inline
            Module * OpenData(const DATANAME &dataname, const memory::ro_buffer &buffer )
            {
                return OpenData(dataname,(const char *)buffer.ro(),buffer.length());
            }
            
            template <typename DATANAME> static inline
            Module * OpenData(const DATANAME &dataname)
            {
                const Tag   tag = new string(dataname);
                const Input inp = MakeInput::FromData(**tag,tag->size());
                return new Module(tag,inp,FromData);
            }
            
            
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
