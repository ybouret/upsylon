//!\file

#ifndef Y_LUA_FUNCTION_INCLUDED
#define Y_LUA_FUNCTION_INCLUDED 1

#include "y/lua++/state.hpp"

namespace upsylon
{

    namespace Lua
    {

        class FunctionType
        {
        public:
            virtual ~FunctionType() throw();

            const string name;

        protected:
            VM         vm;
            lua_State *L;
            explicit FunctionType(const string &, const VM &);
            FunctionType(const FunctionType &other);
            void pull();
            void call( const int narg, const int nret);

        private:
            Y_DISABLE_ASSIGN(FunctionType);
        };

        template <typename T>
        class Function : public FunctionType
        {
        public:
            inline explicit Function(const string &fcn, const VM &lvm) :
            FunctionType(fcn,lvm)
            {
            }

            inline Function(const Function &other) : FunctionType(other) {}

            inline virtual ~Function() throw() {}

            inline T operator()(const T x)
            {
                pull();
                vm->push<T>(x);
                call(1,1);
                const T ans( vm->to<T>(-1) );
                lua_pop(L,1);
                return ans;
            }

            inline T operator()(const T x, const T y)
            {
                pull();
                vm->push<T>(x);
                vm->push<T>(y);
                call(2,1);
                const T ans( vm->to<T>(-1) );
                lua_pop(L,1);
                return ans;
            }


        private:
            Y_DISABLE_ASSIGN(Function);
        };

    }

}

#endif

