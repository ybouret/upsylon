//! \file

#ifndef Y_GFX_OPS_BLOBS_INCLUDED
#define Y_GFX_OPS_BLOBS_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"
#include "y/core/knode.hpp"
#include "y/core/inode.hpp"

namespace upsylon
{
    namespace graphic
    {

        typedef core::knode<coord>    knot;
        typedef knot::list_type       knots_;

        class knots : public entity, public knots_
        {
        public:
            virtual ~knots() throw();
            explicit knots() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(knots);
        };

        typedef arc_ptr<knots> shared_knots;



        class blob : public knots, public inode<blob>
        {
        public:
            explicit blob(const size_t        id,
                          const shared_knots &ks) throw();
            virtual ~blob() throw();


            const size_t label;
            knots        cache;
            shared_knots kpool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(blob);
        };

        typedef core::list_of_cpp<blob> blobs_;

        class blobs : public pixmap<size_t>, public blobs_
        {
        public:
            explicit blobs(const unit_t        W,
                           const unit_t        H,
                           const shared_knots &K);
            virtual ~blobs() throw();



            template <typename T> inline
            static size_t knots_for(const tile &t, const pixmap<T> &pxm) throw()
            {
                size_t res = 0;
                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    const pixrow<T> &r    = pxm[s.y];
                    const unit_t     xmin = s.xmin;
                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        if( !pixel::is_zero(r(x)) )  ++res;
                    }
                }
                return res;
            }

            void initialize(size_t num_knots);

            knots        cache;
            shared_knots kpool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(blobs);

        };


    }


}

#endif

