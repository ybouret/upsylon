#include "y/chemical/sys/flux.hpp"

namespace upsylon
{
    namespace Chemical
    {

        Flux::Node:: ~Node() throw()
        {
        }


        Flux::Node:: Node(const Strain &S) :
        Object(), dnode<Node>(), Vizible(),
        genus(IsStrain)
        {
            strain = &S;
        }
    }

}
