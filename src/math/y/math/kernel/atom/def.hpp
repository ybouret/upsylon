//! \file

//! followed by members
#define Y_MK_ATOM_OPS_API() \
struct ops {

//! followed by getting variables
#define Y_MK_ATOM_OPS_GET(SELF) \
static inline void call(void *_, parallel &ctx, lockable &access) {\
(void)access;\
ops & SELF = *static_cast<ops*>(_)


//! compute the dedicated indices and register variables
#define Y_MK_ATOM_OPS_USE_(LENGTH,CODE) \
size_t __offset = 1;         \
size_t __length = (LENGTH);  \
ctx.split(__length,__offset); \
while(__length--) { const size_t __indx = __offset++; Y_MK_ATOM_##CODE(__indx); }\
} }; \
ops __self ={

//! compute the dedicated indices
#define Y_MK_ATOM_OPS_USE(LENGTH,CODE) Y_MK_ATOM_OPS_USE_(LENGTH,CODE)

//! compute the dedicated indices with some prolog code
#define Y_MK_ATOM_OPS_USE_WITH(PROLOG,LENGTH,CODE) \
PROLOG; Y_MK_ATOM_OPS_USE_(LENGTH,CODE)

//! execute SIMD instructions
#define Y_MK_ATOM_OPS_RUN(LOOP) }; (LOOP).run( ops::call, & __self )
