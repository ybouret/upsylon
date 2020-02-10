#include "y/string/esma.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core {

        unit_t esma :: check_i(const unit_t i, const unit_t m)
        {
            if(i>=m) throw exception("esma::check_i failure");
            return i;
        }

        unit_t esma :: check_j(const unit_t j, const unit_t m)
        {
            if(j>=m) throw exception("esma::check_j failure");
            return j;
        }
        
    }
}
