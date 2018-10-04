#ifndef Y_LANG_DYNAMO_INCLUDED
#define Y_LANG_DYNAMO_INCLUDED 1

#include "y/lang/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        class Dynamo : public Compiler
        {
        public:
            static
            Syntax::Parser *FromFile(const string &filename,
                                     const bool    verbose=false);

            static inline
            Syntax::Parser *FromFile(const char  *filename,
                                     const bool   verbose=false)

            {
                const string _(filename); return FromFile(_,verbose);
            }

            static Syntax::Parser *FromData( const string &filename, const bool verbose=false);

            static inline
            Syntax::Parser *FromData(const char  *filename,
                                     const bool   verbose=false)

            {
                const string _(filename); return FromData(_,verbose);
            }
            


            virtual ~Dynamo() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dynamo);
        };
    }
}

#endif

