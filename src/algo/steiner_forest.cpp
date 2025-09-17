#include "algo/steiner_forest.hpp"
#include <ortools/linear_solver/linear_solver.h>

using operations_research::MPSolver;
using operations_research::MPVariable;

std::vector<uint8_t>
steinerForest(const Map &map, uint8_t playerIdx,
              std::vector<std::pair<uint8_t, uint8_t>> &paths) {
  std::transform(paths.begin(), paths.end(), paths.begin(),
                 [](const auto &path) {
                   const auto &[cityIdx1, cityIdx2] = path;
                   if (cityIdx1 < cityIdx2) {
                     return path;
                   }
                   return std::make_pair(cityIdx2, cityIdx1);
                 });

  auto solver = MPSolver(
      "Solver",
      MPSolver::OptimizationProblemType::SCIP_MIXED_INTEGER_PROGRAMMING);

  // X variables: whether or not an route is used in the solution.
  auto x = std::vector<MPVariable *>(NumRoutes);
  for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
    auto city1 = standardCities[map.routes.city1[routeIdx]];
    auto city2 = standardCities[map.routes.city2[routeIdx]];
    auto claim = map.routes.claim[routeIdx];
    auto ub = (claim == -1 || claim == playerIdx) ? 1.0 : 0.0;
    auto oss = std::ostringstream();
    oss << "x_" << city1 << "_" << city2;
    x[routeIdx] = solver.MakeIntVar(0.0, ub, oss.str());
  }

  // F variables: flow for each route (bidirectional), by path.
  auto f = std::vector<std::vector<MPVariable *>>(
      paths.size(), std::vector<MPVariable *>(NumRoutes * 2));
  for (auto pathIdx = 0uz; pathIdx < paths.size(); pathIdx++) {
    const auto &[pSource, pSink] = paths[pathIdx];
    auto pathSource = standardCities[map.routes.city1[pSource]];
    auto pathSink = standardCities[map.routes.city2[pSink]];
    for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
      auto routeFrom = standardCities[map.routes.city1[routeIdx]];
      auto routeTo = standardCities[map.routes.city2[routeIdx]];
      // Forward direction.
      {
        auto oss = std::ostringstream();
        oss << "f_" << pathSource << "_" << pathSink << "_" << routeFrom << "_"
            << routeTo;
        f[pathIdx][routeIdx * 2] = solver.MakeNumVar(0.0, 1.0, oss.str());
      }
      // Backward direction.
      {
        auto oss = std::ostringstream();
        oss << "f_" << pathSource << "_" << pathSink << "_" << routeTo << "_"
            << routeFrom;
        f[pathIdx][routeIdx * 2 + 1] = solver.MakeNumVar(0.0, 1.0, oss.str());
      }
    }
  }

  // Flow conservation in each city, per path.
  for (auto pathIdx = 0uz; pathIdx < paths.size(); pathIdx++) {
    const auto &[pSource, pSink] = paths[pathIdx];
    for (auto cityIdx = 0; cityIdx < NumCities; cityIdx++) {
      auto val = cityIdx == pSource ? 1.0 : (cityIdx == pSink ? -1.0 : 0.0);
      auto cons = solver.MakeRowConstraint(val, val);
      for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
        auto routeFrom = map.routes.city1[routeIdx];
        auto routeTo = map.routes.city2[routeIdx];
        if (cityIdx == routeFrom) {
          cons->SetCoefficient(f[pathIdx][routeIdx * 2], 1.0);
          cons->SetCoefficient(f[pathIdx][routeIdx * 2 + 1], -1.0);
        } else if (cityIdx == routeTo) {
          cons->SetCoefficient(f[pathIdx][routeIdx * 2], -1.0);
          cons->SetCoefficient(f[pathIdx][routeIdx * 2 + 1], 1.0);
        }
      }
    }
  }

  // Flow selection per path.
  for (auto pathIdx = 0uz; pathIdx < paths.size(); pathIdx++) {
    for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
      auto consFwd = solver.MakeRowConstraint(-solver.infinity(), 0.0);
      consFwd->SetCoefficient(f[pathIdx][routeIdx * 2], 1.0);
      consFwd->SetCoefficient(x[routeIdx], -1.0);
      auto consBwd = solver.MakeRowConstraint(-solver.infinity(), 0.0);
      consBwd->SetCoefficient(f[pathIdx][routeIdx * 2 + 1], 1.0);
      consBwd->SetCoefficient(x[routeIdx], -1.0);
    }
  }

  // Objective: minimise ΣX * ΣW
  auto obj = solver.MutableObjective();
  for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
    auto claim = map.routes.claim[routeIdx];
    if (claim != -1 && claim != playerIdx) {
      continue;
    }
    // TODO: Use heuristic.
    auto weight = claim == -1 ? map.routes.length[routeIdx] : uint8_t(0);
    obj->SetCoefficient(x[routeIdx], weight);
  }
  obj->SetMinimization();

  // Solve.
  auto status = solver.Solve();
  auto path = std::vector<uint8_t>();
  if (status != MPSolver::OPTIMAL && status != MPSolver::FEASIBLE) {
    return path;
  }

  for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
    if (x[routeIdx]->solution_value() > 0.5 &&
        map.routes.claim[routeIdx] != playerIdx) {
      path.push_back(routeIdx);
    }
  }

  return path;
}