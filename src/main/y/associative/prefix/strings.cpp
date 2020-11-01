#include "y/associative/prefix/strings.hpp"
#include "y/string.hpp"

namespace upsylon
{
    
    prefix_strings:: prefix_strings() : depot_type()
    {
    }
    
    prefix_strings:: ~prefix_strings() throw()
    {
    }
    
    prefix_strings:: prefix_strings( const prefix_strings & other) :
    depot_type(other)
    {
    }
    
    prefix_strings & prefix_strings:: operator=(const prefix_strings &other)
    {
        {
            depot_type &self = *this;
            self = other;
        }
        return *this;
    }
    
    bool prefix_strings:: insert(const char   *s)
    {
        return depot_type::insert(s,s?strlen(s):0);
    }

    bool prefix_strings:: insert(const string &s)
    {
        return depot_type::insert(s);
    }
    
}

