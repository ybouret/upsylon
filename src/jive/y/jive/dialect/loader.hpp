
//! \file

#ifndef Y_JIVE_DIALECT_LOADER_INCLUDED
#define Y_JIVE_DIALECT_LOADER_INCLUDED 1

#include "y/jive/dialect/parser.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {

            //__________________________________________________________________
            //
            //
            //! Parse a file and replace include directives
            //
            //__________________________________________________________________
            class Loader : public Dialect::Parser
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Loader();
                virtual ~Loader() throw();

                //______________________________________________________________
                //
                //! load file and recursively load include directives
                //______________________________________________________________
                template <typename FILENAME>
                XNode *loadSketch( const FILENAME &fileName )
                {

                    XNode::Pointer dialect( parseFile(fileName) );
                    checkIncludes(dialect);
                    return dialect.yield();
                }

                //______________________________________________________________
                //
                //! load precompiled grammar
                //______________________________________________________________
                XNode *loadBinary(Module *);
                



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Loader);
                const hashing::minimal_perfect shash;
                void checkIncludes(XNode::Pointer &dialect);
                
            };

        }

    }

}

#endif

