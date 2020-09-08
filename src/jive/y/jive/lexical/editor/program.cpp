
#include "y/jive/lexical/editor/program.hpp"
#include "y/core/temporary-link.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Lexical
        {

            namespace Editor
            {
                Program:: ~Program() throw()
                {
                }

                Program:: Program(const EndlPolicy how) :
                exe(),
                xfc(),
                org(0)
                {
                    switch (how)
                    {
                        case EndlCopy:
                            on( "[:endl:]", this, & Program::endl_copy);
                            break;

                        case EndlToLF:
                            on( "[:endl:]", this, & Program::endl_toLF);
                            break;
                    }
                }



                void Program::endl_copy(ios::ostream &fp, const Token &t)
                {
                    assert(org);
                    org->newLine();
                    fp << t;
                }

                void Program::endl_toLF(ios::ostream &fp, const Token &)
                {
                    assert(org);
                    org->newLine();
                    fp << '\n';
                }




                namespace
                {
                    struct PNode
                    {
                        const Instruction *instr;
                        PNode             *next;
                    };

                    inline bool tryAccept(const PNode *node, Token &token, Source &source)
                    {
                        assert(node);
                        if(node->instr->motif->accept(token,source))
                        {
                            source.uncpy(token);
                            return true;
                        }
                        else
                        {
                            assert(0==token.size);
                            return false;
                        }
                    }
                }

                void Program:: add(Instruction *instr)
                {
                    //----------------------------------------------------------
                    //
                    // append instruction
                    //
                    //----------------------------------------------------------
                    assert(instr);
                    aliasing::_(exe).push_back(instr);

                    try
                    {
                        Leading tfc(xfc);                   // copy xfc
                        Leading lfc;                        // build local first chars
                        instr->motif->start(lfc);           // from intruction motif
                        aliasing::_(tbl).record(instr,lfc); // dispatch instruction
                        tfc.include(lfc);                   // included
                        tfc.commute( aliasing::_(xfc) );    // new state!
                    }
                    catch(...)
                    {
                        delete aliasing::_(exe).pop_back();
                        aliasing::_(tbl).remove(instr);
                        throw;
                    }

                }


                

                void Program:: run(ios::ostream &fp, Source &source)
                {
                    const core::temporary_link<Source> link(source,&org);

                    for(Char *ch = source.get();ch!=NULL;ch=source.get())
                    {
                        //------------------------------------------------------
                        // get the character code
                        //------------------------------------------------------
                        const uint8_t code = ch->code;
                        if( xfc.search(code) )
                        {
                            //--------------------------------------------------
                            // some instructions are registered
                            //--------------------------------------------------
                            source.unget(ch);

                            //--------------------------------------------------
                            // get the possible instructions
                            //--------------------------------------------------
                            const PNode *node     = (const PNode *)(tbl[code].head);
                            const PNode *bestNode = 0;
                            Token        bestData;

                            //--------------------------------------------------
                            // find a possible instructions
                            //--------------------------------------------------
                            while(node)
                            {
                                if(tryAccept(node,bestData,source))
                                {
                                    bestNode = node;
                                    break;
                                }
                                node=node->next;
                            }


                            if(!bestNode)
                            {
                                //----------------------------------------------
                                // none => neutral code and skip
                                //----------------------------------------------
                                fp.write(char(code));
                                source.skip();
                            }
                            else
                            {
                                //----------------------------------------------
                                // continue
                                //----------------------------------------------
                                for(node=node->next;node;node=node->next)
                                {
                                    Token tempData;
                                    if(tryAccept(node, tempData, source) )
                                    {
                                        if(tempData.size>bestData.size)
                                        {
                                            tempData.swap_with(bestData);
                                            bestNode = node;
                                        }
                                    }
                                }

                                //----------------------------------------------
                                // found the bestNode
                                //----------------------------------------------
                                if(bestData.size<=0)
                                {
                                    //------------------------------------------
                                    // feeble => neutral code and skip
                                    //------------------------------------------
                                    fp.write(char(code));
                                    source.skip();
                                }
                                else
                                {
                                    //------------------------------------------
                                    // skip and process
                                    //------------------------------------------
                                    source.skip(bestData.size);
                                    aliasing::_(bestNode->instr->code)(fp,bestData);
                                }
                            }
                        }
                        else
                        {
                            // neutral code
                            Char::Release(ch);
                            fp.write(char(code));
                        }
                    }

                }

            }

        }
    }
}

