
#include "y/jive/lexical/editor/program.hpp"

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

                Program:: Program() : exe(), idb(), xfc()
                {
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



                void Program:: run(ios::ostream &fp, Source &source)
                {
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

