#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/GraphWriter.h"
#include "analysis.h"
#include "pag.h"
#include "cfg.h"

using namespace llvm;

static inline void printFunctions (LLVM& llvmParser)
{
    outs() <<"@@printFunctions \n";

    for (auto it = llvmParser.func_begin (); it != llvmParser.func_end (); it++) 
    {
        Function *function = *it;
        outs() << "Function: " << function->getName() << "\n";

        const FunctionType *funcType = function->getFunctionType();
        outs() << "\t>>Type: " << *funcType << "\n";

        if (function->isDeclaration()) 
        {
            outs() << "\t>>Function is a declaration\n";
        } 
        else 
        {
            outs() << "\t>>Function is a definition\n";
        }

        outs() <<"\n";
    }
}

static inline void printBasicBlockStats(LLVM& llvmParser) 
{
    outs() << "@@printBasicBlockStats\n";

    for (auto it = llvmParser.func_begin (); it != llvmParser.func_end (); it++) 
    {
        Function *function = *it;

        unsigned int basicBlockCount = 0;
        for (const auto &bb : *function) 
        {
            basicBlockCount++;
        }

        outs() << "Function: " << function->getName() << "\n";
        outs() << "\tNumber of Basic Blocks: " << basicBlockCount << "\n";
    }
}


static inline void printInstructions (LLVM& llvmParser)
{
    errs() << "@@printInstructions\n";

    for (auto it = llvmParser.func_begin (); it != llvmParser.func_end (); it++) 
    {
        llvm::Function *function = *it;

        if (function->isDeclaration())
            continue;

        outs() << "Function: " << function->getName() << "\n";
        for (const auto &bb : *function) 
        {
            for (const auto &instr : bb) 
            {
                StringRef opcodeName = instr.getOpcodeName();
                outs() << "    [" << opcodeName << "] " << instr << "\n";
            }
        }

        outs() << "\n";
    }
}

static inline void printCG (std::map<std::string, std::set<std::string>>& callGraph)
{
    for (const auto &caller : callGraph) 
    {
        outs() << "Function: " << caller.first << "\n";
        if (!caller.second.empty())
        {
            outs() << "  Calls:\n";
            for (const auto &callee : caller.second) 
            {
                outs() << "    - " << callee << "\n";
            }
        }
        else
        {
            outs() << "  Calls: None\n";
        }

        outs() << "\n";
    }
}

static inline void printCallGraph(LLVM& llvmParser) 
{
    errs() << "@@printCallGraph\n";

    std::map<std::string, std::set<std::string>> callGraph;
    for (auto it = llvmParser.func_begin (); it != llvmParser.func_end (); it++) 
    {
        Function *function = *it;

        std::string callerName = function->getName().str();
        callGraph[callerName] = std::set<std::string>();

        for (const auto &bb : *function) 
        {
            for (const auto &instr : bb) 
            {
                if (const CallBase *call = dyn_cast<CallBase>(&instr)) 
                {
                    if (Function *calledFunc = call->getCalledFunction()) 
                    {
                        std::string calleeName = calledFunc->getName().str();
                        callGraph[callerName].insert(calleeName);
                    }
                }
            }
        }
    }

    printCG (callGraph);    
}


void DumpCallGraph(CallGraph &CG, const std::string &FileName)
{
    std::error_code EC;
    raw_fd_ostream File(FileName, EC, sys::fs::OF_Text);

    if (!EC) 
    {
        File << "digraph CallGraph {\n";
        std::set<std::pair<std::string, std::string>> uniqueEdges;
                
        for (auto &node : CG) 
        {
            const Function *F = node.first;
            if (!F || F->isDeclaration()) continue;

            std::string FuncName = F->getName().str();
            File << "\"" << FuncName << "\";\n";

            CallGraphNode *CGN = node.second.get();
            for (unsigned i = 0; i < CGN->size(); ++i) 
            {
                CallGraphNode *CalleeNode = CGN->operator[](i);
                Function *Callee = CalleeNode->getFunction();
                if (Callee && !Callee->isDeclaration()) 
                {
                    std::string CalleeName = Callee->getName().str();
                    if (uniqueEdges.insert({FuncName, CalleeName}).second) 
                    {
                        File << "\"" << FuncName << "\" -> \"" << CalleeName << "\";\n";
                    }
                }
            }
        }

        File << "}\n";
        errs() << "Call graph saved to " << FileName << "\n";
    } 
    else 
    {
        errs() << "Error opening file: " << EC.message() << "\n";
    }
}

bool genCallGraph (LLVM& llvmParser) 
{
    llvm::Module* m = llvmParser.getModule();
    CallGraph CG(*m);

    DumpCallGraph (CG, "callgraph.dot");
    return false;
}

static inline string getBBName (const llvm::BasicBlock &BB)
{
    std::string bbName;
    if (BB.hasName())
    {
        bbName = BB.getName().str();
    }
    else
    {
        bbName = "BB_" + std::to_string(reinterpret_cast<uintptr_t>(&BB));
    }

    return bbName;            
}

static inline string getBBLabel (const llvm::BasicBlock &BB)
{
    string label = "";
    for (const llvm::Instruction &I : BB) 
    {
        std::string instStr;
        llvm::raw_string_ostream instStream(instStr);
        I.print(instStream);
        label += instStream.str() + "\\l";
     }

     return label;
}

void printCFG (LLVM& llvmParser, const std::string &Filename = "cfg.dot") 
{
    std::error_code EC;
    llvm::raw_fd_ostream File(Filename, EC, llvm::sys::fs::OF_Text);
    if (EC) 
    {
        llvm::errs() << "Error opening file " << Filename 
                     << " for writing: " << EC.message() << "\n";
        return;
    }

    File << "digraph CFG {\n";
    for (auto it = llvmParser.func_begin (); it != llvmParser.func_end (); it++) 
    {
        Function *F = *it;
        if (F->isDeclaration())
            continue;

        File << "subgraph cluster_" << F->getName().str() << " {\n";
        File << "label = \"" << F->getName().str() << "\";\n";

        for (const llvm::BasicBlock &BB : *F) 
        {
            std::string bbName = getBBName (BB);
            std::string label  = getBBLabel (BB);

            label += bbName + "\\l";
            File << "\"" << bbName << "\" [shape=rectangle, label=\"" << label << "\"];\n";
        }
        
        for (const llvm::BasicBlock &BB : *F) 
        {
            std::string bbName = getBBName (BB);
            for (const llvm::BasicBlock *Succ : llvm::successors(&BB)) 
            {
                std::string succName = getBBName (*Succ);
                File << "\"" << bbName << "\" -> \"" << succName << "\";\n";
            }
        }

        File << "}\n";
    }

    File << "}\n";
}

void printPAG (LLVM& llvmParser, const std::string &Filename = "pag") 
{
    PAG pag (&llvmParser);
    pag.build();
    
    PAGVis vis(Filename, &pag);
    vis.witeGraph();

    return;
}


void analyzeModule(LLVM& llvmParser, string type) 
{
    if (type == "function")
        printFunctions (llvmParser);

    if (type == "block")
        printBasicBlockStats (llvmParser);

    if (type == "inst")
        printInstructions (llvmParser);

    if (type == "cg")
    {
        printCallGraph (llvmParser);
        genCallGraph (llvmParser);
    }

    if (type == "cfg")
    {
        printCFG (llvmParser);
    }

    if (type == "pag")
    {
        printPAG (llvmParser);
    }
}
