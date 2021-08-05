

#include "y/alchemy/freezable.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        Freezable:: Freezable() throw() : frozen(false)
        {
        }

        Freezable:: ~Freezable() throw()
        {
        }

        void Freezable:: permit() throw()
        {
            aliasing::_(frozen) = false;
        }

        void Freezable:: freeze(const char *msg)
        {
            static const char aux[] = "already freezed!";
            if(frozen)
            {
                if(msg)
                {
                    throw exception("%s was %s!",msg,aux);
                }
                else
                {
                    throw exception("%s",aux);
                }
            }
            aliasing::_(frozen) = true;
        }


        Freezer:: Freezer(Freezable &h, const char *msg) :
        host(h)
        {
            host.freeze(msg);
        }

        Freezer:: ~Freezer() throw()
        {
            host.permit();
        }


    }

}


