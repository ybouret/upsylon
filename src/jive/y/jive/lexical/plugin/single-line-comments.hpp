//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_SINGLE_LINE_COMMENTS_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_SINGLE_LINE_COMMENTS_INCLUDED 1

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
            //! single line comments
            //
            //__________________________________________________________________
            class SingleLineComments : public Plugin
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~SingleLineComments() throw();

                template <typename ID, typename RX>
                explicit SingleLineComments(const ID &id, const RX &rx, Lexical::Queue &q) :
                Plugin(id,rx,q)
                {
                    setup();
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SingleLineComments);
                virtual void OnInit(const Token &);
                void         setup();
            };

#define Y_JIVE_LEXICAL_SINGLE_LINE_COMMENTS upsylon::Jive::Lexical::SingleLineComments

#define Y_JIVE_LEXICAL_SL_COMMENTS(NAME,REGEXP) \
class NAME : public Y_JIVE_LEXICAL_SINGLE_LINE_COMMENTS {\
/**/public:\
/**/    template <typename ID> inline\
/**/    explicit NAME(const ID &id, Lexical::Queue &q):\
/**/    Y_JIVE_LEXICAL_SINGLE_LINE_COMMENTS(id,REGEXP,q) {}\
/**/    inline virtual ~NAME() throw() {}\
/**/private:\
/**/    Y_DISABLE_COPY_AND_ASSIGN(NAME);\
}

        }

        Y_JIVE_LEXICAL_SL_COMMENTS(ShellComments,'#');
        Y_JIVE_LEXICAL_SL_COMMENTS(CppComments,"//");
    }
}

#endif
