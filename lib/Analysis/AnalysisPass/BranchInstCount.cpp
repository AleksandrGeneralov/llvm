#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "BranchInstCount"

STATISTIC(NumCondBranch, "Number of conditional branches in the program");
STATISTIC(NumUncondBranch, "Number of unconditional branches in the program");
STATISTIC(NumEqBranch, "Number of conditional branches whose comparison type is equal test");
STATISTIC(NumGTBranch, "Number of conditional branches whose comparison type is greater than test");
STATISTIC(NumLTBranch, "Number of conditional branches whose comparison type is less than test");

namespace {
    struct BranchInstCount : public FunctionPass {
		static char ID; // Pass identification, replacement for typeid
		BranchInstCount() : FunctionPass(ID) {}

		bool runOnFunction(Function &F) override {
			for (Function::iterator bb = F.begin(), bb_e = F.end(); bb != bb_e; ++bb) {
				for (BasicBlock::iterator ii = bb->begin(), ii_e = bb->end(); ii != ii_e; ++ii) {
					BranchInst* branch_inst = dyn_cast<BranchInst>(ii);
				
					if(branch_inst && branch_inst->isConditional()) {
						errs() << "    Conditional: ";
						branch_inst->print(errs(), true);
						errs() << "\n";
					
						NumCondBranch++;
					
						CmpInst* cmp_inst = dyn_cast<CmpInst>(branch_inst->getCondition());
						if(cmp_inst) {
							errs() << "\n    Predicate is " << cmp_inst->getPredicateName(cmp_inst->getPredicate());
							errs() << "\n\n";
							switch(cmp_inst->getPredicate()) {
							case CmpInst::ICMP_EQ:
								NumEqBranch++;
								break;
							case CmpInst::ICMP_SGT:
								NumGTBranch++;
								break;
							case CmpInst::ICMP_SLT:
								NumLTBranch++;
								break;
							default:
								break;
							}
						}
					}				
					else if(branch_inst && branch_inst->isUnconditional()) {
						NumUncondBranch++;

						errs() << "    Unconditional: ";
						branch_inst->print(errs(), true);
						errs() << "\n";
					}
				}
			}

			return false;
		
    };
}

char BranchInstCount::ID = 0;
static RegisterPass<BranchInstCount> X("branchinstcount", "BranchInstCount Pass", false, true);