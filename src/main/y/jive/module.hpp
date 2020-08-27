//! \file

#ifndef Y_JIVE_MODULE_INCLUDED
#define Y_JIVE_MODULE_INCLUDED 1

#include "y/jive/input.hpp"
#include "y/jive/char.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! convert an input of chars into an input of Chars with Contexts
        //
        //______________________________________________________________________
        class Module : public CountedObject, public Context
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<Module> Handle; //!< alias

            //! keep track of creation
            enum Type
            {
                FromFile, //!< OpenFile
                FromData  //!< OpenData
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Module() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            Char *getChar();         //!< try to get next char, increase column
            void  newLine() throw(); //!< increase line, reset column


            //__________________________________________________________________
            //
            // creator
            //__________________________________________________________________

            //! open a file
            template <typename FILENAME> static inline
            Module * OpenFile(const FILENAME &filename)
            {
                const Tag   tag = new string(filename);
                const Input inp = MakeInput::FromFile(*tag);
                return new Module(tag,inp,FromFile);
            }

            //! open some data
            template <typename DATANAME> static inline
            Module * OpenData(const DATANAME &dataname, const char *buffer, const size_t buflen)
            {
                const Tag   tag = new string(dataname);
                const Input inp = MakeInput::FromData(buffer,buflen);
                return new Module(tag,inp,FromData);
            }

            //! open some text
            template <typename DATANAME> static inline
            Module * OpenData(const DATANAME &dataname, const char *buffer )
            {
                return OpenData(dataname,buffer,length_of(buffer));
            }

            //! open a buffer
            template <typename DATANAME> static inline
            Module * OpenData(const DATANAME &dataname, const memory::ro_buffer &buffer )
            {
                return OpenData(dataname,(const char *)buffer.ro(),buffer.length());
            }


            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            Input input;
            explicit Module(const Tag   &tag,
                            const Input &inp,
                            const Type   tid) throw();

        public:
            const Type type; //!< open mode
        };

    }

}

#endif
