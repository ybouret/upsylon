
#include "y/lang/dynamo/symbols.hpp"

namespace upsylon
{
    namespace Lang
    {

        DynamoInfo:: DynamoInfo(const Tag &moduleID, const Syntax::Rule &r, const unsigned inf ) throw() :
        DynamoObject(),
        from(moduleID),
        rule(r),
        info(inf)
        {
        }

        DynamoInfo:: ~DynamoInfo() throw()
        {
        }

        DynamoInfo:: DynamoInfo( const DynamoInfo &other) throw() :
        DynamoObject(),
        from(other.from), rule(other.rule), info(other.info)
        {
        }

        const string & DynamoInfo:: key() const throw()
        {
            return rule.name;
        }

        std::ostream & operator<<( std::ostream &os, const DynamoInfo &di )
        {
            os << di.from << "_" << di.rule.name;
            return os;
        }


        template<>
        std::ostream & DynamoTerm :: display(std::ostream &os) const
        {
            switch(derived.attr)
            {
                case Syntax::Semantic: os << "(-)"; break;
                case Syntax::Standard: os << "(+)"; break;
                case Syntax::Operator: os << "(^)"; break;
            }
            return (os<< static_cast<const DynamoInfo &>(*this));
        }

        template <>
        std::ostream & DynamoRule :: display(std::ostream &os) const
        {
            switch(rule.uuid)
            {
                case Syntax::Aggregate::UUID: os << "[+]"; break;
                case Syntax::Alternate::UUID: os << "[-]"; break;
                default:                      os << "(?)"; break;
            }
            return (os<< static_cast<const DynamoInfo &>(*this));
        }


    }

}

namespace upsylon
{
    namespace Lang
    {
        DynamoSymbols:: DynamoSymbols() throw() :
        terminals(), internals()
        {

        }

        DynamoSymbols:: ~DynamoSymbols() throw()
        {
        }


    }

}

