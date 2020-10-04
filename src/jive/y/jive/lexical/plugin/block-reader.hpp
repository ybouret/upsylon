//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_BLOCK_READER_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_BLOCK_READER_INCLUDED 1

#include "y/jive/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //! Read block between Enter/Leave
            /**
             verbatim, but replace all [:endl:] by '\n'
             */
            //
            //__________________________________________________________________
            class BlockReader : public Plugin
            {
            public:
                //! cleanup
                virtual ~BlockReader() throw();
                
                //! setup with enter/leave regexp
                template <typename ID, typename ENTER, typename LEAVE>
                explicit BlockReader(const ID       &id,
                                     const ENTER    &enter,
                                     const LEAVE    &leave,
                                     Lexical::Queue &q) :
                Plugin(id,enter,q,RejectEOS),
                block(NULL)
                {
                    back(leave,this, &BlockReader::OnQuit);
                    setup();
                }
                
            private:
                Unit::Pointer block;
                
                Y_DISABLE_COPY_AND_ASSIGN(BlockReader);
                void         setup();
                virtual void OnInit(const Token &);
                void         OnQuit(const Token &);
                
                void         OnEndl(const Token &);
                void         OnChar(const Token &);
                
            };
        }
    }
}

#endif

