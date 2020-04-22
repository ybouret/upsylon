
//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_BLOCK_READER_INCLUDED
#define Y_JARGON_LEXICAL_PLUGIN_BLOCK_READER_INCLUDED 1


#include "y/jargon/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {

            //! read and catenate all chars between enter/leave
            class BlockReader : public Plugin {
            public:
                Y_JARGON_PLUGIN_DECL(BlockReader); //!< aliases
                
                //! cleanup
                virtual ~BlockReader() throw();
                
                //! setup
                template <typename ID,
                typename ENTER,
                typename LEAVE> inline
                explicit BlockReader(Analyzer    &Lx,
                                     const ID    &id,
                                     const ENTER &enter,
                                     const LEAVE &leave) :
                Plugin(Lx,id,enter),
                unit(0)
                {
                    back(leave, this, &BlockReader::onEmit);
                    setup();
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(BlockReader);
                void setup();
                auto_ptr<Unit> unit;
                virtual void onInit(const Token &);
                void         onEmit(const Token &);
                void         onChar(const Token &);
            };
            
        }
        
    }
    
}

#endif

