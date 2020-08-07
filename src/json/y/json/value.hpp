//! \file
#ifndef Y_JSON_VALUE_INCLUDED
#define Y_JSON_VALUE_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/string.hpp"

namespace upsylon
{

    namespace JSON
    {
        typedef double Number; //!< alias
        typedef string String; //!< alias

        //! enumeration of different value types
        enum ValueType
        {
            IsNull,     //!< "null"
            IsTrue,     //!< "true"
            IsFalse,    //!< "false"
            IsNumber,   //!< a number
            IsString,   //!< a string
            IsArray,    //!< an array
            IsObject    //!< an object
        };

        //! text for value type
        const char *ValueTypeText(const ValueType t) throw();
        
        typedef struct int2type<IsNull>   NullType_t;   //!< alias
        extern  const  NullType_t         NullType;     //!< alias
        typedef struct int2type<IsTrue>   TrueType_t;   //!< alias
        extern  const  TrueType_t         TrueType;     //!< alias
        typedef struct int2type<IsFalse>  FalseType_t;  //!< alias
        extern  const  FalseType_t        FalseType;    //!< alias
        typedef struct int2type<IsArray>  ArrayType_t;  //!< alias
        extern  const  ArrayType_t        ArrayType;    //!< alias
        typedef struct int2type<IsObject> ObjectType_t; //!< alias
        extern  const  ObjectType_t       ObjectType;   //!< alias
        
        class Array;
        class Object;

        //! Value as a variant
        class Value
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            Value() throw();                         //!< IsNull
            virtual ~Value() throw();                //!< cleanup
            Value(const Value &other);               //!< hard copy
            Value & operator=( const Value &other ); //!< hard-copy/swap

            Value( const NullType_t  & ) throw(); //!< null
            Value( const TrueType_t  & ) throw(); //!< true
            Value( const FalseType_t & ) throw(); //!< false
            Value( const ArrayType_t & );         //!< empty array
            Value( const ObjectType_t &);         //!< empty object
            Value( const Number        );         //!< make number
            Value( const char   *text  );         //!< make string
            Value( const string &str   );         //!< make string
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            void        swapWith( Value &other ) throw(); //!< no-throw swap
            const char *typeName() const throw();         //!< type name

            template <typename T> T       & as() throw();       //!< data conversion
            template <typename T> const T & as() const throw(); //!< data conversion
            
            //! display
            ios::ostream & display(ios::ostream &, int depth=0) const;
            
            const ValueType type; //!< named type
            
        private:
            void *impl;
        };
        
        typedef vector<Value> _Array; //!< base class for Array
        
        //! array of values
        class Array : public _Array
        {
        public:
            explicit Array() throw();       //!< empty array
            virtual ~Array() throw();       //!< destructor
            explicit Array(const size_t n); //!< array of n null
            void     nil(const size_t n);   //!< create array of n null
            Array(const Array &);           //!< hard copy
            
            inline void   push( const Value &v )  { push_back(v);  }                           //!< push a copy
            inline size_t length() const throw()  { return size(); }                           //!< size
            inline void   pop()    throw()        { pop_back();    }                           //!< remove last
            inline void   pop( Value &v ) throw() { v.swapWith(back()); pop_back(); }         //!< get then pop
            inline void   _push(Value &v)         { push(NullType); back().swapWith(v); }     //!< push content

            inline void   push( const NullType_t  &_) { const Value tmp(_); push_back(tmp); }  //!< push a new null
            inline void   push( const TrueType_t  &_) { const Value tmp(_); push_back(tmp); }  //!< push a new true
            inline void   push( const FalseType_t &_) { const Value tmp(_); push_back(tmp); }  //!< push a new false
            Array        &push( const ArrayType_t  &);                                         //!< push a new empty array
            Object       &push( const ObjectType_t &);                                         //!< push a new empty object
            Number       &push( const Number x);                                               //!< push a new number
            String       &push( const String &);                                               //!< push a new string
            String       &push( const char   *);                                               //!< push a new string

            void display( ios::ostream &os, int depth) const; //!< display

        private:
            Y_DISABLE_ASSIGN(Array);
        };
        
        //! base class for Pair
        class _Pair : public object, public counted
        {
        public:
            typedef intr_ptr<string,_Pair> Pointer; //!< alias
            
            const string label; //!< unique label in object
            Value        value; //!< value
            
            virtual ~_Pair() throw();                      //!< destructor
            explicit _Pair(const string &l,const Value &); //!< setup with hard-copy
            explicit _Pair(const string &l);               //!< create with null
            const string & key() const throw();            //!< label, for set
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(_Pair);
        };
        
        typedef _Pair::Pointer    Pair;   //!< a dynamic Pair
        typedef set<string,Pair> _Object; //!< base class for Object

        //! a database of pairs
        class Object : public _Object
        {
        public:
            explicit Object() throw(); //!< setup
            virtual ~Object() throw(); //!< desctructor
            Object(const Object &);    //!< hard copy
            inline size_t length() const throw() { return size(); } //!< number of pairs
            
            Value        &add(const string &label, const Value &v);                                            //!< add a copy
            inline Value &add(const char   *label, const Value &v) { const string _(label); return add(_,v); } //!< add a copy
           
            inline Value & add(const string &label, const NullType_t  &_) { const Value tmp(_); return add(label,tmp); } //!< add a new null
            inline Value & add(const char   *label, const NullType_t  &_) { const Value tmp(_); return add(label,tmp); } //!< add a new null
            
            inline Value & add(const string &label, const TrueType_t  &_) { const Value tmp(_); return add(label,tmp); } //!< add a new true
            inline Value & add(const char   *label, const TrueType_t  &_) { const Value tmp(_); return add(label,tmp); } //!< add a new true

            inline Value & add(const string &label, const FalseType_t  &_) { const Value tmp(_); return add(label,tmp); } //!< add a new false
            inline Value & add(const char   *label, const FalseType_t  &_) { const Value tmp(_); return add(label,tmp); } //!< add a new false
            
            Array        &add(const string &label, const ArrayType_t   &); //!< add a new array
            inline Array &add(const char   *label, const ArrayType_t &_) { const string __(label); return add(__,_); }//!< add a new array
            
            Object        &add(const string &label, const ObjectType_t  &); //!< add a new object
            inline Object &add(const char   *label, const ObjectType_t  &_) { const string __(label); return add(__,_); } //!< add a new object
            
            Number        &add(const string &label, const Number); //!< add a new number
            inline Number &add(const char *label, const Number __) { const string _(label); return add(_,__); } //!< add a new number

            String        &add(const string &label, const String&); //!< add a new string
            inline String &add(const char   *label, const String &__) { const string _(label); return add(_,__); } //!< add a new string

            String        &add(const string &label, const char  *); //!< add a new string
            inline String &add(const char   *label, const char *__) { const string _(label); return add(_,__); } //!< add a new string

            //! display
            void display( ios::ostream &os, int depth) const;

        private:
            Y_DISABLE_ASSIGN(Object);
        };
        
        
    }

}

#endif

