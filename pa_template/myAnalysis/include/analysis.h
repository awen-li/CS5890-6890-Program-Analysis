#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "llvm/IR/Module.h"
#include "generic_graph.h"
#include "graph_visual.h"

void analyzeModule(llvm::Module& module);

#endif // ANALYSIS_H
