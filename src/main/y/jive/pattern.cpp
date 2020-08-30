
#include "y/jive/pattern.hpp"
#include "y/type/block/zset.hpp"
#include "y/ios/ostream.hpp"
#include "y/information/entropy.hpp"
#include "y/ios/osstream.hpp"

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
        CountedObject(),
        Serializable(),
        Vizible(),
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

        bool  operator==(const Pattern &lhs, const Pattern &rhs)
        {
            return Pattern::Eq(lhs,rhs);
        }

        double Pattern:: entropy() const throw()
        {
            Entropy E;
            update(E);
            return *E;
        }

        ios::ostream & Pattern::Code(ios::ostream &fp, const uint8_t code)
        {
            if(
               ( (code>='0') && (code<='9') ) |
               ( (code>='a') && (code<='z') ) |
               ( (code>='A') && (code<='Z') )
               )
            {
                fp << char(code);
            }
            else
            {
                fp << "\\x" << hexadecimal::lowercase[code];
            }
            return fp;
        }
        
        string Pattern:: toRegExp() const
        {
            string        ans;
            {
                ios::osstream fp(ans);
                express(fp);
            }
            return ans;
        }
    }

}
