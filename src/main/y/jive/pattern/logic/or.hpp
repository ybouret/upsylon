//! \file

#ifndef Y_PATTERN_LOGIC_OR_INCLUDED
#define Y_PATTERN_LOGIC_OR_INCLUDED 1

#include "y/jive/pattern/logic/ooo.hpp"

namespace upsylon {

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! accept a pattern among a sequence
        //
        //______________________________________________________________________
        class Or : public OutOfOrder
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];                           //!< CLID
            static const uint32_t UUID = Y_FOURCC('_','O','R','_'); //!< UUID

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Or      *Create();                            //!< new
            virtual Pattern *clone() const;                       //!< copy
            virtual void     start(FirstChars &) const;           //!< merge until strong op
            virtual bool     accept(Y_PATTERN_ACCEPT_ARGS) const; //!< one of the operands
            virtual bool     feeble() const throw();              //!< one is feeble

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Or() throw();

        private:
            Y_DISABLE_ASSIGN(Or);
            explicit Or() throw();
            explicit Or(const Or&) throw();
            virtual void vizCore(ios::ostream &) const;
        };

    }

}

#endif
