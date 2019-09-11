//! \file
#ifndef Y_SORTED_LIST_INCLUDED
#define Y_SORTED_LIST_INCLUDED 1

#include "y/container/ordered.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/type/self-destruct.hpp"
#include "y/comparator.hpp"
#include "y/iterate/linked.hpp"

namespace upsylon
{

    //! ordered list
    template <typename T,
    typename COMPARATOR = increasing_comparator<T>
    >
    class sorted_list : public ordered<T>
    {
    public:
        //----------------------------------------------------------------------
        //
        // types and definitions
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type); //!< aliases

        //! built-in node to handle data
        class node_type
        {
        public:
            node_type   *next; //!< for list
            node_type   *prev; //!< for list
            mutable_type data; //!< the data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
            node_type(); ~node_type() throw();
        };

        typedef core::list_of<node_type> list_type; //!< low-level content
        typedef core::pool_of<node_type> pool_type; //!< memory pool

        //----------------------------------------------------------------------
        //
        // C++ interface
        //
        //----------------------------------------------------------------------

        //! cleaunp
        inline virtual ~sorted_list() throw() { release__(); }

        //!setup
        inline explicit sorted_list() throw() : content(), dormant(), compare() {}

        //! setup with memory
        inline explicit sorted_list(const size_t n, const as_capacity_t &) : content(), dormant(), compare()
        { reserve__(n); }

        //! copy
        inline explicit sorted_list(const sorted_list &other) : content(), dormant(), compare()
        {
            for(const node_type *scan = other.content.head; scan; scan=scan->next )
            {
                node_type *node = object::acquire1<node_type>();
                try        { new ( &(node->data) ) mutable_type(scan->data);        }
                catch(...) { object::release1<node_type>(node); release__(); throw; }
                content.push_back(node);
            }

        }

        //----------------------------------------------------------------------
        //
        // dynamic interface
        //
        //----------------------------------------------------------------------
        inline virtual size_t size()     const throw() { return content.size; }
        inline virtual size_t capacity() const throw() { return content.size + dormant.size; }

        //----------------------------------------------------------------------
        //
        // container interface
        //
        //----------------------------------------------------------------------
        inline virtual void free()    throw()       { free__(); }
        inline virtual void release() throw()       { release__(); }
        inline virtual void reserve(const size_t n) { reserve__(n); }

        //----------------------------------------------------------------------
        //
        // ordered interface
        //
        //----------------------------------------------------------------------

        //! sequential search
        virtual const_type *search( param_type args ) const throw()
        {
            for(const node_type *node=content.head;node;node=node->next)
            {
                if(args==node->data) return &(node->data);
            }
            return 0;
        }

        //! remove by sequential search
        virtual bool remove( param_type args ) throw()
        {
            for(node_type *node=content.head;node;node=node->next)
            {
                if(args==node->data)
                {
                    put_to_sleep( content.unlink(node) );
                    return true;
                }
            }
            return false;
        }

        //----------------------------------------------------------------------
        //
        // specific
        //
        //----------------------------------------------------------------------

        //! fast display
        inline friend std::ostream & operator<< ( std::ostream &os, const sorted_list &l )
        {
            os << '[';
            for(const node_type *node=l.content.head;node;node=node->next)
            {
                os << ' ' << node->data;
            }
            return os << ']' << '\'';
        }

        //----------------------------------------------------------------------
        //
        // iterators
        //
        //----------------------------------------------------------------------
        typedef iterate::linked<const_type,const node_type,iterate::forward>  iterator;            //!< forward iterator
        typedef iterator                                                const_iterator;            //!< const forward iterator
        iterator begin() const throw() { return iterator( content.head ); }                        //!< begin forward
        iterator end()   const throw() { return iterator(0);            }                          //!< end forward
        typedef iterate::linked<const_type,const node_type,iterate::reverse> reverse_iterator;     //!< reverse iterator
        typedef reverse_iterator                                       const_reverse_iterator;     //!< const reverse iterator
        reverse_iterator rbegin() const throw() { return reverse_iterator( content.tail ); }       //!< begin reverse
        reverse_iterator rend()   const throw() { return reverse_iterator(0);              }       //!< end reverse



