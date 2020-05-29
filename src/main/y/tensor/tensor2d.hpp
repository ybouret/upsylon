//! file

#ifndef Y_TENSOR2D_INCLUDED
#define Y_TENSOR2D_INCLUDED 1

#include "y/tensor/tensor1d.hpp"

namespace upsylon {

    namespace core
    {
        //----------------------------------------------------------------------
        //
        //! common stuff for tensor2d
        //
        //----------------------------------------------------------------------
        class tensor2d : public tensor1d
        {
        public:
            virtual ~tensor2d() throw();  //!< cleanup
            const size_t rows;            //!< number of rows
            const size_t items_per_slice; //!< rows*cols = items per slice
            
        protected:
            //! setup
            explicit tensor2d(const size_t r, const size_t c);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tensor2d);
        };
    }
    
    //! forward declaration
    template <typename T> class tensor3d;
    
    //--------------------------------------------------------------------------
    //
    //! tensor2d
    //
    //--------------------------------------------------------------------------
    template <typename T>
    class tensor2d : public core::tensor2d,
    public addressable< upsylon::tensor1d<T> >
    {
    public:
        //----------------------------------------------------------------------
        //
        // types and definitions
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type);               //!< aliases
        typedef upsylon::tensor1d<T> row;  //!< alias
        
        //----------------------------------------------------------------------
        //
        // C++
        //
        //----------------------------------------------------------------------
        
        //! cleanup
        inline virtual ~tensor2d() throw() { cleanup(); }
        
        //! setup with private memory
        inline explicit tensor2d(const size_t r, const size_t c) :
        core::tensor2d(r,c), __row(0)
        {
            mutable_type *objAddr  = 0;
            memory::embed emb[] =
            {
                memory::embed::as(__row,rows),
                memory::embed::as(objAddr,items_per_slice)
            };
            create(emb,sizeof(emb)/sizeof(emb[0]));
            __row -= 1;
            build(objAddr);
        }
        
        //----------------------------------------------------------------------
        //
        // addressable<row>
        //
        //----------------------------------------------------------------------
        
        //! row access
        inline virtual row & operator[](const size_t r) throw()
        {
            assert(r>0);
            assert(r<=rows);
            return __row[r];
        }
        
        //! row access, const
        inline virtual const row & operator[](const size_t r) const throw()
        {
            assert(r>0);
            assert(r<=rows);
            return __row[r];
        }
        
        //! addressable interface
        inline virtual size_t size() const throw() { return rows; }
        
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(tensor2d);
        row          *__row;
       
        
        inline void cleanup() throw()
        {
            while(built>0)
            {
                self_destruct(__row[built--] );
            }
            __row = 0;
        }
        
        inline void build(mutable_type * &data)
        {
            assert(__row);
            assert(data);
            row          *base = __row+1;
            try
            {
                while(built<rows)
                {
                    new (base) row(cols,data);
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
        
        friend class upsylon::tensor3d<T>;
        
        //! setup with other memory, shift memories
        inline explicit tensor2d(const size_t r,
                                 const size_t c,
                                 row          * &rowAddr,
                                 mutable_type * &objAddr) :
        core::tensor2d(r,c), __row(rowAddr)
        {
            assert(rowAddr!=NULL);
            assert(objAddr!=NULL);
            __row -= 1;
            build(objAddr);
            rowAddr += rows;
        }
        
    };
    
}

#endif

