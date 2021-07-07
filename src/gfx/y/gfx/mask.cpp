
#include "y/gfx/mask.hpp"

namespace upsylon
{

    namespace graphic
    {

        mask:: mask() : mask_() {}

        mask:: ~mask() throw()
        {
        }

        void mask:: insert(const coord p)
        {
            const coord    tmp = coord_to_key(p);
            const uint8_t *key = (const uint8_t *)&tmp;
            (void) insert_by(key, sizeof(coord), p);
        }

        coord mask:: coord_to_key(const coord &p) throw()
        {
            return coord( swap_be_as(p.x), swap_be_as(p.y) );
        }

        mask & mask:: operator += (const coord p)
        {
            insert(p);
            return *this;
        }

        mask & mask:: operator += (const mask &other)
        {
            if(this!=&other)
            {
                merge(other);
            }
            return *this;
        }

        area mask:: aabb() const throw()
        {
            if(size()<=0)
            {
                return area(0,0);
            }
            else
            {
                const mask::data_node *node = head(); assert(node);
                coord lower = **node;
                coord upper = **node;
                for(node=node->next;node;node=node->next)
                {
                    const coord tmp = **node;
                    lower = coord::min_of(lower,tmp);
                    upper = coord::max_of(upper,tmp);
                }
                upper-=lower;
                return area(upper.x+1,upper.y+1,lower.x,lower.y);
            }
        }

    }

}
