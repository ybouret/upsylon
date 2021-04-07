
#include "y/ios/echostream.hpp"
#include "y/sequence/list.hpp"

namespace upsylon
{

    namespace ios
    {

        namespace
        {
            typedef list< arc_ptr<ostream> > targets;

        }

        echostream:: echostream() :
        impl( new targets() )
        {
        }

        echostream:: ~echostream() throw()
        {
            delete static_cast<targets *>(impl);
            impl = 0;
        }

        void echostream:: write(char C)
        {
            targets           &self = *static_cast<targets *>(impl);
            size_t            n     = self.size();
            targets::iterator i     = self.begin();
            while(n>0)
            {
                (*i)->write(C);
                --n;
                ++i;
            }
        }

        void echostream:: flush()
        {
            targets           &self = *static_cast<targets *>(impl);
            size_t            n     = self.size();
            targets::iterator i     = self.begin();
            while(n>0)
            {
                (*i)->flush();
                --n;
                ++i;
            }
        }

        void echostream:: clear() throw()
        {
            static_cast<targets *>(impl)->free();
        }

        echostream & echostream:: link(const arc_ptr<ostream> &p)
        {
            static_cast<targets *>(impl)->push_back(p);
            return *this;
        }

        echostream & echostream:: link(ostream *p)
        {
            const arc_ptr<ostream> _(p);
            return link(_);
        }
    }

}
