#pragma once

#include "core/destination_ticket_card_deck.hpp"
#include "core/game_initialiser.hpp"
#include "core/map.hpp"
#include "core/player_state.hpp"
#include "core/train_car_card_deck.hpp"
#include "eval/player_report.hpp"
#include "strat/player_strategy.forward.hpp"
#include "turn/turn_decision.hpp"
#include <cstdint>

class Game {
public:
  TrainCarCardDeck trainCarCardDeck;
  DestinationTicketCardDeck destinationTicketCardDeck;
  std::vector<PlayerState> playerStates;
  std::vector<std::unique_ptr<PlayerStrategy>> playerStrategies;
  Map map;
  uint8_t playerIdx;

  Game(std::vector<std::unique_ptr<PlayerStrategy>> playerStrategies);

  // Returns true when the game is still in progress.
  bool run();

  std::vector<PlayerReport> makePlayerReports();

private:
  GameInitialiser gameInitialiser;
  AsyncGeneratorPtr<TurnDecision> pendingTurnDecision;
  AsyncGeneratorPtr<uint8_t> pendingDestinationTicketsChoiceDecision;
  std::array<DestinationTicketCard, 3> destinationTicketsChoiceDecisionCards;
  AsyncGeneratorPtr<DrawOneFaceUpTrainCarCardDecision::SecondAction>
      pendingDrawSecondTrainCarCardDecision;
  int8_t numTurnsUntilEnd{-1};

  void passTurn();
  void checkIfGameIsFinishing();

  void processDrawTrainCarCardsDecision(const DrawTrainCarCardsDecision &d);
  void processClaimRouteDecision(const ClaimRouteDecision &d);
  void processDrawDestinationTicketsDecision(
      const DrawDestinationTicketsDecision &d);
  void processBuildTrainStationDecision(const BuildTrainStationDecision &d);
  void processDrawTwoFaceDownTrainCarCardsDecision(
      const DrawTwoFaceDownTrainCarCardsDecision &d);
  void processDrawOneFaceUpLocomotiveCardDecision(
      const DrawOneFaceUpLocomotiveCardDecision &d);
  void processDrawOneFaceUpTrainCarCardDecision(
      const DrawOneFaceUpTrainCarCardDecision &d);
  void processDrawSecondFaceUpTrainCarCardDecision(
      const DrawOneFaceUpTrainCarCardDecision::
          DrawSecondFaceUpTrainCarCardDecision &d);
  void processDrawSecondFaceDownTrainCarCardDecision(
      const DrawOneFaceUpTrainCarCardDecision::
          DrawSecondFaceDownTrainCarCardDecision &d);

  bool awaitPendingDestinationTicketsChoiceDecision();
  bool awaitPendingDrawSecondTrainCarCardDecision();

  friend GameInitialiser;
  void handleThreeLocomotivesInFaceUpPile();
};