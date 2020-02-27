//! \file

#ifndef Y_INFORMATION_TRANSLATOR_BYTEWISE_INCLUDED
#define Y_INFORMATION_TRANSLATOR_BYTEWISE_INCLUDED 1

#include "y/information/translator/queue.hpp"
#include "y/information/modulation.hpp"

namespace upsylon {

    namespace Information {

        //! apply a modulation
        class BytewiseTranslator : public TranslatorQueue
        {
        public:
            static const char FMID[]; //!< Bytewise
            
            explicit BytewiseTranslator(const Modulation::Pointer &) throw(); //!< setup
            explicit BytewiseTranslator(Modulation *) throw();                //!< setup from a new modulation, echo by default
            virtual ~BytewiseTranslator() throw();                            //!< cleanup

            virtual const char *family() const throw(); //!< FMID
            virtual void        reset() throw();        //!< transform reset and free
            virtual void        write(char C);          //!< append transformed char
            virtual void        flush();                //!< do nothing
            virtual const char *name() const throw();   //!< modulation name

        private:
            Y_DISABLE_COPY_AND_ASSIGN(BytewiseTranslator);
            uint64_t            wksp[4];
            Modulation::Pointer transform;
        };

    }
}



#endif

