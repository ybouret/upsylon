

//! file
#ifndef Y_TENSOR3D_INCLUDED
#define Y_TENSOR3D_INCLUDED 1

#include "y/tensor/tensor2d.hpp"

namespace upsylon {
    
    namespace core
    {
        //----------------------------------------------------------------------
        //
        //! common stuff for tensor3d
        //
        //----------------------------------------------------------------------
        class tensor3d : public tensor2d
        {
        public:
            virtual ~tensor3d() throw(); //!< cleanup
            const size_t slices;         //!< number of slices
            const size_t it3d;           //!< rows*cols*slices = items per frame
       
        protected:
            //! setup
            explicit tensor3d(const size_t r, const size_t c, const size_t s);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tensor3d);
        };
    }
    
    //! forward declaration
    template <typename T> class tensor4d;
    
    //--------------------------------------------------------------------------
    //
    //! tensor3d
    //
    //--------------------------------------------------------------------------
    template <typename T>
    class tensor3d : public core::tensor3d,
    public addressable< upsylon::tensor2d<T> >
    {
    public:
        //----------------------------------------------------------------------
        //
        // types and definitions
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type);                 //!< aliases
        typedef upsylon::tensor1d<T> row;    //!< alias
        typedef upsylon::tensor2d<T> slice;  //!< alias

        //----------------------------------------------------------------------
        //
        // C++
        //
        //----------------------------------------------------------------------
        
        //! cleanup
        inline virtual ~tensor3d() throw() { cleanup(); }
        
        //! setup with private data
        inline explicit tensor3d(const size_t r, const size_t c, const size_t s) :
        core::tensor3d(r,c,s),
        __slice(0)
        {
            row          *rowAddr = 0;
            mutable_type *objAddr = 0;
            memory::embed emb[] = {
                memory::embed::as(__slice,slices),
                memory::embed::as(rowAddr,slices*rows),
                memory::embed::as(objAddr,it3d)
            };
            create(emb,sizeof(emb)/sizeof(emb[0]));
            __slice -= 1;
            build(rowAddr,objAddr);
        }
        
        //----------------------------------------------------------------------
        //
        // addressable<slice>
        //
        //----------------------------------------------------------------------
      
        //! access
        inline virtual slice & operator[](const size_t s) throw()
        {
            assert(s>0);
            assert(s<=slices);
            return __slice[s];
        }
        
        //! access, const
        inline virtual const slice & operator[](const size_t s) const throw()
        {
            assert(s>0);
            assert(s<=slices);
            return __slice[s];
        }
        
        //! number of slices
        inline virtual size_t size() const throw()
        {
            return slices;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(tensor3d);
        slice * __slice;
        
        inline void cleanup() throw()
        {
            while(built>0)
            {
                self_destruct( __slice[built--] );
            }
            __slice = 0;
        }
        
        inline void build(row *rowAddr, mutable_type *objAddr)
        {
            try {
                assert(rowAddr);
                assert(objAddr);
                assert(__slice);
                slice *base = __slice+1;
                while(built<slices)
                {
                    new (base+built) slice(rows,cols,rowAddr,objAddr);
                    ++built;
                    rowAddr += rows;
                    objAddr += it2d;
                }
            }
            catch(...)
            {
                cleanup();
                throw;
            }
        }
        
    };
    
}

#endif

