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

        typedef point2d<unit_t>          Point;        //!< logical coordinate
        typedef core::zero_flux_index<0> ZeroFlux;     //!< for operators


        //! internal checking
        struct Check
        {
            static unit_t GTZ(const unit_t  w, const char *id, const char *fn); //!< raise exception if w<=0
            static unit_t GEQZ(const unit_t w, const char *id, const char *fn); //!< raise exception if w<0
            static const char Width[];  //!< "width"
            static const char Height[]; //!< "height"
        };

        struct Position
        {
            static const unsigned Core   = 0x00;
            static const unsigned Top    = 0x01;
            static const unsigned Bottom = 0x02;
            static const unsigned Left   = 0x04;
            static const unsigned Right  = 0x08;
            static const char *   Text(const unsigned) throw();
        };
        


#if 0
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
#endif
        
    }
}

#endif
