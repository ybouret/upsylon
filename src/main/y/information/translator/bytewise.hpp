//! \file

#ifndef Y_INFORMATION_TRANSLATOR_BYTEWISE_INCLUDED
#define Y_INFORMATION_TRANSLATOR_BYTEWISE_INCLUDED 1

#include "y/information/translator/queue.hpp"
#include "y/information/modulation.hpp"

namespace upsylon {

    namespace information {

        class BytewiseTranslator : public TranslatorQueue
        {
        public:
            static const char FMID[];
            
            explicit BytewiseTranslator( const modulation::pointer & ) throw();
            explicit BytewiseTranslator( modulation *) throw();
            virtual ~BytewiseTranslator() throw();

            virtual const char *family() const throw();
            virtual void        reset() throw(); //!< transform reset and free
            virtual void        write(char C);   //!< append transformed char
            virtual void        flush();         //!< do nothing

            virtual const char *name() const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(BytewiseTranslator);
            uint64_t            wksp[4];
            modulation::pointer transform;

            //static modulation *MakeEchoAt(void *addr, const size_t size) throw();
        };

    }
}



#endif

