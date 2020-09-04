
//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_MULTI_LINES_COMMENTS_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_MULTI_LINES_COMMENTS_INCLUDED 1

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
            //! multiple lines comments
            //
            //__________________________________________________________________
            class MultiLinesComments : public Plugin
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~MultiLinesComments() throw();


                template <typename ID, typename ENTER, typename LEAVE>
                explicit MultiLinesComments(const ID       &id,
                                            const ENTER    &enter,
                                            const LEAVE    &leave,
                                            Lexical::Queue &q) :
                Plugin(id,enter,q)
                {
                    back(leave,this,&Scanner::nothing);
                    setup();
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(MultiLinesComments);
                virtual void OnInit(const Token &);
                void         setup();

            };

            //! alias
#define Y_JIVE_LEXICAL_MULTI_LINES_COMMENTS upsylon::Jive::Lexical::MultiLinesComments

            //! create a single line comments class
#define Y_JIVE_LEXICAL_ML_COMMENTS(NAME,ENTER,LEAVE) \
class NAME : public Y_JIVE_LEXICAL_MULTI_LINES_COMMENTS {\
/**/public:\
/**/    template <typename ID> inline\
/**/    explicit NAME(const ID &id, Lexical::Queue &q):\
/**/    Y_JIVE_LEXICAL_MULTI_LINES_COMMENTS(id,ENTER,LEAVE,q) {}\
/**/    inline virtual ~NAME() throw() {}\
/**/private:\
/**/    Y_DISABLE_COPY_AND_ASSIGN(NAME);\
}

            Y_JIVE_LEXICAL_ML_COMMENTS(C_Comments,"/\\*","\\*/");
        }
    }
}

#endif
