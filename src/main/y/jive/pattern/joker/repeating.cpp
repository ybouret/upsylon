


#include "y/jive/pattern/joker/repeating.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace Jive
    {
        const char Repeating::CLID_ZOM[] = Y_FOURCC_CHAR8(Repeating::UUID_ZOM);
        const char Repeating::CLID_OOM[] = Y_FOURCC_CHAR8(Repeating::UUID_OOM);

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
            switch(minCount)
            {
                case 0: return CLID_ZOM;
                case 1: return CLID_OOM;
                default:
                    break;
            }
            return CLID;

        }

        size_t Repeating:: serialize(ios::ostream &fp) const
        {
            size_t ans = 0;
            switch(minCount)
            {
                case 0:  ans = fp.write_nbo(UUID_ZOM); break;
                case 1:  ans = fp.write_nbo(UUID_OOM); break;
                default:
                    ans  = fp.write_nbo(UUID);
                    ans += fp.write_upack(minCount);
                    break;
            }
            return ans + motif->serialize(fp);
        }


        void Repeating:: vizCore(ios::ostream &fp) const
        {
            fp << "[label=\"";
            switch(minCount)
            {
                case 0:  fp << '*'; break;
                case 1:  fp << '+'; break;
                default: fp(">=%u",unsigned(minCount)); break;
            }
            fp << "\",shape=diamond]";
            endl(fp);
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
        
        void Repeating::express(ios::ostream &fp) const
        {
            motif->express(fp);
            fp("{%lu}",(unsigned long)minCount);
        }


    }
}
