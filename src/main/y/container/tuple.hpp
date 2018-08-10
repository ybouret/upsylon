//! \file
#ifndef Y_TUPLE_INCLUDED
#define Y_TUPLE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/traits.hpp"
#include <iostream>

namespace upsylon
{
    //! for standard tuple
#define Y_TUPLE_STANDARD(T) type_traits< T >::parameter_type
    //! for tuple in a template
#define Y_TUPLE_TEMPLATE(T) typename type_traits< T >::parameter_type
    
    //! open brace
#define Y_TUPLE_INI() {
    //! close brace
#define Y_TUPLE_END() }
    //__________________________________________________________________________
    //
    // PAIR
    //__________________________________________________________________________

    
    //! pair prototype
#define Y_PAIR_DECL(PARAM,CLASS,T1,N1,T2,N2) \
class CLASS : public virtual object {        \
public:                                      \
typedef Y_TUPLE_##PARAM(T1) PARAM1;          \
typedef Y_TUPLE_##PARAM(T2) PARAM2;          \
inline CLASS( PARAM1 P1, PARAM2 P2) : N1(P1), N2(P2) {}                    \
inline CLASS(const CLASS &other) : object(), N1(other.N1), N2(other.N2) {} \
inline virtual ~CLASS() throw() {}                                         \
friend inline                                                              \
std::ostream & operator<<( std::ostream &os, const CLASS &self )           \
{                                                                          \
os << '(' << #N1 << '=' << self.N1 << ',' << #N2 << '=' << self.N2 << ')'; \
return os;                                                                 \
}                                                                          \
friend inline bool operator==(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1==rhs.N1) && (lhs.N2==rhs.N2);                               \
}                                                                          \
friend inline bool operator!=(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1!=rhs.N1) || (lhs.N2!=rhs.N2);                               \
}                                                                          \
T1 N1; T2 N2
    
    //! pair end
#define Y_PAIR_END() }
    
    //__________________________________________________________________________
    //
    // TRIPLE
    //__________________________________________________________________________
    
    //! triple prototype
#define Y_TRIPLE_DECL(PARAM,CLASS,T1,N1,T2,N2,T3,N3) \
class CLASS : public virtual object {        \
public:                                      \
typedef Y_TUPLE_##PARAM(T1) PARAM1;          \
typedef Y_TUPLE_##PARAM(T2) PARAM2;          \
typedef Y_TUPLE_##PARAM(T3) PARAM3;          \
inline CLASS( PARAM1 P1, PARAM2 P2, PARAM3 P3) : N1(P1), N2(P2), N3(P3)  {} \
inline CLASS(const CLASS &other) : object(), N1(other.N1), N2(other.N2), N3(other.N3) {} \
inline virtual ~CLASS() throw() {}                                         \
friend inline                                                              \
std::ostream & operator<<( std::ostream &os, const CLASS &self )           \
{                                                                          \
os\
<< '(' << #N1 << '=' << self.N1 \
<< ',' << #N2 << '=' << self.N2 \
<< ',' << #N3 << '=' << self.N3 \
<< ')'; \
return os;                                                                 \
}                                                                          \
friend inline bool operator==(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1==rhs.N1) && (lhs.N2==rhs.N2) && (lhs.N3==rhs.N3);           \
}                                                                          \
friend inline bool operator!=(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1!=rhs.N1) || (lhs.N2!=rhs.N2) || (lhs.N3!=rhs.N3);           \
}                                                                          \
T1 N1; T2 N2; T3 N3
    
    //! triple end
#define Y_TRIPLE_END() }
    
    //__________________________________________________________________________
    //
    // QUAD
    //__________________________________________________________________________
    
    //! quad prototype
