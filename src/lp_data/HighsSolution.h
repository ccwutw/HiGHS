/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                       */
/*    This file is part of the HiGHS linear optimization suite           */
/*                                                                       */
/*    Written and engineered 2008-2019 at the University of Edinburgh    */
/*                                                                       */
/*    Available as open-source under the MIT License                     */
/*                                                                       */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**@file lp_data/HighsSolution.h
 * @brief Class-independent utilities for HiGHS
 * @author Julian Hall, Ivet Galabova, Qi Huangfu and Michael Feldmeier
 */
#ifndef LP_DATA_HIGHSSOLUTION_H_
#define LP_DATA_HIGHSSOLUTION_H_

#include <vector>
#include <string>
//
#include "lp_data/HighsLp.h"
#include "lp_data/HighsModelObject.h"
#include "ipm/IpxSolution.h"
#include "lp_data/HighsOptions.h"
#ifdef IPX_ON
#include "ipm/IpxStatus.h"
#include "ipm/ipx/include/ipx_status.h"
#include "ipm/ipx/src/lp_solver.h"
#endif

struct HighsPrimalDualErrors {
  int num_nonzero_basic_duals;
  int num_large_nonzero_basic_duals;
  double max_nonzero_basic_dual;
  double sum_nonzero_basic_duals;
  int num_off_bound_nonbasic;
  double max_off_bound_nonbasic;
  double sum_off_bound_nonbasic;
  int num_primal_residual;
  double max_primal_residual;
  double sum_primal_residual;
  int num_dual_residual;
  double max_dual_residual;
  double sum_dual_residual;
};

// Analyse the HiGHS basic solution of the unscaled LP in a HighsModelObject instance
HighsStatus analyseHighsBasicSolution(const HighsModelObject& highs_model_object,
				      const string message);

// Analyse the HiGHS basic solution of the given LP. Currently only
// used with the unscaled LP, but would work just as well with a
// scaled LP. The solution data passed in is assumed to be correct and
// it is checked as much as possible. Inconsistencies are reported,
// but not corrected.
HighsStatus analyseHighsBasicSolution(const HighsLp& lp,
				      const HighsBasis& basis,
				      const HighsSolution& solution,
				      const HighsModelStatus model_status,
				      const HighsSolutionParams& solution_params,
				      const string message);

// As above, but with report_level
HighsStatus analyseHighsBasicSolution(const HighsLp& lp,
				      const HighsBasis& basis,
				      const HighsSolution& solution,
				      const HighsModelStatus model_status,
				      const HighsSolutionParams& solution_params,
				      const string message,
				      const int report_level);

void getPrimalDualInfeasibilities(const HighsLp& lp,
				  const HighsBasis& basis,
				  const HighsSolution& solution,
				  HighsSolutionParams& solution_params);

void getPrimalDualInfeasibilitiesAndErrors(const HighsLp& lp,
					   const HighsBasis& basis,
					   const HighsSolution& solution,
					   HighsSolutionParams& solution_params,
					   HighsPrimalDualErrors& primal_dual_errors,
					   double& primal_objective_value,
					   double& dual_objective_value,
					   const int report_level);
bool analyseVarBasicSolution(
			bool report,
			const double primal_feasibility_tolerance,
			const double dual_feasibility_tolerance,
			const HighsBasisStatus status,
			const double lower,
			const double upper,
			const double value,
			const double dual,
			int& num_non_basic_var,
			int& num_basic_var,
			double& off_bound_nonbasic,
			double& primal_infeasibility,
			double& dual_infeasibility);

#ifdef HiGHSDEV
void analyseSimplexAndHighsSolutionDifferences(const HighsModelObject& highs_model_object);
#endif

#ifdef IPX_ON
HighsStatus ipxToHighsBasicSolution(const HighsLp& lp,
				    const std::vector<double>& rhs,
				    const std::vector<char>& constraint_type,
				    const IpxSolution& ipx_solution,
				    HighsBasis& highs_basis,
				    HighsSolution& highs_solution);
#endif    

std::string iterationsToString(const HighsSolutionParams& solution_params);

void invalidateModelStatusAndSolutionStatusParams(HighsModelStatus& unscaled_model_status,
						  HighsModelStatus& scaled_model_status,
						  HighsSolutionParams& solution_params);
void invalidateSolutionParams(HighsSolutionParams& solution_params);
void invalidateSolutionIterationCountAndObjectiveParams(HighsSolutionParams& solution_params);
void invalidateSolutionStatusParams(HighsSolutionParams& solution_params);
void invalidateSolutionInfeasibilityParams(HighsSolutionParams& solution_params);

bool equalSolutionParams(const HighsSolutionParams& solution_params0,
			 const HighsSolutionParams& solution_params1);
bool equalSolutionIterationCountAndObjectiveParams(const HighsSolutionParams& solution_params0,
						   const HighsSolutionParams& solution_params1);
bool equalSolutionStatusParams(const HighsSolutionParams& solution_params0,
			       const HighsSolutionParams& solution_params1);
bool equalSolutionInfeasibilityParams(const HighsSolutionParams& solution_params0,
				      const HighsSolutionParams& solution_params1);

void initialiseSolutionParams(HighsSolutionParams& solution_params, const HighsOptions& options);

void copySolutionIterationCountAndObjectiveParams(const HighsSolutionParams& from_solution_params, 
						  HighsSolutionParams& to_solution_params);

void copyFromSolutionParams(HighsInfo& highs_info, const HighsSolutionParams& solution_params);

#endif  // LP_DATA_HIGHSSOLUTION_H_
