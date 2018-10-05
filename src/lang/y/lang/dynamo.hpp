#ifndef Y_LANG_DYNAMO_INCLUDED
#define Y_LANG_DYNAMO_INCLUDED 1

#include "y/lang/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! generate a compiler for the given grammar/compiled grammar
        class Dynamo : public Compiler
        {
        public:
            //! input format
            enum FormatType
            {
                SourceFile, //!< a valid grammar
                BinaryFile  //!< a compiled grammar
            };

            //! load from source grammar
            static Syntax::Parser *FromSource(const string &filename, const bool verbose=false);
            //! load from compiled grammar
            static Syntax::Parser *FromBinary(const string &filename, const bool verbose=false);
            //! load from compiled grammar in memory chunk
            static Syntax::Parser *FromBinary(const char   *name, const char *data, const size_t size, const bool verbose=false);
            //! dispatch calls
            static Syntax::Parser *Load( const string &filename, const FormatType type, const bool verbose=false);
            //! compile a grammar into its binary form
            static string          ToBinary(const string &filename);

            //! destructor
            virtual ~Dynamo() throw();

            //! Create a compiler from source/binary file
            inline explicit Dynamo(const string    &filename,
                                   const FormatType type,
                                   const bool       verbose=false) :
            Compiler( Load(filename,type,verbose) )
            {
            }
            
            //! create a compiler from a binary data chunk
            inline explicit Dynamo(const char  *name,
                                   const char  *data,
                                   const size_t size,
                                   const bool   verbose=false) :
            Compiler( FromBinary(name,data,size,verbose ) )
            {

            }

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dynamo);
        };
    }
}

#endif

