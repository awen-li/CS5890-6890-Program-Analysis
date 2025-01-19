#include "cmd_parser.h"
#include "analysis.h"

static inline void addCmdOptions (CommandLineParser& cmdParser)
{
    // Add your new option here
    cmdParser.addOption('b', "", "Specify the path of LLVM bitcode file for analysis");
}

static inline void analyzeModule(const string& bcPath)
{
    // Initialize LLVM context
    llvm::LLVMContext context;

    // Read IR file
    llvm::SMDiagnostic error;
    std::unique_ptr<llvm::Module> module = llvm::parseIRFile(bcPath, error, context);
    if (!module) 
    {
        error.print(bcPath.c_str(), llvm::errs());
        return;
    }

    llvm::outs() << "Loaded module:\n";
    module->print(llvm::outs(), nullptr);

    analyzeModule(*module);
    return;
}   

int main(int argc, char** argv) 
{
    CommandLineParser cmdParser(argc, argv);
    addCmdOptions (cmdParser);
    cmdParser.parse ();
    
    if (cmdParser.hasOption("h")) 
    {
        cmdParser.help ();
    }

    if (cmdParser.hasOption("b"))
    {
        analyzeModule (cmdParser.getOption ("b"));
    }

    return 0;
}
