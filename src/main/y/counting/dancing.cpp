#include "y/counting/dancing.hpp"
#include "y/counting/comb.hpp"


namespace upsylon
{
    dancing:: guest:: guest(const size_t i) throw() :  object(), label(i), next(0), prev(0) {}
    dancing::guest:: ~guest() throw() {}
}

#include <iostream>

namespace upsylon
{
    dancing:: group::  group() throw() : object(), guests(), next(0), prev(0) {}
    dancing:: group:: ~group() throw() {}

    bool dancing:: group:: has_guest_with_label( const size_t label ) const throw()
    {
        for(const guest *g = head; g; g=g->next )
        {
            if(label==g->label) return true;
        }
        return false;
    }

    bool dancing:: group:: is_distinct_from( const group *grp ) const throw()
    {
        assert(grp); assert(grp!=this);
        for(const guest *h=grp->head;h;h=h->next)
        {
            if(has_guest_with_label(h->label)) return false;
        }
        return true;
    }

    dancing::group * dancing::group:: single( const size_t label )
    {
        group *grp = new group();
        try { grp->push_back( new guest(label) ); } catch(...) { delete grp; throw; }
        return grp;
    }

    std::ostream & operator<<( std::ostream &os, const dancing:: group &grp )
    {
        os << '{';
        for(const dancing:: guest *g = grp.head; g; g=g->next)
        {
            os << g->label;
            if(g!=grp.tail) os << ',';
        }
        return (os << '}');
    }
}


namespace upsylon
{
    dancing:: configuration:: configuration(const size_t wgs) throw() :
    workgroup_size(wgs),
    workgroups(0),
    extraneous(0),
    next(0), prev(0)
    {}

    dancing:: configuration:: ~configuration() throw()
    {
    }


    bool dancing::configuration:: would_accept( const group *grp ) const throw()
    {
        for(const group *sub=head;sub;sub=sub->next)
        {
            if( !sub->is_distinct_from(grp) ) return false;
        }
        return true;
    }

    void dancing:: configuration:: finalize( const size_t n )
    {
        assert(size>0);
        assert(0==workgroups);
        assert(0==extraneous);

        // update groups
        for(const group *grp=head;grp;grp=grp->next)
        {
            assert(workgroup_size==grp->size);
            ++(size_t&)workgroups;
        }

        // check singles
        {
            groups singles;
            for(size_t label=1;label<=n;++label)
            {
                bool has_label = false;
                for(const group *grp=head;grp;grp=grp->next)
                {
                    if(grp->has_guest_with_label(label))
                    {
                        has_label = true;
                        break;
                    }
                }
                if(!has_label)
                {
                    singles.push_back( group::single(label) );
                    ++(size_t&)extraneous;
                }
            }
            merge_back(singles);
        }
    }

    std::ostream & operator<<( std::ostream &os, const dancing::configuration &cfg )
    {
        os << '{';
        for(const dancing:: group *grp = cfg.head; grp; grp=grp->next)
        {
            os << ' ' << *grp;
        }
        return (os << ' ' << '}' << '[' << cfg.workgroups << '+' << cfg.extraneous << ']' );
    }

}

#include "y/sort/merge.hpp"
#include "y/comparison.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    dancing:: ~dancing() throw()
    {

    }

    static inline
    int compare_cfg(const dancing::configuration *lhs,
                    const dancing::configuration *rhs,
                    void *) throw()
    {
        return comparison::decreasing(lhs->workgroups,rhs->workgroups);
    }

    static inline
    dancing::group * new_group_from( const combination &comb )
    {
        auto_ptr<dancing::group> grp = new dancing::group();
        for(size_t i=comb.k;i>0;--i)
        {
            grp->push_front( new dancing::guest( comb[i] ) );
        }
        return grp.yield();
    }

    dancing:: dancing(const size_t n,
                      const size_t k) :
    configurations()
    {
        const size_t  max_groups_per_cycle = n/k;
        //std::cerr << "dancing(" << n << "," << k << "): max_groups/cycle=" << max_groups_per_cycle << std::endl;

        //----------------------------------------------------------------------
        //
        // compute all the groups
        //
        //----------------------------------------------------------------------
        groups G;
        {
            combination  comb(n,k);
            for( comb.start(); comb.valid(); comb.next() )
            {
                G.push_back( new_group_from(comb) );
            }
        }
        //std::cerr << "#possible_groups=" << G.size << std::endl;

        //----------------------------------------------------------------------
        //
        // dispatch all the groups
        //
        //----------------------------------------------------------------------
        {
            configuration::list_type &configs = (configuration::list_type &) configurations;
            while(G.size>0)
            {
                auto_ptr<configuration> cfg = new configuration(k);
                groups        tmp;
                while( (G.size>0) && (cfg->size< max_groups_per_cycle) )
                {
                    group *grp = G.pop_front();
                    if( cfg->would_accept( grp ) )
                    {
                        cfg->push_back(grp);
                    }
                    else
                    {
                        tmp.push_back(grp);
                    }
                }
                G.merge_front(tmp);
                cfg->finalize(n);
                configs.push_back( cfg.yield() );
            }
            //std::cerr << "sorting..." << std::endl;
            merging<configuration>::sort(configs, compare_cfg, NULL);
        }
        for(const configuration *cfg = configurations.head; cfg; cfg=cfg->next)
        {
            std::cerr << *cfg << std::endl;
        }

    }


}
