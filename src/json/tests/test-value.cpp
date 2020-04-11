#include "y/json/value.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(value)
{
    {
        JSON::Value  v0;
        JSON::Value  v1 = JSON::NullType;
        JSON::Value  v2 = JSON::TrueType;
        JSON::Value  v3 = JSON::FalseType;
        JSON::Value  v4 = 4.0;
        JSON::Value  v5 = "Hello";
        const string world = "World";
        JSON::Value  v6 = world;

        {
            v0 = JSON::NullType;
            v0 = JSON::TrueType;
            v0 = JSON::FalseType;
            v0 = 1;
            v0 = "Hello";
            v0 = world;
        }
    }

    

}
Y_UTEST_DONE()

