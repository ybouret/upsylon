//! \file

#ifndef Y_GFX_TYPES_INCLUDED
#define Y_GFX_TYPES_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/core/zero-flux-index.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    namespace GFX
    {

        typedef point2d<unit_t>          Point;
        typedef core::zero_flux_index<0> ZeroFlux;

        class Area
        {
        public:
            const unit_t x;
            const unit_t y;
            const unit_t w;
            const unit_t h;
            const unit_t n;

            Area(const unit_t W, const unit_t H); //!< W>=0, H>=0
            Area(const unit_t X, const unit_t Y, const unit_t W, const unit_t H);
            Area(const Area &) throw();
            virtual ~Area() throw();

        private:
            Y_DISABLE_ASSIGN(Area);
        };

        class Stretch
        {
        public:
            Stretch(const Area   &area,
                    const size_t  size,
                    const size_t  rank) throw();
            ~Stretch() throw();
            Stretch(const Stretch &) throw();

            const unit_t work;
            const Point  begin;
            const Point  end;

        private:
            Y_DISABLE_ASSIGN(Stretch);

        };


        class Manager : public singleton<Manager>
        {
        public:
            Y_SINGLETON_DECL_WITH(0,Manager);

        private:
            virtual ~Manager() throw();
            explicit Manager();
            Y_DISABLE_COPY_AND_ASSIGN(Manager);
        };

    }
}

#endif
