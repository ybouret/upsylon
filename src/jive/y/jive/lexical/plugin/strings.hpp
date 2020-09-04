
//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_STRINGS_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_STRINGS_INCLUDED 1

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
                const char delimiter; //!< the delimiter

                virtual ~String_() throw(); //!< cleanup

                //! setup
                template <typename ID>
                explicit String_(const ID &id, const char C, Queue &q) :
                Plugin(id,C,q,RejectEOS),
                delimiter(C),
                s()
                {
                    setup();
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(String_);
                Token s;
                void  setup();
                virtual void OnInit(const  Token &);
                void         OnQuit(const  Token &);
                void         OnCore(const  Token &);

                void         OnDelim(const Token &);
                void         OnHexa(const  Token &);
                void         OnEsc(const   Token &);
                
                void         OnError(const Token &);
            };

            class jString : public String_
            {
            public:
                virtual ~jString() throw();

                template <typename ID>
                explicit jString(const ID &id, Queue &q) :
                String_(id, '"' , q)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(jString);
            };


            class rString : public String_
            {
            public:
                virtual ~rString() throw();

                template <typename ID>
                explicit rString(const ID &id, Queue &q) :
                String_(id, '\'' , q)
                {
                }
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(rString);
            };


        }

    }

}

#endif

