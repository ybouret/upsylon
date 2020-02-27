#include "y/information/translator/bytewise.hpp"
#include "y/information/modulation/echo.hpp"

namespace upsylon {

    namespace Information {

        const char BytewiseTranslator:: FMID[] = "Bytewise";

        const char *BytewiseTranslator:: family() const throw() { return FMID; }

        BytewiseTranslator:: ~BytewiseTranslator() throw()
        {

        }

        BytewiseTranslator:: BytewiseTranslator( const Modulation::pointer & m ) throw() :
        wksp(),
        transform(m)
        {

        }

        static inline Modulation *  MakeEchoAt(void *addr, const size_t size) throw()
        {
            assert(addr);
            assert(size>=sizeof(EchoModulation));
            memset(addr,0,size);
            Modulation *m = new (addr) EchoModulation();
            m->withhold();
            return m;
        }
        
        BytewiseTranslator:: BytewiseTranslator(Modulation *m) throw() :
        wksp(),
        transform(m ? m : MakeEchoAt(wksp,sizeof(wksp) ))
        {

        }

        const char *BytewiseTranslator:: name() const throw()
        {
            return transform->name();
        }

        void BytewiseTranslator:: reset() throw()
        {
            transform->reset();
            free();
        }

        void BytewiseTranslator:: flush()
        {
        }

        void BytewiseTranslator:: write(char C)
        {
            push_back( transform->fetch(C) );
        }



    }

}

