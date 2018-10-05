
#include "y/lang/dynamo.hpp"
#include "y/lang/dynamo/generator.hpp"
#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {

        namespace
        {
            class DynamoServer : public singleton<DynamoServer>
            {
            public:
                DynamoCompiler  compiler;
                DynamoParser   &parser;
                DynamoGenerator generator;




                Syntax::Node * process( Module *module )
                {
                    Y_LOCK(access);
                    return compiler.process(module);
                }

                Syntax::Parser *create( Syntax::Node *dynamo, const bool verbose )
                {
                    auto_ptr<Syntax::Node> guard(dynamo);
                    Y_LOCK(access);
                    return generator.create(*dynamo,verbose);
                }

            private:

                friend class singleton<DynamoServer>;

                explicit DynamoServer() throw() :
                compiler(), parser( compiler.parser ), generator()
                {
                    std::cerr << "DynamoServer#reserved=" << reserved() << std::endl;
                }

                virtual ~DynamoServer() throw()
                {
                }

                static const at_exit::longevity life_time = memory::pooled::life_time-10;

            };
        }

        Syntax::Parser * Dynamo:: FromSource(const string &filename, const bool verbose)
        {
            static DynamoServer &dynamo = DynamoServer::instance();
            return dynamo.create( dynamo.process( Module::OpenFile(filename) ), verbose);
        }

        Syntax::Parser * Dynamo:: FromBinary(const string &filename, const bool verbose)
        {
            Source                 source( Module::OpenFile(filename) );
            static DynamoServer   &dynamo = DynamoServer::instance();

            return dynamo.create(Syntax::Node::Load(source,dynamo.parser),verbose);
        }


        Syntax::Parser * Dynamo:: FromBinary(const char *name, const char *data, const size_t size,const bool verbose)
        {
            static DynamoServer   &dynamo = DynamoServer::instance();
            Source                 source( Module::OpenData(name,data,size) );

            return dynamo.create(Syntax::Node::Load(source,dynamo.parser),verbose);
        }

        Syntax::Parser * Dynamo:: Load(const string &filename, const FormatType type, const bool verbose)
        {
            switch (type)
            {
                case SourceFile: return FromSource(filename,verbose);
                case BinaryFile: return FromBinary(filename,verbose);
            }
            throw exception("Dynamo.Load(unexpected FormatType)");
        }

        string Dynamo:: ToBinary(const string &filename)
        {
            static DynamoServer   &dynamo = DynamoServer::instance();
            auto_ptr<Syntax::Node> node   = dynamo.process( Module::OpenFile(filename) );
            return node->to_binary();
        }


        Dynamo:: ~Dynamo() throw()
        {
        }


        

    }
}
