
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
            Range *r = new (p) Range(lower,upper);
            std::cerr << "---> merging to " << *r << std::endl;
            return r;
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
                case  0:
                    std::cerr << "---> remove duplicate " << *lhs << std::endl;
                    return lhs;
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
                std::cerr << "---> " << *lhs << " => ";
                --aliasing::_(lhs->lower);
                std::cerr << *lhs << std::endl;
                return lhs;
            }

            if(Ch==Up+1)
            {
                std::cerr << "---> " << *lhs << " => ";
                ++aliasing::_(lhs->upper);
                std::cerr << *lhs << std::endl;
                return lhs;
            }

            if(Lo<=Ch&&Ch<=Up)
            {
                std::cerr << "---> remove included " << *rhs << std::endl;
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
            const Range *a = lhs;
            const Range *b = rhs;

            if(a->lower>b->lower)
            {
                cswap(a,b);
            }
            assert(a->lower<=b->lower);

            if(b->upper<=a->upper)
            {
                std::cerr << "---> remove included " << *b << " in " << *a << std::endl;
                aliasing::_(lhs->lower) = a->lower;
                aliasing::_(lhs->upper) = a->upper;
                return lhs;
            }
            assert(b->upper>a->upper);

            const int a_up = a->upper;
            const int b_lo = b->lower;
            if(b_lo<=a_up+1)
            {
                std::cerr << "---> gathering intersection " << *a << " and " << *b << std::endl;
                aliasing::_(lhs->lower) = a->lower;
                aliasing::_(lhs->upper) = b->upper;
                return lhs;
            }

            // distinct
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
                    assert(lhs->is<Range >());
                    assert(rhs->is<Single>());
                    return Merge( lhs->as<Range>(), rhs->as<Single>() );
                }
            }

            // lhs is range
            if( lhs->is<Range>() )
            {
                if(rhs->is<Single>())
                {
                    return Merge( lhs->as<Range>(), rhs->as<Single>() );
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
            plist.swap_with(tmp);
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


