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

void analyzeModule(llvm::Module& module) 
{
    printFunctions (module);
}
