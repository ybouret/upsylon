//! file

#ifndef Y_TENSOR1D_INCLUDED
#define Y_TENSOR1D_INCLUDED 1

#include "y/tensor/tensor.hpp"
#include "y/type/self-destruct.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {

    namespace core
    {

        //----------------------------------------------------------------------
        //
        //! common stuff for tensor1d
        //
        //----------------------------------------------------------------------
        class tensor1d : public tensor
        {
        public:
            virtual ~tensor1d() throw();         //!< cleanup

        protected:
            explicit tensor1d(const size_t c); //!< setup


            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tensor1d);

        };
    }
 
    //! forward declaration
    template <typename> class tensor2d;
    
    //--------------------------------------------------------------------------
    //
    //! tensor1d
    //
    //--------------------------------------------------------------------------
    
    template <typename T>
    class tensor1d : public core::tensor1d, public addressable<T>
    {
    public:
        //----------------------------------------------------------------------
        //
        // types and definitions
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type); //!< aliases

        //----------------------------------------------------------------------
        //
        // C++
        //
        //----------------------------------------------------------------------
        
        //! cleanup
        inline virtual ~tensor1d() throw() { cleanup(); }

        //! setup
        inline tensor1d(const size_t n) :
        core::tensor1d(n),
        __col(0)
        {
            memory::embed emb[] = { memory::embed::as(__col,cols) };
            create(emb,sizeof(emb)/sizeof(emb[0]));
            __col -= 1;
            build();
        }

        //----------------------------------------------------------------------
        //
        // addressable<T>
        //
        //----------------------------------------------------------------------
        
        //! cols
        inline virtual size_t       size()                  const throw() { return cols; }
        
        //! access
        inline virtual type       & operator[](size_t indx)       throw() { assert(indx>0); assert(indx<=cols); return __col[indx]; }
        
        //! access, const
        inline virtual const_type & operator[](size_t indx) const throw() { assert(indx>0); assert(indx<=cols); return __col[indx]; }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(tensor1d);
        mutable_type *__col;
      
        
        inline void cleanup() throw()
        {
            while(built>0)
            {
                self_destruct( __col[built--] );
            }
            __col = 0;
        }

        inline void build()
        {
            assert(__col);
            mutable_type *base = __col+1;
            try {
                while(built<cols)
                {
                    new (base) mutable_type();
                    ++built;
                    ++base;
                }
            }
            catch(...)
            {
                cleanup();
                throw;
            }
        }

        friend class upsylon::tensor2d<T>;
        
        //! build from other data, shift address
        inline tensor1d(const size_t n, mutable_type * &data) :
        core::tensor1d(n),
        __col( data-1 )
        {
            build();
            data += cols;
        }

    };
    
}

#endif
