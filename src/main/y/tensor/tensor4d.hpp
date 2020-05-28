
//! file

#ifndef Y_TENSOR4D_INCLUDED
#define Y_TENSOR4D_INCLUDED 1

#include "y/tensor/tensor3d.hpp"

namespace upsylon {
    
    namespace core
    {
        //----------------------------------------------------------------------
        //
        //! common stuff for tensor4d
        //
        //----------------------------------------------------------------------
        class tensor4d : public tensor3d
        {
        public:
            virtual ~tensor4d() throw(); //!< cleanup
            const size_t frames;         //!< number of frames
            const size_t items_per_view; //!< items_per_frame*frames = all items
            
        protected:
            //! setup
            explicit tensor4d(const size_t r, const size_t c, const size_t s, const size_t f);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tensor4d);
        };
    }
 
    //--------------------------------------------------------------------------
    //
    //! tensor3d
    //
    //--------------------------------------------------------------------------
    template <typename T>
    class tensor4d : public core::tensor4d,
    public addressable< upsylon::tensor3d<T> >
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
        typedef upsylon::tensor3d<T> frame;  //!< alias

        //----------------------------------------------------------------------
        //
        // C++
        //
        //----------------------------------------------------------------------
        
        //! cleanup
        inline virtual ~tensor4d() throw() { cleanup(); }
        
        //! setup with private data
        inline explicit tensor4d(const size_t r,
                                 const size_t c,
                                 const size_t s,
                                 const size_t f) :
        core::tensor4d(r,c,s,f),
        __frame(0)
        {
            slice        *slcAddr = 0;
            row          *rowAddr = 0;
            mutable_type *objAddr = 0;
            memory::embed emb[] = {
                memory::embed::as(__frame,frames),
                memory::embed::as(slcAddr,slices*frames),
                memory::embed::as(rowAddr,slices*rows*frames),
                memory::embed::as(objAddr,items_per_view)
            };
            create(emb,sizeof(emb)/sizeof(emb[0]));
            __frame -= 1;
            build(slcAddr,rowAddr,objAddr);
        }
        
        //----------------------------------------------------------------------
        //
        // addressable<slice>
        //
        //----------------------------------------------------------------------
        
        //! access
        inline virtual frame & operator[](const size_t f) throw()
        {
            assert(f>0);
            assert(f<=frames);
            return __frame[f];
        }
        
        //! access, const
        inline virtual const frame & operator[](const size_t f) const throw()
        {
            assert(f>0);
            assert(f<=frames);
            return __frame[f];
        }
        
        //! number of slices
        inline virtual size_t size() const throw()
        {
            return frames;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(tensor4d);
        
        frame * __frame;
        
        inline void cleanup() throw()
        {
            while(built>0)
            {
                self_destruct( __frame[built--] );
            }
            __frame = 0;
        }
        
        inline void build(slice * &slcAddr, row * &rowAddr, mutable_type * &objAddr)
        {
            try {
                assert(rowAddr);
                assert(objAddr);
                assert(__frame);
                frame *base = __frame+1;
                while(built<frames)
                {
                    new (base+built) frame(rows,cols,slices,slcAddr,rowAddr,objAddr);
                    ++built;
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
