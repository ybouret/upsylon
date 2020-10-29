

//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_BSTRING_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_BSTRING_INCLUDED 1

#include "y/jive/lexical/plugin/string_.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {
            //__________________________________________________________________
            //
            //
            //! Bracket String, betweeen brackets
            //
            //__________________________________________________________________
            class bString : public String_
            {
            public:
                virtual ~bString() throw(); //!< cleanup

                //! setup
                template <typename ID> inline
                explicit bString(const ID &id, Queue &q) :
                String_(id,bra,ket,q)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(bString);
            };

        }

    }
}

#endif

