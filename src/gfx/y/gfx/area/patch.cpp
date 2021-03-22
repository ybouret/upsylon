
#include "y/gfx/area/patch.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace graphic
    {

        namespace crux
        {

            void patch:: throw_empty_patch()
            {
                entity::error("graphic::patch is empty!");
            }

            coord patch:: symmetrical_upper(const unit_t W, const unit_t H)
            {
                checking::gtz(W, "graphic::patch::width"  );
                checking::gtz(H, "graphic::patch::height" );
                return coord( (W-1)/2, (H-1)/2 );
            }

            coord patch:: symmetrical_lower(const unit_t W, const unit_t H)
            {
                const coord up = symmetrical_upper(W,H);
                return coord(1+up.x-W,1+up.y-H);
            }


        }

    }

}


