
//! \file

#ifndef Y_GFX_FILTERS_INCLUDED
#define Y_GFX_FILTERS_INCLUDED 1

#include "y/gfx/filter.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        typedef filter<float>              filter_type;
        typedef arc_ptr<const filter_type> shared_filter;
        
        class filters : public entity
        {
        public:
            virtual ~filters() throw();
            
            const string        name;
            const shared_filter y;
            const shared_filter x;
            
        protected:
            template <typename ID, typename U>
            inline explicit filters(const ID    &ident,
                                    const U     *coeff,
                                    const unit_t width) :
            name(ident),
            y( new filter_type(name,coeff,width,false, "_y") ),
            x( new filter_type(name,coeff,width,true,  "_x") )
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(filters);
        };
        
        typedef arc_ptr<const filters> shared_filters;
        
    }
    
}

#define Y_FILTERS_DECL(NAME,DIM)           \
/**/ class NAME##DIM : public filters {    \
/**/ public:                               \
/**/ static const char ID[];               \
/**/ explicit NAME##DIM();                 \
/**/ virtual ~NAME##DIM() throw();         \
/**/ private:                              \
/**/ Y_DISABLE_COPY_AND_ASSIGN(NAME##DIM); \
/**/ static const int Coeffs[DIM][DIM];    \
/**/ }

#define Y_FILTERS_IMPL(NAME,DIM)                                 \
/**/ const char   NAME##DIM :: ID[] = #NAME #DIM;                \
/**/ NAME##DIM::  NAME##DIM() : filters(ID,&Coeffs[0][0],DIM) {} \
/**/ NAME##DIM:: ~NAME##DIM() throw() {} \
/**/ const int NAME##DIM :: Coeffs[DIM][DIM] = {

#define Y_FILTERS_DONE() }

#endif