#define Y_QUAD_DECL(PARAM,CLASS,T1,N1,T2,N2,T3,N3,T4,N4) \
class CLASS : public virtual object {        \
public:                                      \
typedef Y_TUPLE_##PARAM(T1) PARAM1;          \
typedef Y_TUPLE_##PARAM(T2) PARAM2;          \
typedef Y_TUPLE_##PARAM(T3) PARAM3;          \
typedef Y_TUPLE_##PARAM(T4) PARAM4;          \
inline CLASS( PARAM1 P1, PARAM2 P2, PARAM3 P3, PARAM4 P4) : N1(P1), N2(P2), N3(P3), N4(P4)  {} \
inline CLASS(const CLASS &other) : object(), N1(other.N1), N2(other.N2), N3(other.N3), N4(other.N4) {} \
inline virtual ~CLASS() throw() {}                                         \
friend inline                                                              \
std::ostream & operator<<( std::ostream &os, const CLASS &self )           \
{                                                                          \
os\
<< '(' << #N1 << '=' << self.N1 \
<< ',' << #N2 << '=' << self.N2 \
<< ',' << #N3 << '=' << self.N3 \
<< ',' << #N4 << '=' << self.N4 \
<< ')'; \
return os;                                                                 \
}                                                                          \
friend inline bool operator==(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1==rhs.N1) && (lhs.N2==rhs.N2) && (lhs.N3==rhs.N3) && (lhs.N4==rhs.N4); \
}                                                                          \
friend inline bool operator!=(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1!=rhs.N1) || (lhs.N2!=rhs.N2) || (lhs.N3!=rhs.N3) || (lhs.N4!=rhs.N4); \
}                                                                          \
T1 N1; T2 N2; T3 N3; T4 N4
    
    //! quad end
#define Y_QUAD_END() }
    
    //__________________________________________________________________________
    //
    // PENTUPLE
    //__________________________________________________________________________
    
    //! pentuple prototype
#define Y_PENTUPLE_DECL(PARAM,CLASS,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5) \
class CLASS : public virtual object {        \
public:                                      \
typedef Y_TUPLE_##PARAM(T1) PARAM1;          \
typedef Y_TUPLE_##PARAM(T2) PARAM2;          \
typedef Y_TUPLE_##PARAM(T3) PARAM3;          \
typedef Y_TUPLE_##PARAM(T4) PARAM4;          \
typedef Y_TUPLE_##PARAM(T5) PARAM5;          \
inline CLASS( PARAM1 P1, PARAM2 P2, PARAM3 P3, PARAM4 P4, PARAM5 P5) : N1(P1), N2(P2), N3(P3), N4(P4), N5(P5)  {} \
inline CLASS(const CLASS &other) : object(), N1(other.N1), N2(other.N2), N3(other.N3), N4(other.N4), N5(other.N5) {} \
inline virtual ~CLASS() throw() {}                                         \
friend inline                                                              \
std::ostream & operator<<( std::ostream &os, const CLASS &self )           \
{                                                                          \
os\
<< '(' << #N1 << '=' << self.N1 \
<< ',' << #N2 << '=' << self.N2 \
<< ',' << #N3 << '=' << self.N3 \
<< ',' << #N4 << '=' << self.N4 \
<< ',' << #N5 << '=' << self.N5 \
<< ')'; \
return os;                                                                 \
}                                                                          \
friend inline bool operator==(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1==rhs.N1) && (lhs.N2==rhs.N2) && (lhs.N3==rhs.N3) && (lhs.N4==rhs.N4) && (lhs.N5==rhs.N5); \
}                                                                          \
friend inline bool operator!=(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1!=rhs.N1) || (lhs.N2!=rhs.N2) || (lhs.N3!=rhs.N3) || (lhs.N4!=rhs.N4) || (lhs.N5!=rhs.N5); \
}                                                                          \
T1 N1; T2 N2; T3 N3; T4 N4; T5 N5
    
    //! quad end
#define Y_PENTUPLE_END() }
    
    //__________________________________________________________________________
    //
    // SEXTUPLE
    //__________________________________________________________________________
    
    //! sextuple prototype
