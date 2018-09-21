//!\file

#ifndef Y_LUA_FUNCTION_INCLUDED
#define Y_LUA_FUNCTION_INCLUDED 1

#include "y/lua++/vm.hpp"

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
            VM vm;
            explicit FunctionType(const string &, const VM &);
            FunctionType(const FunctionType &other);
            void pull();

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


        private:
            Y_DISABLE_ASSIGN(Function);
        };

    }

}

#endif

