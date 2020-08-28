


#include "y/jive/pattern/joker/repeating.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Repeating);

        Repeating:: ~Repeating() throw() {}

        Repeating::  Repeating(const Motif &m, const size_t n) throw() :
        Joker(UUID,m),
        minCount(n)
        {
            Y_PATTERN_SELF(Repeating);
        }

        Repeating::  Repeating(const Repeating &_) throw() :
        Joker(_),
        minCount(_.minCount)
        {
            assert(UUID==uuid);
            Y_PATTERN_SELF(Repeating);
        }

        Pattern *Repeating:: clone() const
        {
            return new Repeating(*this);
        }

        Repeating     * Repeating:: Create(Pattern *p, const size_t n)
        {
            const Motif m(p);
            return new Repeating(m,n);
        }


        const char *Repeating:: className() const throw()
        {
            return CLID;
        }

        size_t Repeating:: serialize(ios::ostream &fp) const
        {
            size_t ans = id(fp);
            ans += fp.write_upack(minCount);
            return ans + motif->serialize(fp);
        }


        void Repeating:: vizCore(ios::ostream &fp) const
        {
            endl(fp("[label=\">=%u\",shape=diamond]",unsigned(minCount)));
            vizLink(fp);
        }

        bool Repeating:: feeble() const throw()
        {
            if(minCount<=0)
            {
                return true;
            }
            else
            {
                return motif->feeble();
            }
        }

        bool Repeating:: accept(Y_PATTERN_ACCEPT_ARGS) const
        {
            assert(0==token.size);
            Token  cat;
            size_t num = 0;
            while(true)
            {
                Token tmp;
                if(motif->accept(tmp,source))
                {
                    cat.merge_back(tmp);
                    ++num;
                }
                else
                {
                    break;
                }
            }
            //std::cerr << "<Repeating #" << num << "/" << minCount << ">" << std::endl;
            if(num>=minCount)
            {
                token.swap_with(cat);
                return true;
            }
            else
            {
                source.unget(cat);
                return false;
            }
        }

    }
}
