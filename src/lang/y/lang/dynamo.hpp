#ifndef Y_LANG_DYNAMO_INCLUDED
#define Y_LANG_DYNAMO_INCLUDED 1

#include "y/lang/compiler.hpp"
#include "y/hashing/sha1.hpp"

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

            template <typename HASH_FUNCTION>
            class Hasher
            {
            public:
                inline explicit Hasher() throw() : H() {}
                inline virtual ~Hasher() throw()  {}
                inline int32_t  operator()(const string &s) throw()
                {
                    const uint32_t u = H.template key<uint32_t>(s);
                    const int32_t  h = int32_t(u&0x7fffffff); assert(h>=0);
                    return h;
                }

                HASH_FUNCTION H;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Hasher);
            };

            typedef Hasher<hashing::sha1> Hash31;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dynamo);
        };
    }
}

#endif

