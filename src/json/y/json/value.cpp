#include "y/json/value.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace JSON
    {
        
#define Y_JSON_VALUE(TYPE) case Is##TYPE : return #TYPE
        
        const char *ValueTypeText(const ValueType t) throw()
        {
            switch(t)
            {
                    Y_JSON_VALUE(Null);
                    Y_JSON_VALUE(False);
                    Y_JSON_VALUE(True);
                    Y_JSON_VALUE(Number);
                    Y_JSON_VALUE(String);
                    Y_JSON_VALUE(Array);
                    Y_JSON_VALUE(Object);
            }
            return "???";
        }
        
        
        const char * Value:: typeName() const throw() { return ValueTypeText(type); }
        
        const NullType_t   NullType   = {};
        const TrueType_t   TrueType   = {};
        const FalseType_t  FalseType  = {};
        const ArrayType_t  ArrayType  = {};
        const ObjectType_t ObjectType = {};

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
        
        Value:: Value( const ArrayType_t & ) :
        type( IsArray ),
        impl( new Array() )
        {
        }
        
        Value:: Value( const ObjectType_t &) :
        type( IsObject ),
        impl( new Object() )
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
                case IsString: impl = new String( *(const String  *)other.impl );  break;
                case IsArray:  impl = new Array(  *(const Array   *)other.impl );  break;
                case IsObject: impl = new Object( *(const Object  *)other.impl );  break;
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
                case IsArray:  assert(impl); delete static_cast<Array  *>(impl); break;
                case IsObject: assert(impl); delete static_cast<Object *>(impl); break;
            }
            impl = 0;
        }
        
       

        void Value:: swapWith(Value &other) throw()
        {
            _cswap(type,other.type);
            _cswap(impl,other.impl);
        }

        Value & Value:: operator=(const Value &other)
        {
            Value tmp(other);
            swapWith(tmp);
            return *this;
        }

#define Y_JSON_VALUE_AS(TYPE) \
template <> TYPE       & Value::as<TYPE>() throw()       { assert(Is##TYPE==type); return *static_cast<TYPE*>(impl); } \
template <> const TYPE & Value::as<TYPE>() const throw() { assert(Is##TYPE==type); return *static_cast<const TYPE*>(impl); }

        Y_JSON_VALUE_AS(Number)
        Y_JSON_VALUE_AS(Array)
        Y_JSON_VALUE_AS(Object)
        Y_JSON_VALUE_AS(String)
        
        Array::  Array() throw() : _Array() {}
        Array:: ~Array() throw() {}
        
        Array:: Array(const Array &other) :  collection(), addressable<Value>(), counted_object(), _Array(other) {}

        Array:: Array(const size_t n) : collection(), _Array(n,as_capacity)
        {
            nil(n);
        }

        void Array:: nil(const size_t n)
        {
            const Value tmp;
            make(n,tmp);
        }

        Array  & Array:: push( const ArrayType_t  &_) { const Value tmp(_); push_back(tmp); return back().as<Array>();  }
        Object & Array:: push( const ObjectType_t &_) { const Value tmp(_); push_back(tmp); return back().as<Object>(); }
        Number & Array:: push( const Number        _) { const Value tmp(_); push_back(tmp); return back().as<Number>(); }
        String & Array:: push( const String       &_) { const Value tmp(_); push_back(tmp); return back().as<String>(); }
        String & Array:: push( const char         *_) { const Value tmp(_); push_back(tmp); return back().as<String>(); }

        _Pair:: _Pair( const string &l, const Value &v ) :
        label(l),
        value(v)
        {}
        
        _Pair:: _Pair( const string &l) :
        label(l),
        value()
        {}
        
        _Pair:: ~_Pair() throw()
        {
        }
        
        const string & _Pair:: key() const throw()
        {
            return label;
        }
        
        Object::  Object() throw() : _Object() {}

        Object:: ~Object() throw() {}
        
        Object:: Object( const Object &other ) :  collection(), _Object(other) {}

        Value &Object:: add(const string &label, const Value &v)
        {
            Pair p = new _Pair(label,v);
            if(!insert(p))
            {
                throw exception("JSON::Object mutliple label '%s' for JSON::%s",*label,v.typeName() );
            }
            return p->value;
        }

        Array  & Object:: add(const string &label, const ArrayType_t  &_)
        {
            Value tmp(_);
            Value &v = add(label,NullType);
            v.swapWith(tmp);
            return v.as<Array>();
        }

        Object & Object:: add(const string &label, const ObjectType_t  &_)
        {
            Value tmp(_);
            Value &v = add(label,NullType);
            v.swapWith(tmp);
            return v.as<Object>();
        }

        Number & Object::add(const string &label, const Number _)
        {
            Value tmp(_);
            Value &v = add(label,NullType);
            v.swapWith(tmp);
            return v.as<Number>();
        }

        String & Object::add(const string &label, const String &_)
        {
            Value tmp(_);
            Value &v = add(label,NullType);
            v.swapWith(tmp);
            return v.as<String>();
        }

        
        String & Object::add(const string &label, const char * _)
        {
            Value tmp(_);
            Value &v = add(label,NullType);
            v.swapWith(tmp);
            return v.as<String>();
        }

    }

}


#include "y/code/utils.hpp"

namespace upsylon
{

    namespace JSON
    {

        static inline void __emit_string( ios::ostream &os, const String &s )
        {
            static const char quotes = '"';
            os << quotes;
            for(size_t i=0;i<s.size();++i)
            {
                os << cchars::encoded[ uint8_t(s[i]) ];
            }
            os << quotes;
        }

        ios::ostream & Value:: display(ios::ostream &os, int level) const
        {
            switch (type)
            {
                case IsNull:  os << "null"; break;
                case IsTrue:  os << "true"; break;
                case IsFalse: os << "false"; break;
                case IsNumber: os("%.15g",as<Number>()); break;
                case IsString: __emit_string(os,as<String>());  break;
                case IsArray:  as<Array>( ).display(os,level);  break;
                case IsObject: as<Object>().display(os,level);  break;
            }
            return os;
        }

        void Array:: display( ios::ostream &os, int level) const
        {
            os << '[';
            for(size_t i=1;i<=size();++i)
            {
                (*this)[i].display(os,level);
                if(i<size())
                {
                    os << ',' << ' ';
                }
            }
            os << ']';
        }

        void Object:: display( ios::ostream &os, int level) const
        {
            os  << '{';
            const size_t n = size();
            size_t       j = 1;
            for(const_iterator i=begin();j<=n;++i,++j)
            {
                const Pair &p = *i;
                __emit_string(os,p->label);
                os << ' ' << ':' << ' ';
                (void) p->value.display(os,level);
                if(j<n)
                {
                    os << ',' << ' ';
                }
            }
            os << '}';
        }

    }
}