    protected:
        //! insert args at a valid place
        inline void insert_multiple(const_type &args)
        {
            //------------------------------------------------------------------
            //
            // special cases
            //
            //------------------------------------------------------------------
            assert(check());
            switch(content.size)
            {
                case 0: content.push_back( query(args) ); assert(check()); return;
                case 1:
                    if(compare(args,content.head->data)<0)
                    {
                        content.push_front( query(args) );
                    }
                    else
                    {
                        content.push_back( query(args) );
                    }
                    assert(check());
                    return;
                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            // generic cases
            //
            //------------------------------------------------------------------
            assert(content.size>=2);
            const int cmpHead = compare(args,content.head->data);
            if(cmpHead<=0)
            {
                content.push_front( query(args) );
                assert(check());
                return;
            }
            else
            {
                assert(cmpHead>0);
                const int cmpTail = compare(args,content.tail->data);
                if(cmpTail>=0)
                {
                    content.push_back(query(args));
                    return;
                }
                else
                {
                    assert(cmpHead>0); assert(cmpTail<0);
                    const node_type *tail = content.tail;
                    node_type       *node = content.head;
                    while(node->next!=tail&&compare(args,node->next->data)>0)
                    {
                        node=node->next;
                    }
                    content.insert_after(node, query(args) );
                    assert(check());
                    return;
                }
            }
        }

        //! try to insert args at its only possible place
        inline bool insert_single( const_type &args )
        {
            //------------------------------------------------------------------
            //
            // special cases
            //
            //------------------------------------------------------------------
            assert( check_strict() );
            switch(content.size)
            {
                case 0: content.push_back( query(args) ); assert(check_strict()); return true;
                case 1: {
                    const int cmp = compare(args,content.head->data);
                    if(cmp<0)
                    {
                        content.push_front( query(args) ); assert(check_strict()); return true;
                    }
                    else if(cmp>0)
                    {
                        content.push_back( query(args) ); assert(check_strict()); return true;
                    }
                    else
                    {
                        assert(0==cmp);
                        return false;
                    }
                }
                default: assert(content.size>=2); break;
            }
            //------------------------------------------------------------------
            //
            // generic cases
            //
            //------------------------------------------------------------------
            assert(content.size>=2);
            const int cmpHead = compare(args,content.head->data);
            if(cmpHead<0)
            {
                //--------------------------------------------------------------
                //
                // before head
                //
                //--------------------------------------------------------------
                content.push_front( query(args) ); assert( check_strict() );
                return true;
            }
            else if(cmpHead>0)
            {
                //--------------------------------------------------------------
                //
                // after head
                //
                //--------------------------------------------------------------
                const int cmpTail = compare(args,content.tail->data);
                if(cmpTail>0)
                {
                    //----------------------------------------------------------
                    //
                    // and after tail
                    //
                    //----------------------------------------------------------
                    content.push_back( query(args) ); assert( check_strict() );
                    return true;
                }
                else if( cmpTail<0 )
                {
                    //----------------------------------------------------------
                    //
                    // but  before tail
                    //
                    //----------------------------------------------------------
                    assert(cmpHead>0); assert(cmpTail<0);
                    const node_type *tail = content.tail;
                    node_type       *curr = content.head;
                    while(true)
                    {
                        node_type *next = curr->next;
                        if(tail==next)
                        {
                            break; // early winner
                        }

                        const int cmp = compare(args,next->data);
                        if(cmp<0)
                        {
                            break;// winner
                        }
                        else if(cmp>0)
                        {
                            curr = next;
                            continue;
                        }
                        else
                        {
                            assert(0==cmp);
                            return false;
                        }
                    }
                    assert(curr); assert(curr->next);
                    assert( compare(args,curr->data) > 0 );
                    assert( compare(args,curr->next->data) < 0 );
                    content.insert_after(curr,query(args) ); assert(check_strict());
                    return true;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // already at tail
                    //
                    //----------------------------------------------------------
                    assert(0==cmpTail);
                    return false;
                }
            }
            else
            {
                assert(0==cmpHead);
                //--------------------------------------------------------------
                //
                // alread at head
                //
                //--------------------------------------------------------------
                return false;
            }
        }



    private:
        Y_DISABLE_ASSIGN(sorted_list);
        list_type          content;
        pool_type          dormant;
        mutable COMPARATOR compare;

        inline bool check() const throw()
        {
            for(const node_type *node =content.head;node;node=node->next)
            {
                if(node->next)
                {
                    if(compare(node->data,node->next->data)>0)
                        return false;
                }
            }
            return true;
        }

        inline bool check_strict() const throw()
        {
            for(const node_type *node =content.head;node;node=node->next)
            {
                if(node->next)
                {
                    if(compare(node->data,node->next->data)>=0)
                        return false;
                }
            }
            return true;
        }

        node_type *query( const_type &args )
        {
            node_type *node = (dormant.size>0) ? dormant.query() : object::acquire1<node_type>();
            try
            {
                new (&(node->data)) mutable_type( args );
            }
            catch(...)
            {
                dormant.store(node);
                throw;
            }
            return node;
        }

        inline void put_to_sleep(node_type *node) throw() { self_destruct(dormant.store(node)->data); }
        inline void reserve__(size_t n) { while(n-->0) { dormant.store( object::acquire1<node_type>() ); } }
        inline void free__() throw()    { while( content.size ) { put_to_sleep( content.pop_back() ); }    }

        inline void release__() throw()
        {
            while( content.size ) { node_type *node = content.pop_back(); self_destruct(node->data); object::release1(node); }
            while( dormant.size ) { node_type *node = dormant.query();                               object::release1(node); }
        }

        virtual const_type & getObjectAt(const size_t indx) const throw()
        {
            return content.fetch(indx+1)->data;
        }

    };

}

#endif

