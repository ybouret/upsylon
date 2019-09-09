//! \file
#ifndef Y_OXIDE_FIELD_IO_INCLUDED
#define Y_OXIDE_FIELD_IO_INCLUDED 1

#include "y/ios/plugin.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include <typeinfo>

namespace upsylon
{

    namespace Oxide
    {

        class IO : public singleton<IO>
        {
        public:
            typedef hashing::fnv  HashFuntion;
            class  Hasher
            {
            public:
                HashFuntion H;
                ~Hasher() throw() {}
                Hasher() throw() : H() {}

                size_t operator()( const std::type_info &t ) throw()
                {
                    return H.key<size_t>( t.name(), strlen(t.name()) );
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Hasher);
            };

            class _Plugin : public counted_object, public ios::plugin
            {
            public:
                const std::type_info &tid;

                _Plugin(const std::type_info &t,
                        const ios::plugin::save_proc s,
                        const ios::plugin::load_proc l) throw() :
                ios::plugin(s,l), tid(t)
                {
                }

                virtual ~_Plugin() throw() {}

                const std::type_info &key() const throw() { return tid; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(_Plugin);
            };

            typedef intr_ptr<std::type_info,_Plugin>  Plugin;
            typedef set<std::type_info,Plugin,Hasher> Plugins;
            typedef Plugins::const_iterator           iterator;
            iterator begin() const throw() { return plugins.begin(); }
            iterator end()   const throw() { return plugins.end(); }

            template <typename T>
            void declare( const ios::plugin::save_proc s, const ios::plugin::load_proc p)
            {
                declare(typeid(T),s,p);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(IO);
            Plugins plugins;
            static const at_exit::longevity life_time = 0;
            virtual ~IO() throw();
            explicit IO();
            friend class singleton<IO>;

            void declare( const std::type_info       &tid,
                         const ios::plugin::save_proc s,
                         const ios::plugin::load_proc l);

        };

    };

}

#endif

