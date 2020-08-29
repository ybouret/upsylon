//! \file

#ifndef Y_PATTERN_LOGIC_OR_INCLUDED
#define Y_PATTERN_LOGIC_OR_INCLUDED 1

#include "y/jive/pattern/logic/rev.hpp"

namespace upsylon {

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! accept a pattern among a sequence
        //
        //______________________________________________________________________
        class Or : public Reworkable
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
            virtual bool     feeble() const throw();              //!< one is feeble
            virtual void     optimize() throw();                  //!< applyOptimize, merge alike, rework

            //! accept one of the operands
            /**
             give its chance to operands after a matchin feeble!
             */
            virtual bool     accept(Y_PATTERN_ACCEPT_ARGS) const; 

            
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
            virtual ~Or() throw(); //!< cleanup



        private:
            Y_DISABLE_ASSIGN(Or);
            explicit Or() throw();
            explicit Or(const Or&) throw();
            virtual void vizCore(ios::ostream &) const;
        };

    }

}

#endif
