
//! \file

#ifndef Y_GFX_PATCH_INCLUDED
#define Y_GFX_PATCH_INCLUDED 1

#include "y/gfx/area.hpp"
#include "y/gfx/entity.hpp"
#include "y/memory/embed.hpp"

namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! row for a patch of data
        //
        //______________________________________________________________________
        template <typename T>
        class patch_row
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases

            //! setup
            inline patch_row(mutable_type *p,const area &a) throw() : addr(p), from(a) { }

            //! access
            type       & operator[](const unit_t x) throw()       { assert(x>=from.lower.x); assert(x<=from.upper.x); return addr[x]; }

            //! access, const
            const_type & operator[](const unit_t x) const throw() { assert(x>=from.lower.x); assert(x<=from.upper.x); return addr[x]; }

        private:
            mutable_type  *addr;

        public:
            const area    &from; //!< original area

        private:
            Y_DISABLE_COPY_AND_ASSIGN(patch_row);
            inline ~patch_row() throw() {}

        };

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


            //! setup
            inline  patch(const unit_t W,
                          const unit_t H,
                          const unit_t X,
                          const unit_t Y) :
            entity(),
            area(W,H,X,Y), rows(0), wksp(0), wlen(0)
            {
                if(items<=0) error("empty graphic::patch!");
                initialize();
            }

            //! cleanup
            virtual ~patch() throw()
            {
                suppress(wksp,wlen);
                rows=0;
            }

            //! copy
            inline patch(const patch &other) :
            entity(),
            area(other),
            rows(0), wksp(0), wlen(0)
            {
                initialize();
                memcpy(wksp,other.wksp,items*sizeof(T));
            }

            //! copy/transpose
            inline patch(const patch &other, const area::transpose_t &_) :
            entity(),
            area(other,_),
            rows(0), wksp(0), wlen(0)
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

            //! info
            inline size_t allocated() const throw() { return wlen; }

            //! row access
            row       & operator[](const unit_t y)       throw() { assert(y<=upper.y);assert(y>=lower.y); return rows[y]; }

            //! row access, const
            const row & operator[](const unit_t y) const throw() { assert(y<=upper.y);assert(y>=lower.y); return rows[y]; }

        private:
            Y_DISABLE_ASSIGN(patch);
            row   *rows;
            void  *wksp;
            size_t wlen;

            inline void initialize()
            {
                mutable_type *data = 0;
                {
                    memory::embed emb[] =
                    {
                        memory::embed::as(data,items),
                        memory::embed::as(rows,h)
                    };
                    wksp = allocate(emb, sizeof(emb)/sizeof(emb[0]), wlen);
                    assert(wksp==data);
                }
                rows -= lower.y;
                data -= lower.x;
                for(unit_t y=lower.y;y<=upper.y;++y,data+=w)
                {
                    new (rows+y) row(data,*this);
                }
            }

        };

    }
}

#endif


