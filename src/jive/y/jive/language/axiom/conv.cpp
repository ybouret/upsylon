
#include "y/jive/language/axiom/all.hpp"

namespace upsylon
{
    namespace Jive
    {

        namespace Language
        {

#define Y_AXIOM_CONV_ERR(TARGET) throw exception("Axiom <%s> [%s] is no %s", **name, fourcc_(uuid), #TARGET)

            template <>
            const Aggregate & Axiom:: to<Aggregate>() const
            {
                switch(uuid)
                {
                    case Aggregate::UUID: return as<Aggregate>();
                    default:
                        break;
                }
                Y_AXIOM_CONV_ERR(Aggregate);
            }

            template <>
            const Alternate & Axiom:: to<Alternate>() const
            {
                switch(uuid)
                {
                    case Alternate::UUID: return as<Alternate>();
                    default:
                        break;
                }
                Y_AXIOM_CONV_ERR(Alternate);
            }

            template<>
            const Compound & Axiom:: to<Compound>() const
            {
                switch(uuid)
                {
                    case Alternate::UUID: return as<Alternate>();
                    case Aggregate::UUID: return as<Aggregate>();
                    default:
                        break;
                }
                Y_AXIOM_CONV_ERR(Compound);
            }

            template <>
            const Repeat & Axiom:: to<Repeat>() const
            {
                switch(uuid)
                {
                    case Repeat::UUID: return as<Repeat>();
                    default:
                        break;
                }
                Y_AXIOM_CONV_ERR(Repeat);
            }

            template <>
            const Option & Axiom:: to<Option>() const
            {
                switch(uuid)
                {
                    case Option::UUID: return as<Option>();
                    default:
                        break;
                }
                Y_AXIOM_CONV_ERR(Option);
            }



            template<>
            const Wildcard & Axiom:: to<Wildcard>() const
            {
                switch(uuid)
                {
                    case Repeat::UUID: return as<Repeat>();
                    case Option::UUID: return as<Option>();
                    default:
                        break;
                }
                Y_AXIOM_CONV_ERR(Wildcard);
            }
        }
    }
}
