/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                       */
/*    This file is part of the HiGHS linear optimization suite           */
/*                                                                       */
/*    Written and engineered 2008-2020 at the University of Edinburgh    */
/*                                                                       */
/*    Available as open-source under the MIT License                     */
/*                                                                       */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "mip/HighsMipSolver.h"

#include "mip/HighsCliqueTable.h"
#include "mip/HighsCutPool.h"
#include "mip/HighsDomain.h"
#include "mip/HighsImplications.h"
#include "mip/HighsLpRelaxation.h"
#include "mip/HighsMipSolverData.h"
#include "mip/HighsPseudocost.h"
#include "mip/HighsSearch.h"
#include "mip/HighsSeparation.h"
#include "util/HighsCDouble.h"

HighsMipSolver::HighsMipSolver(const HighsOptions& options, const HighsLp& lp)
    : options_mip_(&options), model_(&lp) {}

HighsMipSolver::~HighsMipSolver() = default;

void HighsMipSolver::run() {
  mipdata_ = decltype(mipdata_)(new HighsMipSolverData(*this));
  mipdata_->timer.start(mipdata_->timer.solve_clock);
  mipdata_->setup();
  mipdata_->evaluateRootNode();

  if (mipdata_->nodequeue.empty()) {
    printf("\nmodel was solved in the root node\n");
    mipdata_->timer.stop(mipdata_->timer.solve_clock);
    return;
  }

  printf("\nstarting tree search\n");

  std::shared_ptr<const HighsBasis> basis;
  HighsSearch search{*this, mipdata_->pseudocost};
  HighsSeparation sepa;

  search.setLpRelaxation(&mipdata_->lp);
  sepa.setLpRelaxation(&mipdata_->lp);

  mipdata_->lower_bound = mipdata_->nodequeue.getBestLowerBound();
  search.installNode(mipdata_->nodequeue.popBestBoundNode());

  while (search.hasNode()) {
    // set iteration limit for each lp solve during the dive to 10 times the
    // average nodes

    mipdata_->lp.setIterationLimit(
        10 * int(mipdata_->lp.getNumLpIterations() /
                 (double)std::max(size_t{1}, mipdata_->num_nodes)));

    // perform the dive and put the open nodes to the queue
    size_t plungestart = mipdata_->num_nodes;
    while (true) {
      search.dive();
      ++mipdata_->num_leaves;

      search.flushStatistics();
      if (!search.backtrack()) break;

      if (search.getCurrentEstimate() >= mipdata_->upper_limit) break;

      if (mipdata_->num_nodes - plungestart >= 1000) break;

      if (mipdata_->dispfreq != 0) {
        if (mipdata_->num_leaves - mipdata_->last_displeave >=
            mipdata_->dispfreq)
          mipdata_->printDisplayLine();
      }

      // printf("continue plunging due to good esitmate\n");
    }
    search.openNodesToQueue(mipdata_->nodequeue);
    mipdata_->lower_bound = std::min(mipdata_->upper_bound,
                                     mipdata_->nodequeue.getBestLowerBound());

    if (mipdata_->dispfreq != 0) {
      if (mipdata_->num_leaves - mipdata_->last_displeave >= mipdata_->dispfreq)
        mipdata_->printDisplayLine();
    }

    // the search datastructure should have no installed node now
    assert(!search.hasNode());

    // propagate the global domain
    mipdata_->domain.propagate();

#ifdef HIGHS_DEBUGSOL
    for (int i = 0; i != mipsolver.numCol(); ++i) {
      assert(lp.getMip().debugSolution_[i] + 1e-6 >=
             separate mipsolver.mipdata_->domain.colLower_[i]);
      assert(lp.getMip().debugSolution_[i] - 1e-6 <=
             mipsolver.mipdata_->domain.colUpper_[i]);
    }
#endif

    // if global propagation detected infeasibility, stop here
    if (mipdata_->domain.infeasible()) {
      mipdata_->nodequeue.clear();
      mipdata_->pruned_treeweight = 1.0;
      break;
    }

    // if global propagation found bound changes, we update the local domain
    if (!mipdata_->domain.getChangedCols().empty()) {
      printf("added %lu global bound changes\n",
             mipdata_->domain.getChangedCols().size());

      mipdata_->domain.setDomainChangeStack(std::vector<HighsDomainChange>());
      search.resetLocalDomain();

      mipdata_->domain.clearChangedCols();
    }

    // remove the iteration limit when installing a new node
    mipdata_->lp.setIterationLimit();

    // loop to install the next node for the search
    while (!mipdata_->nodequeue.empty()) {
      // printf("popping node from nodequeue (length = %lu)\n",
      // nodequeue.size());
      assert(!search.hasNode());
      search.installNode(mipdata_->nodequeue.popBestNode());

      assert(search.hasNode());

      // set the current basis if available
      if (basis) {
        mipdata_->lp.setStoredBasis(basis);
        mipdata_->lp.recoverBasis();
      }

      // we evaluate the node directly here instead of performing a dive
      // because we first want to check if the node is not fathomed due to
      // new global information before we perform separation rounds for the node
      search.evaluateNode();

      // if the node was pruned we remove it from the search and install the
      // next node from the queue
      if (search.currentNodePruned()) {
        search.backtrack();
        ++mipdata_->num_leaves;
        ++mipdata_->num_nodes;
        search.flushStatistics();
        mipdata_->lower_bound = std::min(
            mipdata_->upper_bound, mipdata_->nodequeue.getBestLowerBound());
        continue;
      }

      // the node is still not fathomed, so perform separation
      sepa.separate(search.getLocalDomain());

      // after separation we store the new basis and proceed with the outer loop
      // to perform a dive from this node
      if (mipdata_->lp.getStatus() != HighsLpRelaxation::Status::Error &&
          mipdata_->lp.getStatus() != HighsLpRelaxation::Status::NotSet)
        mipdata_->lp.storeBasis();

      basis = mipdata_->lp.getStoredBasis();

      break;
    }
  }

  mipdata_->timer.stop(mipdata_->timer.solve_clock);
  mipdata_->printDisplayLine();
}