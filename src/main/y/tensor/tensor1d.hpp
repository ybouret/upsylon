//! file
#ifndef Y_TENSOR1D_INCLUDED
#define Y_TENSOR1D_INCLUDED 1

#include "y/type/self-destruct.hpp"
#include "y/sequence/addressable.hpp"
#include "y/memory/embed.hpp"

namespace upsylon {

    namespace core
    {
        //----------------------------------------------------------------------
        //
        //! common stuff for tensor1d
        //
        //----------------------------------------------------------------------
        class tensor1d
        {
        public:
            virtual ~tensor1d() throw(); //!< cleanup
            const size_t cols;           //!< number of columns
            
            size_t allocated() const throw(); //!< private bytes
            
        protected:
            explicit tensor1d(const size_t c); //!< setup
            size_t   built;                    //!< shared construction level
           
            //! create and plug global private memory
            void   create(memory::embed [],const size_t);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tensor1d);
            size_t   bytes;
            void    *where;
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
            memory::embed emb[] =
            {
                memory::embed::as(__col,cols)
            };
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
                    new (base+built) mutable_type();
                    ++built;
                }
            }
            catch(...)
            {
                cleanup();
                throw;
            }
        }

        friend class upsylon::tensor2d<T>;
        inline tensor1d(const size_t n, void *data) :
        core::tensor1d(n),
        __col( static_cast<mutable_type *>(data)-1 )
        {
            build();
        }

    };
    
}

#endif
