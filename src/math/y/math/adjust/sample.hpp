
//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_INCLUDED 1

#include "y/math/adjust/sample/type.hpp"
#include "y/math/adjust/types.hpp"
#include "y/sort/index.hpp"

namespace upsylon {
    
    namespace Adjust {
        
        template <typename T>
        class Sample : public SampleType<T>, public counted_object
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef arc_ptr<Sample>             Pointer;
            typedef vector< Sample::Pointer>    Handles;
            typedef typename Type<type>::Series Series;
            
            inline explicit Sample(const Series &userAbscissa,
                                   const Series &userOrdinate,
                                   Series       &userAdjusted) throw() :
            abscissa(userAbscissa),
            ordinate(userOrdinate),
            adjusted(userAdjusted)
            {}
            
            inline virtual ~Sample() throw() {}
            
            const Series abscissa;
            const Series ordinate;
            Series       adjusted;
            
            inline virtual size_t count() const throw()
            {
                assert(abscissa->size()==ordinate->size());
                assert(adjusted->size()==abscissa->size());
                return abscissa->size();
            }
            
            inline virtual void ready()
            {
                // memory
                const size_t n = this->count();
                indices.adjust(n,0);
                deltaSq.adjust(n,0);
                
                // indexing
                indexing::make(indices,comparison::increasing<type>,*abscissa);
                std::cerr << "abscissa = " << abscissa << std::endl;
                std::cerr << "indices  = " << indices  << std::endl;
            }
            
        private:
            Indices      indices;
            vector<type> deltaSq;
            
            Y_DISABLE_COPY_AND_ASSIGN(Sample);
        };
        
    }
    
}


#endif

