#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "generic_graph.h"
#include "graph_visual.h"
#include "llvm_wrapper.h"

void analyzeModule(LLVM& llvmParser, string type);

#endif // ANALYSIS_H
