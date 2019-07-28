#include "y/sparse/dok.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace sparse
    {
        dok_base:: ~dok_base() throw()
        {
        }

        dok_base:: dok_base() throw()
        {

        }

        void dok_base:: throw_bad_copy() const
        {
            throw exception("unexpected failure in dictionary of keys copy!");
        }
    }
}
