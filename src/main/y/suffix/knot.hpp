//! \file

#ifndef Y_SUFFIX_KNOT_INCLUDED
#define Y_SUFFIX_KNOT_INCLUDED 1

#include "y/suffix/node.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{

    template <typename T>
    class suffix_knot
    {
    public:
        typedef core::list_of<suffix_knot> list_;
        Y_DECL_ARGS(T,type);
        
        
        inline type       & operator*() throw() { return data; }
        inline const_type & operator*() const throw() { return data; }
        
        
        suffix_knot *next;
        suffix_knot *prev;
        void        *hook;
        type         data;
        
        inline  suffix_knot(const_type &args) : next(0), prev(0), hook(0), data(args) {}
        inline ~suffix_knot() throw() { assert(0==next); assert(0==prev); }
        
        //! list of alive knots
        class list_type : public list_
        {
        public:
            inline explicit list_type() throw() : list_() {}
            inline virtual ~list_type() throw()
            {
                while(this->size)
                {
                    suffix_knot *knot = this->pop_back();
                    self_destruct( *knot );
                    object::release1(knot);
                }
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(list_type);
        };
        
        //! pool of zombie nodes
        class pool_type : public list_
        {
        public:
            inline explicit pool_type() throw() : list_() {}
            inline virtual ~pool_type() throw()
            {
                while(this->size)
                {
                    suffix_knot *knot = this->pop_back();
                    object::release1(knot);
                }
            }
            
            inline void cache(size_t n) { while(n-- > 0) this->push_back( object::acquire1<suffix_knot>() ); }
            
            inline void store(suffix_knot *knot) throw()
            {
                assert(knot);
                self_destruct(*knot);
                this->push_front(knot);
            }
            
            inline void store(list_type &l) throw()
            {
                while(l.size) store( l.pop_back() );
            }
            
            inline suffix_knot *query(const_type &args)
            {
                suffix_knot *knot = (this->size>0) ? this->pop_front() : object::acquire1<suffix_knot>();
                try {
                    return new (knot) suffix_knot(args);
                } catch(...) {
                    this->push_front(knot);
                    throw;
                }
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool_type);

        };
        
       
    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_knot);
    };
    
    
}


#endif
