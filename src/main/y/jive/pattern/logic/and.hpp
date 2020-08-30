//! \file

#ifndef Y_PATTERN_LOGIC_AND_INCLUDED
#define Y_PATTERN_LOGIC_AND_INCLUDED 1

#include "y/jive/pattern/logic/seq.hpp"

namespace upsylon {

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! accept a sequence of patterns
        //
        //______________________________________________________________________
        class And : public Sequential
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];                           //!< CLID
            static const uint32_t UUID = Y_FOURCC('A','N','D','_'); //!< UUID

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  And     *Create();                          //!< new
            virtual Pattern *clone() const;                     //!< copy
            virtual void     start(FirstChars &) const;         //!< merge until strong op
            virtual bool     feeble() const throw();            //!< all are feeble
            virtual void     optimize() throw();                //!< applyOptimize, merge alike
            virtual void     express(ios::ostream &) const;     //!< escaped

            //! accept all operands or none
            /**
             the produced token is the catenation of all produced by operangss
             */
            virtual bool     accept(Y_PATTERN_ACCEPT_ARGS) const;

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw(); //!< CLID
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~And() throw(); //!< cleanup

        private:
            Y_DISABLE_ASSIGN(And);
            explicit And() throw();
            explicit And(const And&) throw();
            virtual void vizCore(ios::ostream &) const;
        };

    }

}

#endif
