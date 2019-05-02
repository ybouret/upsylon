
//! \file
#ifndef Y_MATH_FIT_LS_RESCALE_INCLUDED
#define Y_MATH_FIT_LS_RESCALE_INCLUDED 1

#include "y/math/fit/ls.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            class RescalerInfo
            {
            public:
                virtual ~RescalerInfo() throw();
                explicit RescalerInfo() throw();

                static const char FunctionCoeff[]; //!< "FunctionCoeff";
                static const char VariableScale[]; //!< "VariableScale"
                static const char VariableShift[]; //!< "VariableShift";

                void declare( Variables &vars );
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(RescalerInfo);
            };

            template <typename FUNCTION>
            class Rescaler : public RescalerInfo
            {
            public:
                FUNCTION F; //!< use copy constructor of a 1D function

                inline explicit Rescaler( const FUNCTION &f1d ) :
                RescalerInfo(), F(f1d)
                {

                }

                inline virtual ~Rescaler() throw()
                {
                }

                template <typename T>
                inline T Compute( T value, const array<T> &aorg, const Variables &vars )
                {
                    const T scale = vars(aorg,VariableScale);
                    const T shift = vars(aorg,VariableShift);
                    const T coeff = vars(aorg,FunctionCoeff);
                    return  coeff * F( scale*(value-shift) );
                }

                template <typename T>
                inline void initialize(array<T> &aorg, const Variables &vars )
                {
                    vars(aorg,FunctionCoeff) = 1;
                    vars(aorg,VariableScale) = 1;
                    vars(aorg,VariableShift) = 0;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rescaler);
            };

        }

    }

}

#endif

