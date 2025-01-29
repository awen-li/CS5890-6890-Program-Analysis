#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "analysis.h"

using namespace llvm;

static inline void printFunctions (llvm::Module& module)
{
    outs() <<"@@printFunctions \n";

    for (const auto &function : module) 
    {
        outs() << "Function: " << function.getName() << "\n";

        const FunctionType *funcType = function.getFunctionType();
        outs() << "\t>>Type: " << *funcType << "\n";

        if (function.isDeclaration()) 
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

static inline void printBasicBlockStats(const Module& module) 
{
    outs() << "@@printBasicBlockStats\n";

    for (const auto &function : module) 
    {
        unsigned int basicBlockCount = 0;
        for (const auto &bb : function) 
        {
            basicBlockCount++;
        }

        outs() << "Function: " << function.getName() << "\n";
        outs() << "\tNumber of Basic Blocks: " << basicBlockCount << "\n";
    }
}


static inline void printInstructions (const Module& module)
{
    errs() << "@@printInstructions\n";

    for (const auto &function : module) 
    {
        if (function.isDeclaration())
            continue;

        outs() << "Function: " << function.getName() << "\n";
        for (const auto &bb : function) 
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

void analyzeModule(llvm::Module& module, string type) 
{
    if (type == "function")
        printFunctions (module);

    if (type == "block")
        printBasicBlockStats (module);

    if (type == "inst")
        printInstructions (module);
}
