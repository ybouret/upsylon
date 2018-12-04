//!\file

#ifndef Y_LUA_FUNCTION_INCLUDED
#define Y_LUA_FUNCTION_INCLUDED 1

#include "y/lua++/state.hpp"

namespace upsylon
{

    namespace Lua
    {

        //! base type for lua++ functions
        class FunctionType
        {
        public:
            virtual ~FunctionType() throw(); //!< destructor
            const string name;               //!< its name

        protected:
            VM         vm; //!< shared lua_State
            lua_State *L;  //!< the lua_State
            explicit FunctionType(const string &, const VM &); //!< setup
            FunctionType(const FunctionType &other);           //!< copy
            void pull();                                       //!< pull the function by its name
            void call( const int narg, const int nret);        //!< execute the function

        private:
            Y_DISABLE_ASSIGN(FunctionType);
        };

        //! lua function call wrapper
        template <typename T>
        class Function : public FunctionType
        {
        public:

            //! setup
            inline explicit Function(const string &fcn, const VM &lvm) :
            FunctionType(fcn,lvm)
            {
            }

            //! copy
            inline Function(const Function &other) : FunctionType(other) {}

            //! destructor
            inline virtual ~Function() throw() {}

            //! 1 argument call
            inline T operator()(const T x)
            {
                pull();
                vm->push<T>(x);
                call(1,1);
                const T ans( vm->to<T>(-1) );
                lua_pop(L,1);
                return ans;
            }

            //! two arguments call
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

