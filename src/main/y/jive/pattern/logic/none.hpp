
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
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('N','O','N','E');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  None    *Create();                  //!< new
            virtual Pattern *clone() const;             //!< copy
            virtual void     start(FirstChars &) const; //!< complete and remove ops
            virtual bool     accept(Y_PATTERN_ACCEPT_ARGS) const; //!<
            virtual bool     feeble() const throw();    //!< ...

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~None() throw();

        private:
            Y_DISABLE_ASSIGN(None);
            explicit None() throw();
            explicit None(const None&) throw();
            virtual void vizCore(ios::ostream &) const;
        };

    }

}

#endif
