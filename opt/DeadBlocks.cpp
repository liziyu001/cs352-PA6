//
//  DeadBlocks.cpp
//  uscc
//
//  Implements Dead Block Removal optimization pass.
//  This removes blocks from the CFG which are unreachable.
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
#include <llvm/IR/CFG.h>
#include <llvm/ADT/DepthFirstIterator.h>
#pragma clang diagnostic pop
#include <set>

using namespace llvm;

namespace uscc
{
namespace opt
{
	
bool DeadBlocks::runOnFunction(Function& F)
{
	bool changed = false;
	
	// PA6: Implement
	std::set<BasicBlock*> visitedSet;
	for (auto blockIter = df_begin(&F.getEntryBlock()), blockEnd = df_end(&F.getEntryBlock());
		 blockIter != blockEnd;
		 ++blockIter)
	{
		visitedSet.insert(*blockIter);
	}

	std::set<BasicBlock*> unreachableSet;
	for (Function::iterator blockIter = F.begin(); blockIter != F.end(); ++blockIter) {
		if (visitedSet.find(&*blockIter) == visitedSet.end()) {
			unreachableSet.insert(&*blockIter);
		}
	}

	if (unreachableSet.size() > 0) {
		changed = true;
		for (auto block : unreachableSet) {
			for (auto iter = succ_begin(block); iter != succ_end(block); iter++) {
				(*iter)->removePredecessor(block);
			}
			block->eraseFromParent();
		}
	}
	
	
	return changed;
}
	
void DeadBlocks::getAnalysisUsage(AnalysisUsage& Info) const
{
	// PA6: Implement
	Info.addRequired<ConstantBranch>();
}

} // opt
} // uscc

char uscc::opt::DeadBlocks::ID = 0;
