
#include "y/jive/pattern/basic/rework.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Jive
    {

        Pattern * Rework:: Single2Range(Single *p, const uint8_t lower, const uint8_t upper) throw()
        {
            assert(p!=NULL);
            assert(lower<=upper);
            p->~Single();
            new (p) Range(lower,upper);
            return p;
        }


        //======================================================================
        //
        // lhs = Single
        //
        //======================================================================

        static inline
        Pattern *Merge(Single       *lhs,
                       const Single *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            const int L = lhs->code;
            const int R = rhs->code;
            switch(L-R)
            {
                case  0: return lhs;
                case -1: return Rework::Single2Range(lhs,lhs->code,rhs->code);
                case  1: return Rework::Single2Range(lhs,rhs->code,lhs->code);
                default:
                    break;
            }
            return NULL;
        }

        //======================================================================
        //
        // lhs = Range
        //
        //======================================================================
        static inline
        Pattern *Merge(Range        *lhs,
                       const Single *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            const int Lo = lhs->lower;
            const int Up = lhs->upper;
            const int Ch = rhs->code;
            if(Ch==Lo-1)
            {
                --aliasing::_(lhs->lower);
                return lhs;
            }

            if(Ch==Up+1)
            {
                ++aliasing::_(lhs->upper);
                return lhs;
            }

            if(Lo<=Ch&&Ch<=Up)
            {
                return lhs;
            }

            return NULL;
        }


        static inline
        Pattern *Merge(Range        *lhs,
                       const Range  *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            return NULL;
        }

        static inline
        Pattern * Merge( Pattern * & lhs, Pattern * &rhs ) throw()
        {
            // lhs is single
            if( lhs->is<Single>() )
            {
                if(rhs->is<Single>())
                {
                    return Merge( lhs->as<Single>(), rhs->as<Single>() );
                }

                if(rhs->is<Range>())
                {
                    cswap(lhs,rhs);
                    return Merge( lhs->as<Range>(), rhs->as<Single>() );
                }
            }

            // lhs is range
            if( lhs->is<Range>() )
            {
                if(rhs->is<Single>())
                {
                    cswap(lhs,rhs);
                    return Merge( lhs->as<Single>(), rhs->as<Single>() );
                }

                if(rhs->is<Range>())
                {
                    return Merge( lhs->as<Range>(), rhs->as<Range>() );

                }
            }

            return NULL;

        }

        static inline
        int TryMerge( Pattern::List &plist, Pattern *rhs ) throw()
        {
            assert(rhs!=NULL);
            Pattern::List tmp;
            while(plist.size)
            {
                Pattern *lhs = plist.pop_front();
                Pattern *mrg = Merge(lhs,rhs);
                if(mrg)
                {
                    tmp.push_back(mrg);
                    tmp.merge_back(plist);
                    delete rhs;
                    plist.swap_with(tmp);
                    return 0x01;
                }
                tmp.push_back(lhs);
            }

            // no merge
            plist.push_back(rhs);
            return 0x00;
        }

        void Rework:: Compact(Pattern::List &ops) throw()
        {
            while(ops.size>1)
            {
                Pattern::List plist;
                int           merge = 0x00;
                plist.push_back( ops.pop_front() );
                while(ops.size>0)
                {
                    merge |= TryMerge(plist,ops.pop_front());
                }
                ops.swap_with(plist);
                if(!merge)
                    break;
            }

        }

    }

}


