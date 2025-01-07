#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include "analysis.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        llvm::errs() << "Usage: " << argv[0] << " <IR file>\n";
        return 1;
    }

    // Initialize LLVM context
    llvm::LLVMContext context;

    // Read IR file
    llvm::SMDiagnostic error;
    std::unique_ptr<llvm::Module> module = llvm::parseIRFile(argv[1], error, context);
    if (!module) {
        error.print(argv[0], llvm::errs());
        return 1;
    }

    llvm::outs() << "Loaded module:\n";
    module->print(llvm::outs(), nullptr);

    // Perform analysis
    analyzeModule(*module);

    return 0;
}
