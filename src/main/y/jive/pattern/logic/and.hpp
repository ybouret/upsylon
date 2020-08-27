//! \file

#ifndef Y_PATTERN_LOGIC_AND_INCLUDED
#define Y_PATTERN_LOGIC_AND_INCLUDED 1

#include "y/jive/pattern/logic/logical.hpp"

namespace upsylon {

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! accept a sequence of patterns
        //
        //______________________________________________________________________
        class And : public Logical
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('A','N','D','_');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  And     *Create();                  //!< new
            virtual Pattern *clone() const;             //!< copy
            virtual void     start(FirstChars &) const; //!< merge until strong op
            virtual bool     accept(Y_PATTERN_ACCEPT_ARGS) const; //!< all
            virtual bool     feeble() const throw();    //!< all are feeble

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~And() throw();

        private:
            Y_DISABLE_ASSIGN(And);
            explicit And() throw();
            explicit And(const And&) throw();
            virtual void vizCore(ios::ostream &) const;
        };

    }

}

#endif
