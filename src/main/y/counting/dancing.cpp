#include "y/counting/dancing.hpp"
#include "y/counting/comb.hpp"


namespace upsylon
{
    dancing:: guest::  guest(const size_t i) throw() :  object(), inode<guest>(), label(i) {}
    dancing:: guest:: ~guest()               throw() {}
}

#include <iostream>

namespace upsylon
{
    dancing:: group::  group() throw() : object(), guests(), inode<group>() {}
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
    dancing:: frame:: frame(const size_t wgs) throw() :
    inode<frame>(),
    workgroup_size(wgs),
    workgroups(0),
    extraneous(0)
    {
    }


    dancing:: frame:: ~frame() throw()
    {
    }


    bool dancing::frame:: would_accept( const group *grp ) const throw()
    {
        for(const group *sub=head;sub;sub=sub->next)
        {
            if( !sub->is_distinct_from(grp) ) return false;
        }
        return true;
    }

    void dancing:: frame:: finalize( const size_t n )
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

    std::ostream & operator<<( std::ostream &os, const dancing::frame &cfg )
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
    int compare_cfg(const dancing::frame *lhs,
                    const dancing::frame *rhs,
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
    frames(),
    wg_max(0),
    wg_min(0),
    amount(0)
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
            (size_t &) amount = comb.count;
            for(comb.boot(); comb.good(); comb.next())
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
            //__________________________________________________________________
            //
            // build the configurations
            //__________________________________________________________________
            frame::list_type &configs = (frame::list_type &) frames;
            while(G.size>0)
            {
                auto_ptr<frame> cfg = new frame(k);
                groups          tmp;
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

            //__________________________________________________________________
            //
            // sort them by decreasing (valid) workgroups
            //__________________________________________________________________
            merging<frame>::sort(configs, compare_cfg, NULL);
        }
        
        (size_t&)wg_max = frames.head->workgroups;
        (size_t&)wg_min = frames.tail->workgroups;

    }

    size_t   dancing:: find(const size_t  workgroups,
                            const frame * &ini,
                            const frame * &end) const throw()
    {
        assert(ini==NULL);
        assert(end==NULL);

        const frame *cfg = frames.head;

        while(cfg)
        {
            if(workgroups==cfg->workgroups)
            {
                // this is the first value;
                size_t ans = 1;
                ini = end = cfg;

                // check contiguous valid configuration
                while(true)
                {
                    cfg = cfg->next;
                    if(!cfg || workgroups!=cfg->workgroups) break;
                    ++ans;
                    end = cfg;
                }

                assert(ini);
                assert(end);
                
                return ans;

            }
            cfg = cfg->next;
        }

        // nothing was found
        return 0;
    }
}
