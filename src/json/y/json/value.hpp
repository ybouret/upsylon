#ifndef Y_JSON_VALUE_INCLUDED
#define Y_JSON_VALUE_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{

    namespace JSON
    {
        typedef double Number; //!< alias
        typedef string String; //!< alias

        //! enumeration of different value types
        enum ValueType
        {
            IsNull,
            IsTrue,
            IsFalse,
            IsNumber,
            IsString,
            IsArray,
            IsObject
        };

        const char *ValueTypeText(const ValueType t) throw();
        
        typedef struct int2type<IsNull>   NullType_t;
        extern  const  NullType_t         NullType;
        typedef struct int2type<IsTrue>   TrueType_t;
        extern  const  TrueType_t         TrueType;
        typedef struct int2type<IsFalse>  FalseType_t;
        extern  const  FalseType_t        FalseType;
        typedef struct int2type<IsArray>  ArrayType_t;
        extern  const  ArrayType_t        ArrayType;
        typedef struct int2type<IsObject> ObjectType_t;
        extern  const  ObjectType_t       ObjectType;
        
        class Array;
        class Object;
        
        class Value
        {
        public:
            Value() throw(); //!< IsNull
            virtual ~Value() throw();
            Value(const Value &other);
            Value & operator=( const Value &other );
            size_t length() const throw();

            Value( const NullType_t  & ) throw();
            Value( const TrueType_t  & ) throw();
            Value( const FalseType_t & ) throw();
            Value( const ArrayType_t & );
            Value( const ObjectType_t &);
            Value( const Number        );
            Value( const char   *text  );
            Value( const string &str   );
            
            const ValueType type;
            void  swap_with( Value &other ) throw();
            
            template <typename T> T       & as() throw();
            template <typename T> const T & as() const throw();

            inline const char *typeName() const throw() { return ValueTypeText(type); }
            
            void display(std::ostream &os, int depth=0) const;
            
            inline friend std::ostream & operator<<( std::ostream &os, const Value v)
            {
                v.display(os,0);
                return os;
            }
            
        private:
            void *impl;
        };
        
        typedef vector<Value> _Array; //!< base class for Array
        
        //! array of values
        class Array : public _Array
        {
        public:
            explicit Array() throw();
            virtual ~Array() throw();
            Array(const Array &);
            
            inline void   push( const Value &v ) { push_back(v);  }
            inline size_t length() const throw() { return size(); }
            inline void   pop()    throw()       { pop_back();    }
            
            inline void   push( const NullType_t  &_) { const Value tmp(_); push_back(tmp); }
            inline void   push( const TrueType_t  &_) { const Value tmp(_); push_back(tmp); }
            inline void   push( const FalseType_t &_) { const Value tmp(_); push_back(tmp); }
            Array  &push( const ArrayType_t  &);
            Object &push( const ObjectType_t &);
            Number &push( const Number x);
            String &push( const String &);
            String &push( const char   *);
            
        private:
            Y_DISABLE_ASSIGN(Array);
        };
        
        
        class _Pair : public counted_object
        {
        public:
            typedef intr_ptr<string,_Pair> Pointer;
            
            const string label;
            Value        value;
            
            virtual ~_Pair() throw();
            explicit _Pair(const string &l,const Value &);
            explicit _Pair(const string &l);
            const string & key() const throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(_Pair);
        };
        
        typedef _Pair::Pointer    Pair;
        typedef set<string,Pair> _Object;
        
        class Object : public _Object
        {
        public:
            explicit Object() throw();
            virtual ~Object() throw();
            Object(const Object &);
            inline size_t length() const throw() { return size(); }
            
            Value        &add(const string &label, const Value &v);
            inline Value &add(const char   *label, const Value &v) { const string _(label); return add(_,v); }
           
            inline Value & add(const string &label, const NullType_t  &_) { const Value tmp(_); return add(label,tmp); }
            inline Value & add(const char   *label, const NullType_t  &_) { const Value tmp(_); return add(label,tmp); }
            
            inline Value & add(const string &label, const TrueType_t  &_) { const Value tmp(_); return add(label,tmp); }
            inline Value & add(const char   *label, const TrueType_t  &_) { const Value tmp(_); return add(label,tmp); }

            inline Value & add(const string &label, const FalseType_t  &_) { const Value tmp(_); return add(label,tmp); }
            inline Value & add(const char   *label, const FalseType_t  &_) { const Value tmp(_); return add(label,tmp); }
            
            Array        &add(const string &label, const ArrayType_t   &);
            inline Array &add(const char   *label, const ArrayType_t &_) { const string __(label); return add(__,_); }
            
            Object        &add(const string &label, const ObjectType_t  &);
            inline Object &add(const char   *label, const ObjectType_t  &_) { const string __(label); return add(__,_); }
            
            Number &add(const string &label, const Number);
            String &add(const string &label, const String&);
            String &add(const string &label, const char  *);

        private:
            Y_DISABLE_ASSIGN(Object);
        };
        
        
    }

}

#endif

