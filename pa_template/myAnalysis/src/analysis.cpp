#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include "analysis.h"

void analyzeModule(llvm::Module& module) {
    for (auto& function : module) {
        llvm::outs() << "Function: " << function.getName() << "\n";

        for (auto& block : function) {
            llvm::outs() << "  Basic block:\n";
            for (auto& instruction : block) {
                llvm::outs() << "    " << instruction << "\n";
            }
        }
    }
}
