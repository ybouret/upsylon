#ifndef Y_JSON_VALUE_INCLUDED
#define Y_JSON_VALUE_INCLUDED 1

#include "y/ios/ostream.hpp"

namespace upsylon
{

    namespace JSON
    {
        typedef double Number;
        typedef string String;

        enum ValueType
        {
            IsNull,
            IsTrue,
            IsFalse,
            IsNumber,
            IsString
        };

        typedef struct int2type<IsNull>  NullType_t;  extern const NullType_t  NullType;
        typedef struct int2type<IsTrue>  TrueType_t;  extern const TrueType_t  TrueType;
        typedef struct int2type<IsFalse> FalseType_t; extern const FalseType_t FalseType;


        class Value
        {
        public:
            Value() throw(); //!< IsNull
            virtual ~Value() throw();
            Value(const Value &other);
            Value & operator=( const Value &other );
            size_t size() const throw();

            Value( const NullType_t  & ) throw();
            Value( const TrueType_t  & ) throw();
            Value( const FalseType_t & ) throw();
            Value( const double        );
            Value( const char   *text  );
            Value( const string &str   );
            
            const ValueType type;
            void  swap_with( Value &other ) throw();

        private:
            void *impl;
        };
    }

}

#endif

