
#include "y/jive/pattern/joker/counting.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Counting);

        Counting:: ~Counting() throw() {}

        Counting::  Counting(const Motif &m, const size_t nmin, const size_t nmax) throw() :
        Joker(UUID,m),
        minCount(nmin),
        maxCount(nmax)
        {
            assert(nmin<=nmax);
            Y_PATTERN_SELF(Counting);
        }

        Counting::  Counting(const Counting &_) throw() :
        Joker(_),
        minCount(_.minCount),
        maxCount(_.maxCount)
        {
            assert(UUID==uuid);
            Y_PATTERN_SELF(Counting);
        }

        Pattern *Counting:: clone() const
        {
            return new Counting(*this);
        }




        const char *Counting:: className() const throw()
        {
            return CLID;
        }

        size_t Counting:: serialize(ios::ostream &fp) const
        {
            size_t ans = id(fp);
            ans += fp.write_upack(minCount);
            ans += fp.write_upack(maxCount);
            return ans + motif->serialize(fp);
        }


        void Counting:: vizCore(ios::ostream &fp) const
        {
            endl(fp("[label=\"{%u,%u}\",shape=diamond]",unsigned(minCount),unsigned(maxCount)));
            vizLink(fp);
        }

        bool Counting:: feeble() const throw()
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

        bool Counting:: accept(Y_PATTERN_ACCEPT_ARGS) const
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
            if(minCount<=num&&num<=maxCount)
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
        
        void Counting::express(ios::ostream &fp) const
        {
            motif->express(fp);
            fp("{%lu,%lu}",(unsigned long)minCount, (unsigned long)maxCount);
        }


    }
}

#include "y/jive/pattern/logic/none.hpp"
#include "y/jive/pattern/joker/optional.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Jive
    {

        Pattern * Counting:: Create( Pattern *p, size_t nmin, size_t nmax)
        {
            auto_ptr<Pattern> q(p);

            if(nmin>nmax) cswap(nmin,nmax);
            if(nmin<=0)
            {
                switch(nmax)
                {
                    case 0:
                    {
                        auto_ptr<Logical> l = None::Create();
                        l->push_back( q.yield() );
                        return l.yield();
                    }

                    case 1: return Optional::Create( q.yield() );

                    default:
                        break;
                }
            }
            const Motif m( q.yield() );
            return new Counting(m,nmin,nmax);

        }

    }
}
