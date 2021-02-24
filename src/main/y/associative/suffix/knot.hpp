//! \file

#ifndef Y_SUFFIX_KNOT_INCLUDED
#define Y_SUFFIX_KNOT_INCLUDED 1

#include "y/core/list.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/args.hpp"
#include "y/object.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! terminal knots for a graph
    //
    //__________________________________________________________________________
    template <typename T>
    class suffix_knot
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef core::list_of<suffix_knot> list_; //!< alias
        Y_DECL_ARGS(T,type);                      //!< aliasies
        
        //______________________________________________________________________
        //
        // C+
        //______________________________________________________________________
        //! setup empty
        inline  suffix_knot(const_type &args) : next(0), prev(0), hook(0), data(args) {}
        //! cleanup
        inline ~suffix_knot() throw() { assert(0==next); assert(0==prev); }
        
        //! access
        inline type       & operator*() throw()       { return data; }
        //! const access
        inline const_type & operator*() const throw() { return data; }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        suffix_knot *next; //!< for list
        suffix_knot *prev; //!< for list
        void        *hook; //!< for tree
        type         data; //!< actual data
      
        //______________________________________________________________________
        //
        //! list of alive knots
        //______________________________________________________________________
        class list_type : public list_
        {
        public:
            //! setup
            inline explicit list_type() throw() : list_() {}
            
            //! cleanup
            inline virtual ~list_type() throw()
            {
                release();
            }
            
            //! destruct and release nodes
            inline void release() throw()
            {
                while(this->size)
                {
                    suffix_knot *knot = this->pop_back();
                    self_destruct::on( *knot );
                    object::release1(knot);
                }
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(list_type);
        };
        
        //______________________________________________________________________
        //
        //! pool of zombie nodes
        //______________________________________________________________________
        class pool_type : public list_
        {
        public:
            //! setup
            inline explicit pool_type() throw() : list_() {}
            
            //! cleanup
            inline virtual ~pool_type() throw()
            {
                release();
            }
            
            //! release zombie nodes
            inline void release() throw()
            {
                while(this->size)
                {
                    suffix_knot *knot = this->pop_back();
                    object::release1(knot);
                }
            }
            
            //! load zombie nodes
            inline void cache(size_t n) { while(n-- > 0) this->push_back( object::acquire1<suffix_knot>() ); }
            
            //! destruct and load knot
            inline void store(suffix_knot *knot) throw()
            {
                assert(knot);
                self_destruct::on(*knot);
                this->push_front(knot);
            }
            
            //! destruct all list
            inline void store(list_type &l) throw()
            {
                while(l.size) store( l.pop_back() );
            }
            
            //! query a zombie and make it alive with args
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
