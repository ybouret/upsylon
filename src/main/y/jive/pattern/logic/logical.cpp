
#include "y/jive/pattern/logic/logical.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive
    {
        Logical:: ~Logical() throw()
        {
        }

        Logical:: Logical(const uint32_t t) throw() :
        Pattern(t), Operands()
        {
        }



        Logical:: Logical(const Logical &_) :
        Pattern(_), Operands()
        {
            for(const Pattern *op=_.head;op;op=op->next)
            {
                  push_back( op->clone() );
            }
        }

        size_t Logical:: serialize(ios::ostream &fp) const
        {
            size_t ans=0;
            ans += id(fp);
            ans += fp.write_upack(size);
            for(const Pattern *op=head;op;op=op->next)
            {
                ans += op->serialize(fp);
            }
            return ans;
        }




        void Logical:: load(ios::istream &fp)
        {
            static const char fn[] = "Jive::Logical::Load: ";
            assert(0==size);
            size_t shift = 0;
            size_t nops  = 0;
            if( !fp.query_upack(nops,shift) ) throw exception("%s missing #ops for %s", fn, className() );
            while(nops-- > 0 )
            {
                push_back( Load(fp) );
            }
        }

        void Logical:: vizLink(ios::ostream &fp) const
        {
            for(const Pattern *op=head;op;op=op->next)
            {
                op->vizSave(fp);
                vizName(fp);
                fp << " -> ";
                op->vizName(fp);
                endl(fp);
            }
        }

    }

}
