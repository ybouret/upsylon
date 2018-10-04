
#include "y/lang/dynamo.hpp"
#include "y/lang/dynamo/generator.hpp"
#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {

        Syntax::Parser * Dynamo:: FromSource(const string &filename, const bool verbose)
        {
            DynamoCompiler         dynCompiler;
            auto_ptr<Syntax::Node> dynamo  = dynCompiler.process( Module::OpenFile(filename) );
            DynamoGenerator        dynGenerator;
            return dynGenerator.create(*dynamo,verbose);
        }

        Syntax::Parser * Dynamo:: FromBinary(const string &filename, const bool verbose)
        {
            DynamoParser           dynParser;
            Source                 source( Module::OpenFile(filename) );
            auto_ptr<Syntax::Node> dynamo  = Syntax::Node::Load(source,dynParser);
            DynamoGenerator        dynGenerator;
            return dynGenerator.create(*dynamo,verbose);
        }


        Syntax::Parser * Dynamo:: FromBinary(const char *name, const char *data, const size_t size,const bool verbose)
        {
            DynamoParser           dynParser;
            Source                 source( Module::OpenData(name,data,size) );
            auto_ptr<Syntax::Node> dynamo  = Syntax::Node::Load(source,dynParser);
            DynamoGenerator        dynGenerator;
            return dynGenerator.create(*dynamo,verbose);
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

        string Dynamo:: Compile(const string &filename)
        {
            DynamoCompiler         dynCompiler;
            auto_ptr<Syntax::Node> dynamo  = dynCompiler.process( Module::OpenFile(filename) );
            return dynamo->to_binary();
        }

#if 0
        Syntax::Node * Dynamo:: CompileAST( const string &filename )
        {
            std::cerr << "-- CompileAST from '" << filename << "'" << std::endl;
            DynamoCompiler dynCompiler;
            return dynCompiler.process( Module::OpenFile(filename) );
        }

        Syntax::Node * Dynamo:: RebuildAST( const string &filename )
        {
            std::cerr << "-- RebuildAST from '" << filename << "'" << std::endl;
            DynamoParser   grammar;
            Source         source( Module::OpenFile(filename) );
            return Syntax::Node::Load(source,grammar);
        }


        Syntax::Node * Dynamo:: LoadAST(const string &filename, const FormatType type)
        {
            std::cerr << "-- Loading AST" << std::endl;
            switch(type)
            {
                case SourceFile: return CompileAST(filename);
                case BinaryFile: return RebuildAST(filename);
            }
            throw exception("Dynamo.LoadAST(unexpected FormatType)");
        }

        Syntax::Node * Dynamo:: RebuildAST( const char *name, const char *data, const size_t size)
        {
            DynamoParser   grammar;
            Source         source( Module::OpenData(name,data,size) );
            return Syntax::Node::Load(source,grammar);
        }

        Syntax::Parser * Dynamo::CreateFrom( Syntax::Node *ast, const bool verbose )
        {
            assert(ast);
            std::cerr << "-- Creating Parser from AST" << std::endl;

            auto_ptr<Syntax::Node> dynamo(ast);
            {
                std::cerr << "-- Saving..." << std::endl;
                ast->GraphViz("dynast.dot");
                std::cerr << "-- ...done" << std::endl;
            }
            DynamoGenerator        dyn;
            return dyn.create(*dynamo,verbose);
        }
#endif

        Dynamo:: ~Dynamo() throw()
        {
        }


    }
}
