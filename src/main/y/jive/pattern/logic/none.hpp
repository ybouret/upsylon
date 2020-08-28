
//! \file

#ifndef Y_PATTERN_LOGIC_NONE_INCLUDED
#define Y_PATTERN_LOGIC_NONE_INCLUDED 1

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
        class None : public OutOfOrder
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];                            //!< CLID
            static const uint32_t UUID = Y_FOURCC('N','O','N','E');  //!< UUID

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  None    *Create();                            //!< new
            virtual Pattern *clone() const;                       //!< copy
            virtual void     start(FirstChars &) const;           //!< complete and remove ops
            virtual bool     accept(Y_PATTERN_ACCEPT_ARGS) const; //!< single after none of operands
            virtual bool     feeble() const throw();              //!< false
            virtual void     optimize() throw();                  //!< applyOptimize...

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw(); //!< CLID

            //__________________________________________________________________
            //
            // reworking
            //__________________________________________________________________
            virtual void rework() throw();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~None() throw(); //!< cleanup

        private:
            Y_DISABLE_ASSIGN(None);
            explicit None() throw();
            explicit None(const None&) throw();
            virtual void vizCore(ios::ostream &) const;
        };

    }

}

#endif
