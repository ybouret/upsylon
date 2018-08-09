#ifndef Y_TUPLE_INCLUDED
#define Y_TUPLE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/traits.hpp"
#include <iostream>

namespace upsylon
{
    
#define Y_TUPLE_STANDARD(T) type_traits< T >::parameter_type
#define Y_TUPLE_TEMPLATE(T) typename type_traits< T >::parameter_type
    
#define Y_PAIR_DECL(PARAM,CLASS,T1,N1,T2,N2) \
class CLASS : public virtual object {        \
public:                                      \
typedef Y_TUPLE_##PARAM(T1) PARAM1;          \
typedef Y_TUPLE_##PARAM(T2) PARAM2;          \
inline CLASS( PARAM1 P1, PARAM2 P2) : N1(P1), N2(P2) {} \
inline CLASS(const CLASS &other) : object(), N1(other.N1), N2(other.N2) {}\
inline virtual ~CLASS() throw() {} \
friend inline \
std::ostream & operator<<( std::ostream &pair_os, const CLASS &self )\
{\
pair_os << '(' << #N1 << '=' << self.N1 << ',' << #N2 << '=' << self.N2 << ')';\
return pair_os;\
}\
friend inline bool operator==(const CLASS &lhs, const CLASS &rhs) throw()\
{\
return (lhs.N1==rhs.N1) && (lhs.N2==rhs.N2);\
}\
friend inline bool operator!=(const CLASS &lhs, const CLASS &rhs) throw()\
{\
return (lhs.N1!=rhs.N1) || (lhs.N2!=rhs.N2);\
}\
T1 N1; T2 N2
    
#define Y_PAIR_END() }
}

#endif

