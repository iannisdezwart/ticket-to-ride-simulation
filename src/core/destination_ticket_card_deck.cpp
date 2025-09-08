#include "core/destination_ticket_card_deck.hpp"

DestinationTicketCardDeck::DestinationTicketCardDeck()
    : regularCards(standardRegularCards), n(NumRegularDestinationTicketCards),
      longCards(standardLongCards), m(NumLongDestinationTicketCards)

{
  std::shuffle(regularCards.begin(), regularCards.end(), rng());
  std::shuffle(longCards.begin(), longCards.end(), rng());
}

DestinationTicketCard DestinationTicketCardDeck::drawRegularCard() {
  if (n == 0) {
    throw std::runtime_error("Draw pile is empty.");
  }
  n--;
  return regularCards[n];
}

DestinationTicketCard DestinationTicketCardDeck::drawLongCard() {
  if (m == 0) {
    throw std::runtime_error("Draw pile is empty.");
  }
  m--;
  return longCards[m];
}

void DestinationTicketCardDeck::returnRegularCard(DestinationTicketCard card) {
  if (n >= NumRegularDestinationTicketCards) {
    throw std::logic_error("More cards returned than drawn");
  }
  std::move(regularCards.begin(), regularCards.end(), regularCards.begin() + 1);
  regularCards[0] = card;
  n++;
}