#define Y_SEXTUPLE_DECL(PARAM,CLASS,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6) \
class CLASS : public virtual object {        \
public:                                      \
typedef Y_TUPLE_##PARAM(T1) PARAM1;          \
typedef Y_TUPLE_##PARAM(T2) PARAM2;          \
typedef Y_TUPLE_##PARAM(T3) PARAM3;          \
typedef Y_TUPLE_##PARAM(T4) PARAM4;          \
typedef Y_TUPLE_##PARAM(T5) PARAM5;          \
typedef Y_TUPLE_##PARAM(T6) PARAM6;          \
inline CLASS( PARAM1 P1, PARAM2 P2, PARAM3 P3, PARAM4 P4, PARAM5 P5, PARAM6 P6) : N1(P1), N2(P2), N3(P3), N4(P4), N5(P5), N6(P6)  {} \
inline CLASS(const CLASS &other) : object(), N1(other.N1), N2(other.N2), N3(other.N3), N4(other.N4), N5(other.N5), N6(other.N6) {} \
inline virtual ~CLASS() throw() {}                                         \
friend inline                                                              \
std::ostream & operator<<( std::ostream &os, const CLASS &self )           \
{                                                                          \
os\
<< '(' << #N1 << '=' << self.N1 \
<< ',' << #N2 << '=' << self.N2 \
<< ',' << #N3 << '=' << self.N3 \
<< ',' << #N4 << '=' << self.N4 \
<< ',' << #N5 << '=' << self.N5 \
<< ',' << #N6 << '=' << self.N6 \
<< ')'; \
return os;                                                                 \
}                                                                          \
friend inline bool operator==(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1==rhs.N1) && (lhs.N2==rhs.N2) && (lhs.N3==rhs.N3) && (lhs.N4==rhs.N4) && (lhs.N5==rhs.N5) && (lhs.N6==rhs.N6); \
}                                                                          \
friend inline bool operator!=(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1!=rhs.N1) || (lhs.N2!=rhs.N2) || (lhs.N3!=rhs.N3) || (lhs.N4!=rhs.N4) || (lhs.N5!=rhs.N5) || (lhs.N6!=rhs.N6);; \
}                                                                          \
T1 N1; T2 N2; T3 N3; T4 N4; T5 N5; T6 N6
    
    //! sextuple end
#define Y_SEXTUPLE_END() }
    
    //__________________________________________________________________________
    //
    // SEPTUPLE
    //__________________________________________________________________________
    
    //! septuple prototype
#define Y_SEPTUPLE_DECL(PARAM,CLASS,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7) \
class CLASS : public virtual object {        \
public:                                      \
typedef Y_TUPLE_##PARAM(T1) PARAM1;          \
typedef Y_TUPLE_##PARAM(T2) PARAM2;          \
typedef Y_TUPLE_##PARAM(T3) PARAM3;          \
typedef Y_TUPLE_##PARAM(T4) PARAM4;          \
typedef Y_TUPLE_##PARAM(T5) PARAM5;          \
typedef Y_TUPLE_##PARAM(T6) PARAM6;          \
typedef Y_TUPLE_##PARAM(T7) PARAM7;          \
inline CLASS( PARAM1 P1, PARAM2 P2, PARAM3 P3, PARAM4 P4, PARAM5 P5, PARAM6 P6, PARAM7 P7) : N1(P1), N2(P2), N3(P3), N4(P4), N5(P5), N6(P6), N7(P7)  {} \
inline CLASS(const CLASS &other) : object(), N1(other.N1), N2(other.N2), N3(other.N3), N4(other.N4), N5(other.N5), N6(other.N6), N7(other.N7) {} \
inline virtual ~CLASS() throw() {}                                         \
friend inline                                                              \
std::ostream & operator<<( std::ostream &os, const CLASS &self )           \
{                                                                          \
os\
<< '(' << #N1 << '=' << self.N1 \
<< ',' << #N2 << '=' << self.N2 \
<< ',' << #N3 << '=' << self.N3 \
<< ',' << #N4 << '=' << self.N4 \
<< ',' << #N5 << '=' << self.N5 \
<< ',' << #N6 << '=' << self.N6 \
<< ',' << #N7 << '=' << self.N7 \
<< ')'; \
return os;                                                                 \
}                                                                          \
friend inline bool operator==(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1==rhs.N1) && (lhs.N2==rhs.N2) && (lhs.N3==rhs.N3) && (lhs.N4==rhs.N4) && (lhs.N5==rhs.N5) && (lhs.N6==rhs.N6) && (lhs.N7==rhs.N7); \
}                                                                          \
friend inline bool operator!=(const CLASS &lhs, const CLASS &rhs) throw()  \
{                                                                          \
return (lhs.N1!=rhs.N1) || (lhs.N2!=rhs.N2) || (lhs.N3!=rhs.N3) || (lhs.N4!=rhs.N4) || (lhs.N5!=rhs.N5) || (lhs.N6!=rhs.N6) || (lhs.N7!=rhs.N7); \
}                                                                          \
T1 N1; T2 N2; T3 N3; T4 N4; T5 N5; T6 N6; T7 N7
    
    //! sextuple end
#define Y_SEPTUPLE_END() }
    
}

#endif

