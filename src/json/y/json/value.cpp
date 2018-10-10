#include "y/json/value.hpp"


namespace upsylon
{
    namespace JSON
    {
        const NullType_t  NullType  = {};
        const TrueType_t  TrueType  = {};
        const FalseType_t FalseType = {};

        Value:: Value() throw() :
        type( IsNull ),
        impl( NULL )
        {

        }

        Value:: Value( const NullType_t & ) throw() :
        type( IsNull ),
        impl( NULL )
        {
        }

        Value:: Value( const TrueType_t & ) throw() :
        type( IsTrue ),
        impl( NULL   )
        {
        }

        Value:: Value( const FalseType_t & ) throw() :
        type( IsFalse ),
        impl( NULL    )
        {
        }

        Value:: Value( const Number x ) :
        type( IsNumber ),
        impl( object::acquire1<Number>() )
        {
            *(Number *)impl = x;
        }

        Value:: Value( const char   *text  ) :
        type( IsString ),
        impl( new string(text) )
        {

        }

        Value:: Value( const string &str  ) :
        type( IsString ),
        impl( new string(str) )
        {

        }


        Value:: Value( const Value &other ) :
        type( other.type ),
        impl( NULL )
        {
            switch(type)
            {
                case IsNull:   break;
                case IsTrue:   break;
                case IsFalse:  break;
                case IsNumber: *(Number *)(impl=object::acquire1<Number>()) = *(const Number *)other.impl; break;
                case IsString: impl = new String( *(const String *) other.impl );  break;
            }
        }

        Value:: ~Value() throw()
        {
            switch(type)
            {
                case IsNull:  break;
                case IsTrue:  break;
                case IsFalse: break;
                case IsNumber: assert(impl); object::release1<Number>( *(Number **)&impl ); break;
                case IsString: assert(impl); delete static_cast<String *>(impl); break;
            }
        }

        void Value:: swap_with(Value &other) throw()
        {
            cswap(type,other.type);
            cswap(impl,other.impl);
        }

        Value & Value:: operator=(const Value &other)
        {
            Value tmp(other);
            swap_with(tmp);
            return *this;
        }

    }
}
