

#include "y/alchemy/freezable.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        Freezable:: Freezable() throw() : freezed(false)
        {
        }

        Freezable:: ~Freezable() throw()
        {
        }

        void Freezable:: permit() throw()
        {
            aliasing::_(freezed) = false;
        }

        void Freezable:: freeze(const char *msg)
        {
            static const char aux[] = "already freezed!";
            if(freezed)
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
            aliasing::_(freezed) = true;
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


