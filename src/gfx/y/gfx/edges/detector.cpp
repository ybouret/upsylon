
#include "y/gfx/edges/detector.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            detector:: ~detector() throw()
            {
            }

            detector:: detector(const unit_t W, const unit_t H) :
            grad(W,H),
            gmax(0),
            kmax(W,H),
            prof(),
            temp(W,H)
            {

            }


            void detector:: build(const pixmap<float>  &field,
                                  broker               &apply,
                                  const shared_filters &delta,
                                  const blur           *cloud)
            {
                //--------------------------------------------------------------
                //
                // optional blur then gradient
                //
                //--------------------------------------------------------------
                if(cloud)
                {
                    cloud->cover(temp,apply,field);
                    gmax = grad.compute(temp,apply,delta);
                }
                else
                {
                    gmax = grad.compute(field,apply,delta);
                }

                //--------------------------------------------------------------
                //
                // keep maxima
                //
                //--------------------------------------------------------------
                kmax(apply,grad);

                //--------------------------------------------------------------
                //
                // find threshold
                //
                //--------------------------------------------------------------
                const uint8_t strong = kmax.threshold();
                const uint8_t feeble = strong/2;
                prof.tighten(kmax,apply,feeble,strong);
                
            }

        }

    }

}


