
#include "y/string/defrag.hpp"
#include "y/sort/merge.hpp"

namespace upsylon
{
    string_defrag:: ~string_defrag() throw()
    {
        release();
    }

    string_defrag:: string_defrag() throw() :
    _list(),
    _pool()
    {
    }

    void string_defrag:: free() throw()
    {
        while( _list.size )
        {
            _pool.store( _list.pop_back() )->addr = 0;
        }
    }


    static inline
    int __s_node_compare( const core::addr_node<string> *lhs, const core::addr_node<string> *rhs, void *)
    {
        assert(lhs);
        assert(rhs);
        return comparison::increasing(lhs->addr->size(),rhs->addr->size());
    }

    void string_defrag:: record(string &s)
    {
        if( _pool.size )
        {
            _list.push_back( _pool.query() )->addr = &s;
        }
        else
        {
            _list.push_back( new s_node(&s) );
        }

    }

    void string_defrag:: release() throw()
    {
        _list.release();
        _pool.release();
    }

    void string_defrag:: remove(string &s) throw()
    {
        for( s_node *node = _list.head; node; node=node->next )
        {
            if( &s == node->addr )
            {
                _pool.store( _list.unlink(node) )->addr = 0;
                return;
            }
        }
    }



    size_t string_defrag:: one_cycle() throw()
    {
        size_t n_ok = 0;
        s_list temp;
        while(_list.size)
        {
            s_node *node = _list.pop_front();
            if(node->addr->compact())
            {
                _pool.store( node )->addr = 0;
                ++n_ok;
            }
            else
            {
                temp.push_back(node);
            }
        }
        _list.swap_with(temp);
        return n_ok;
    }

    size_t string_defrag:: cycle() throw()
    {
        merging<s_node>::sort(_list, __s_node_compare, NULL);
        size_t sum = 0;
        size_t tmp = 0;
        while( (tmp=one_cycle()) > 0 )
        {
            sum += tmp;
        }
        return sum;
    }

}
