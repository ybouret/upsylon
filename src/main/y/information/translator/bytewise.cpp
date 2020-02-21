#include "y/information/translator/bytewise.hpp"
#include "y/information/modulation/echo.hpp"

namespace upsylon {

    namespace information {

        const char BytewiseTranslator:: FMID[] = "Bytewise";

        const char *BytewiseTranslator:: family() const throw() { return FMID; }

        BytewiseTranslator:: ~BytewiseTranslator() throw()
        {

        }

        BytewiseTranslator:: BytewiseTranslator( const modulation::pointer & m ) throw() :
        wksp(),
        transform(m)
        {

        }

        static inline modulation *  MakeEchoAt(void *addr, const size_t size) throw()
        {
            assert(addr);
            assert(size>=sizeof(echo_modulation));
            memset(addr,0,size);
            modulation *m = new (addr) echo_modulation();
            m->withhold();
            return m;
        }
        
        BytewiseTranslator:: BytewiseTranslator(modulation *m) throw() :
        wksp(),
        transform(m ? m : MakeEchoAt(wksp,sizeof(wksp) ))
        {

        }

        const char *BytewiseTranslator:: name() const throw()
        {
            return "todo";
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

