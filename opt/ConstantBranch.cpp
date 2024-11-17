//
//  ContantBranch.cpp
//  uscc
//
//  Implements Constant Branch Folding opt pass.
//  This converts conditional branches on constants to
//  unconditional branches.
//
//---------------------------------------------------------
//  Copyright (c) 2014, Sanjay Madhav
//  All rights reserved.
//
//  This file is distributed under the BSD license.
//  See LICENSE.TXT for details.
//---------------------------------------------------------
#include "Passes.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>
#pragma clang diagnostic pop
#include <set>

using namespace llvm;

namespace uscc
{
namespace opt
{
	
bool ConstantBranch::runOnFunction(Function& F)
{
	bool changed = false;
	
	// Make a set that contains the instructions we'll remove
	std::set<Instruction*> removeSet;

	// PA6: Implement
	for (Function::iterator blockIter = F.begin(); blockIter != F.end(); ++blockIter) {
		for (BasicBlock::iterator instrIter = blockIter->begin(); instrIter != blockIter->end(); ++instrIter) {
			if (BranchInst* branch = dyn_cast<BranchInst>(instrIter)) {
				if (branch->isConditional()) {
					if (ConstantInt* cond = dyn_cast<ConstantInt>(branch->getCondition())) {
						removeSet.insert(instrIter);
					}
				}
			}
		}
	}

	if (removeSet.size() > 0) {
		changed = true;
		for (std::set<Instruction*>::iterator i = removeSet.begin(); i != removeSet.end(); ++i) {
			BranchInst* branch = cast<BranchInst>(*i);
			ConstantInt* cond = cast<ConstantInt>(branch->getCondition());
			if (cond->isZero()) {
				BasicBlock* target = branch->getSuccessor(1);
				BranchInst::Create(target, branch);
			} else {
				BasicBlock* target = branch->getSuccessor(0);
				BranchInst::Create(target, branch);
			}
			(*i)->eraseFromParent();
		}
	}
	
	
	return changed;
}

void ConstantBranch::getAnalysisUsage(AnalysisUsage& Info) const
{
	// PA6: Implement
	Info.addRequired<ConstantOps>();
}
	
} // opt
} // uscc

char uscc::opt::ConstantBranch::ID = 0;
