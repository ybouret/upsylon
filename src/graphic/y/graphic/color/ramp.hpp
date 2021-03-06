
//! \file
#ifndef Y_GRAPHIC_COLOR_RAMP_INCLUDED
#define Y_GRAPHIC_COLOR_RAMP_INCLUDED 1

#include "y/graphic/color/data2rgba.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
    
    namespace Graphic {
        
        //! color ramp interface
        class Ramp  : public Object
        {
        public:
            typedef arc_ptr<Ramp> Pointer; //!< alias

            virtual ~Ramp() throw();       //!< cleanup
            
            //! get the matching color
            rgb operator()(const float v, const float vmin=0.0f, const float vmax=1.0f);
            
        protected:
            explicit Ramp() throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ramp);
            virtual const rgb *_256colors() const throw() = 0;
        };

        //! base class to wrap ramp + Data2RGBA
        class ColorRamp_ : public Data2RGBA
        {
        public:
            Ramp::Pointer ramp; //!< shared ramp
            const float   vmin; //!< minimum value
            const float   vmax; //!< maximum value
            
            virtual ~ColorRamp_() throw(); //!< cleanup

            //! setup
            explicit ColorRamp_(const Ramp::Pointer &_ramp,
                                const float          _vmin,
                                const float          _vmax) throw();

            //! update range
            void setRange(const float _vmin, const float _vmax) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ColorRamp_);
        };

        //! use default conversion to make a typed color ramp
        template <typename T>
        class ColorRamp : public ColorRamp_
        {
        public:
            //! setup
            inline explicit ColorRamp(const Ramp::Pointer &_ramp,
                                      const float          _vmin=0,
                                      const float          _vmax=1) throw() :
            ColorRamp_(_ramp,_vmin,_vmax)
            {
            }

            //! cleanup
            inline virtual ~ColorRamp() throw() {}
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ColorRamp);
            
            inline virtual rgba get(const void *addr) throw()
            {
                const float v   = Convert::Get<float,T>( *static_cast<const T*>(addr) );
                Ramp       &rmp = *ramp;
                return      rmp(v,vmin,vmax);
            }
        };
        
    }
    
}


#endif


