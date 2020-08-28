
#include "y/jive/pattern.hpp"
#include "y/type/block/zset.hpp"
#include "y/ios/ostream.hpp"
#include "y/information/entropy.hpp"

namespace upsylon {

    namespace Jive
    {
        Pattern:: ~Pattern() throw()
        {
            _bzset(uuid);
        }

        Pattern:: Pattern(const uint32_t t) throw() :
        uuid(t), self(0), next(0), prev(0)
        {
        }

        Pattern:: Pattern(const Pattern &other) throw() :
        uuid(other.uuid), self(0), next(0), prev(0)
        {
        }

        size_t   Pattern:: id(ios::ostream&fp) const
        {
            return fp.write_nbo(uuid);
        }

        bool Pattern::  strong() const throw()
        {
            return !feeble();
        }

        bool Pattern:: Eq(const Pattern &lhs, const Pattern &rhs)
        {
            if(lhs.uuid!=rhs.uuid)
            {
                return false;
            }
            else
            {
                const string L = lhs.to_binary();
                const string R = rhs.to_binary();
                return L==R;
            }
        }

        double Pattern:: entropy() const throw()
        {
            Entropy E;
            update(E);
            return *E;
        }

    }

}
