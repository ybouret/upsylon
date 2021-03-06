
//! \file

#ifndef Y_GFX_PATCH_INCLUDED
#define Y_GFX_PATCH_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/memory/embed.hpp"
#include "y/os/static-check.hpp"
#include <iomanip>
#include <cstring>

namespace upsylon
{
    
    namespace graphic
    {
        
        namespace crux
        {
            //__________________________________________________________________
            //
            //! common ops for patch
            //__________________________________________________________________
            struct patch
            {
                static void   throw_empty_patch();                                //!< throw if patch has no item
                static coord  symmetrical_lower(const unit_t W, const unit_t H);  //!< convert
                static coord  symmetrical_upper(const unit_t W, const unit_t H);  //!< convert
                static unit_t symmetrical_upper(const unit_t L, const char   *);  //!< convert
                static unit_t symmetrical_lower(const unit_t L, const char   *);  //!< convert
                
                typedef int   (*compare)(const void *, const void*);              //!< compare patch types
                static void   sort_data(void*,size_t,size_t,compare) throw();     //!< call sorting algo
            };
        }
        
        //! access macro
#define Y_GFX_PATCH_ROW(x) assert(x>=from.lower.x); assert(x<=from.upper.x); return addr[x]
        
        //______________________________________________________________________
        //
        //
        //! pre-computed row for a patch of data
        //
        //______________________________________________________________________
        template <typename T>
        class patch_row
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            inline patch_row(mutable_type *p,const area &a)      throw() : addr(p), from(a)  { } //!< setup
            inline type       & operator[](const unit_t x)       throw() { Y_GFX_PATCH_ROW(x); } //!< access
            inline const_type & operator[](const unit_t x) const throw() { Y_GFX_PATCH_ROW(x); } //!< access, const
            
        private:
            mutable_type  *addr;
            const area    &from;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(patch_row);
            inline ~patch_row() throw() {}
        };
        
        
        //! access macro
#define Y_GFX_PATCH_GET(y) assert(y<=upper.y);assert(y>=lower.y); return rows[y]
        
        //______________________________________________________________________
        //
        //
        //! patch of data
        //
        //______________________________________________________________________
        template <typename T>
        class patch : public entity, public area
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);       //!< aliases
            typedef patch_row<T> row;  //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! helper for constructor
#define Y_GFX_PATCH_CTOR_() rows(0), wksp(0), wlen(0), room(0)
            
            //! setup
            inline  patch(const unit_t W,
                          const unit_t H,
                          const unit_t X,
                          const unit_t Y) :
            entity(), area(W,H,X,Y), Y_GFX_PATCH_CTOR_()
            {
                initialize();
            }
            
            //! setup symmetrical
            inline  patch(const unit_t W,
                          const unit_t H) :
            entity(),
            area(W,H,
                 crux::patch::symmetrical_lower(W,checking::width ),
                 crux::patch::symmetrical_lower(H,checking::height) ),
            Y_GFX_PATCH_CTOR_()
            {
                initialize();
            }
            //! cleanup
            virtual ~patch() throw()
            {
                suppress(wksp,wlen);
                rows=0;
                aliasing::_(room) = 0;
            }
            
            //! copy
            inline patch(const patch &other) :
            entity(), area(other), Y_GFX_PATCH_CTOR_()
            {
                initialize();
                memcpy(wksp,other.wksp,room);
            }
            
            //! copy/transpose
            inline patch(const patch &other, const area::transpose_t &_) :
            entity(), area(other,_), Y_GFX_PATCH_CTOR_()
            {
                initialize();
                for(unit_t y=lower.y;y<=upper.y;++y)
                {
                    row &r=(*this)[y];
                    for(unit_t x=lower.x;x<=upper.x;++x)
                    {
                        r[x] = other[x][y];
                    }
                }
            }
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! access data entry
            inline const_type * operator*() const throw() { return static_cast<const_type *>(wksp); }
            
            //! info
            inline size_t allocated() const throw() { return wlen; }
            
            //! row access
            inline row       & operator[](const unit_t y)       throw() { Y_GFX_PATCH_GET(y); }
            
            //! row access, const
            inline const row & operator[](const unit_t y) const throw() { Y_GFX_PATCH_GET(y); }
            
            //! display
            inline std::ostream & display(std::ostream &os) const
            {
                for(unit_t y=upper.y;y>=lower.y;--y)
                {
                    os << "@y=" << std::setw(4) << y << " :";
                    for(unit_t x=lower.x;x<=upper.x;++x)
                    {
                        os << " [" << std::setw(4) << x << "]=" << std::setw(4) << (*this)[y][x];
                    }
                    os << std::endl;
                }
                return os;
            }
            
            //! sum
            inline const_type sum() const throw()
            {
                return sum_of(**this,items);
            }
            
            //! sorted sum using user's memory
            inline const_type sum(mutable_type *tmp) const throw()
            {
                assert(tmp);
                memcpy(tmp,wksp,room);
                crux::patch::sort_data(tmp,items,sizeof(T),cmp);
                return sum_of(tmp,items);
            }
            
            //! load  per-channel accumulation
            template <
            typename       PIXEL,
            typename       U,
            const unsigned NCH
            >
            inline void ld(mutable_type        *acc,
                           const pixmap<PIXEL> &pxm,
                           const coord          pos) const throw()
            {
                Y_STATIC_CHECK(sizeof(PIXEL)>=NCH*sizeof(U),invalid_pixel);
                static const unsigned zlen = NCH*sizeof(T);
               
                memset(acc,0,zlen);
                const unit_t xmax   = upper.x;
                const unit_t xmin   = lower.x;
                const unit_t ymin   = lower.y;
                for(unit_t y=upper.y;y>=ymin;--y)
                {
                    const pixrow<PIXEL> &src = pxm[pos.y+y];
                    const patch_row<T>  &cof = (*this)[y];
                    
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        const U    *u = (const U *) &src[pos.x+x];
                        const_type  t = cof[x];
                        
                        for(size_t ch=0;ch<NCH;++ch)
                        {
                            acc[ch] += t * T(u[ch]);
                        }
                    }
                }
            }

            
            
        private:
            Y_DISABLE_ASSIGN(patch);
            row         *rows;
            void        *wksp;
            size_t       wlen;
            const size_t room;
            
            inline void initialize()
            {
                if(items<=0) crux::patch::throw_empty_patch();
                mutable_type *data = 0;
                {
                    memory::embed emb[] =
                    {
                        memory::embed::as(data,items),
                        memory::embed::as(rows,h)
                    };
                    wksp = allocate(emb, sizeof(emb)/sizeof(emb[0]), wlen);
                    assert(wksp==data);
                    aliasing::_(room) = items * sizeof(T);
                }
                rows -= lower.y;
                data -= lower.x;
                for(unit_t y=lower.y;y<=upper.y;++y,data+=w)
                {
                    new (rows+y) row(data,*this);
                }
            }
            
            static inline const_type sum_of(const_type *arr, size_t num)  throw()
            {
                assert(NULL!=arr);
                mutable_type res(0);
                while(num-- > 0)
                    res += *(arr++);
                return res;
            }
            
            static inline int cmp(const void *lhs, const void *rhs) throw()
            {
                assert(lhs);
                assert(rhs);
                const_type L = *static_cast<const_type *>(lhs);
                const_type R = *static_cast<const_type *>(rhs);
                return (L<R) ? -1 : ( (R<L) ? 1 : 0 );
            }
        };
        
    }
}

#endif


