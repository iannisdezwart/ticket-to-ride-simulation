#include "eval/completed_routes.hpp"

void dfs(int playerIdx, const std::array<int8_t, NumRoutes> &claim,
         const Routes &routes, std::bitset<NumCities> &cVis, int cCur) {
  cVis[cCur] = true;

  for (auto i = 0; i < NumRoutes; i++) {
    auto ours = claim[i] == playerIdx;
    if (!ours) {
      continue;
    }
    auto c1 = routes.city1[i];
    auto c2 = routes.city2[i];
    if ((c1 != cCur && c2 != cCur) || (cVis[c1] && cVis[c2])) {
      continue;
    }
    auto cNew = c1 ^ c2 ^ cCur;
    dfs(playerIdx, claim, routes, cVis, cNew);
  }
};

bool pathExists(int city1, int city2, int playerIdx,
                const std::array<int8_t, NumRoutes> &claim,
                const Routes &routes) {
  std::bitset<NumCities> cVis;
  dfs(playerIdx, claim, routes, cVis, city1);
  return cVis[city2];
}

void directPathPass(int playerIdx, const Map &map,
                    const std::vector<DestinationTicketCard> &cards,
                    CompletedRoutesReport &rep) {
  for (auto i = 0uz; i < cards.size(); i++) {
    const auto &c = cards[i];
    if (pathExists(c.city1, c.city2, playerIdx, map.routes.claim, map.routes)) {
      rep.completed[i] = true;
      rep.points += c.points;
    }
  }
}

std::vector<std::array<int8_t, NumRoutes>>
getClaimPermutations(int playerIdx, const Map &map, int cityIdx,
                     std::array<int8_t, NumRoutes> base) {
  if (cityIdx == NumCities) {
    return std::vector<std::array<int8_t, NumRoutes>>{base};
  }
  if (map.trainStationClaims[cityIdx] != playerIdx) {
    return getClaimPermutations(playerIdx, map, cityIdx + 1, std::move(base));
  }

  std::vector<std::array<int8_t, NumRoutes>> altClaims;
  for (auto i = 0; i < NumRoutes; i++) {
    auto claim = map.routes.claim[i];
    if (claim == -1 || claim == playerIdx) {
      continue;
    }
    auto c1 = map.routes.city1[i];
    auto c2 = map.routes.city2[i];
    if (c1 != cityIdx && c2 != cityIdx) {
      continue;
    }
    auto altBase = base;
    altBase[i] = playerIdx;
    auto res =
        getClaimPermutations(playerIdx, map, cityIdx + 1, std::move(altBase));
    std::copy(res.begin(), res.end(), std::back_inserter(altClaims));
  }
  return altClaims;
}

void stationPass(int playerIdx, const Map &map,
                 const std::vector<DestinationTicketCard> &cards,
                 CompletedRoutesReport &rep) {
  if (std::count_if(
          map.trainStationClaims.begin(), map.trainStationClaims.end(),
          [playerIdx](const auto &x) { return x == playerIdx; }) == 0) {
    return;
  }

  auto bestCombination = CompletedRoutesReport{
      .completed = std::vector<bool>(cards.size()),
      .points = 0,
  };

  for (const auto &claim :
       getClaimPermutations(playerIdx, map, 0, map.routes.claim)) {
    auto cur = CompletedRoutesReport{
        .completed = std::vector<bool>(cards.size()),
        .points = 0,
    };

    for (auto i = 0uz; i < cards.size(); i++) {
      if (rep.completed[i]) {
        continue;
      }

      const auto &c = cards[i];
      if (pathExists(c.city1, c.city2, playerIdx, claim, map.routes)) {
        cur.completed[i] = true;
        cur.points += c.points;
      }
    }

    if (cur.points > bestCombination.points) {
      bestCombination = std::move(cur);
    }
  }

  for (auto i = 0uz; i < rep.completed.size(); i++) {
    rep.completed[i] = rep.completed[i] || bestCombination.completed[i];
  }
  rep.points += bestCombination.points;
}

CompletedRoutesReport
evalCompletedRoutes(int playerIdx, const Map &map,
                    std::vector<DestinationTicketCard> cards) {
  std::sort(cards.begin(), cards.end(),
            [](const auto &a, const auto &b) { return a.points > b.points; });
  auto rep = CompletedRoutesReport{
      .completed = std::vector<bool>(cards.size()),
      .points = 0,
  };

  directPathPass(playerIdx, map, cards, rep);
  stationPass(playerIdx, map, cards, rep);

  return rep;
}