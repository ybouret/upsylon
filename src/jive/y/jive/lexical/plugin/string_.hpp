
//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_STRING__INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_STRING__INCLUDED 1

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
            //! finding string between delimiters
            //
            //__________________________________________________________________
            class String_ : public Plugin
            {
            public:
                static const char quote;  //!< single quote
                static const char dquote; //!< double quote
                static const char bra;    //!< '<'
                static const char ket;    //!< '>'

                const char delimiter; //!< the (closing) delimiter

                virtual ~String_() throw(); //!< cleanup

            protected:
                //! setup
                template <typename ID>
                explicit String_(const ID  &id,
                                 const char delimOpen,
                                 const char delimClose,
                                 Queue     &q) :
                Plugin(id,delimOpen,q,RejectEOS),
                delimiter(delimClose),
                unit(NULL)
                {
                    setup();
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(String_);
                Unit::Pointer unit;

                void  setup();
                virtual void OnInit(const  Token &);
                void         OnQuit(const  Token &);
                void         OnCore(const  Token &);

                void         OnDelim(const Token &);
                void         OnHexa(const  Token &);
                void         OnHerr(const  Token &);
                void         OnEsc(const   Token &);
                
                void         OnError(const Token &);
            };

        }

    }

}

#endif

