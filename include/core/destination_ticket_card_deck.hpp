#pragma once

#include <array>
#include "core/destination_ticket_card.hpp"
#include "util/rng.hpp"

constexpr const int NumRegularDestinationTicketCards = 40;
constexpr const int NumLongDestinationTicketCards = 6;

constexpr std::array<DestinationTicketCard, NumRegularDestinationTicketCards>
    standardRegularCards{
        DestinationTicketCard{
            .city1 = cityIdx("Frankfurt"),
            .city2 = cityIdx("København"),
            .points = 5,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Marseille"),
            .city2 = cityIdx("Essen"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Smolensk"),
            .city2 = cityIdx("Rostov"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Kyïv"),
            .city2 = cityIdx("Sochi"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Paris"),
            .city2 = cityIdx("Zágráb"),
            .points = 7,
        },
        DestinationTicketCard{
            .city1 = cityIdx("London"),
            .city2 = cityIdx("Wien"),
            .points = 10,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Riga"),
            .city2 = cityIdx("Bucuresti"),
            .points = 10,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Berlin"),
            .city2 = cityIdx("Bucuresti"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Berlin"),
            .city2 = cityIdx("Moskva"),
            .points = 12,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Erzurum"),
            .city2 = cityIdx("Rostov"),
            .points = 5,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Brest"),
            .city2 = cityIdx("Marseille"),
            .points = 7,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Venezia"),
            .city2 = cityIdx("Constantinople"),
            .points = 10,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Stockholm"),
            .city2 = cityIdx("Wien"),
            .points = 11,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Sarajevo"),
            .city2 = cityIdx("Sevastopol"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Kyïv"),
            .city2 = cityIdx("Petrograd"),
            .points = 6,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Paris"),
            .city2 = cityIdx("Wien"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Berlin"),
            .city2 = cityIdx("Roma"),
            .points = 9,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Madrid"),
            .city2 = cityIdx("Dieppe"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Zágráb"),
            .city2 = cityIdx("Brindisi"),
            .points = 6,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Bruxelles"),
            .city2 = cityIdx("Danzig"),
            .points = 9,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Athína"),
            .city2 = cityIdx("Wilno"),
            .points = 11,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Zürich"),
            .city2 = cityIdx("Brindisi"),
            .points = 6,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Essen"),
            .city2 = cityIdx("Kyïv"),
            .points = 10,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Zürich"),
            .city2 = cityIdx("Budapest"),
            .points = 6,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Barcelona"),
            .city2 = cityIdx("München"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Palermo"),
            .city2 = cityIdx("Constantinople"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Athína"),
            .city2 = cityIdx("Angora"),
            .points = 5,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Warszawa"),
            .city2 = cityIdx("Smolensk"),
            .points = 6,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Edinburgh"),
            .city2 = cityIdx("Paris"),
            .points = 7,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Budapest"),
            .city2 = cityIdx("Sofia"),
            .points = 5,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Barcelona"),
            .city2 = cityIdx("Bruxelles"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Sofia"),
            .city2 = cityIdx("Smyrna"),
            .points = 5,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Madrid"),
            .city2 = cityIdx("Zürich"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Amsterdam"),
            .city2 = cityIdx("Wilno"),
            .points = 12,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Brest"),
            .city2 = cityIdx("Venezia"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Roma"),
            .city2 = cityIdx("Smyrna"),
            .points = 8,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Angora"),
            .city2 = cityIdx("Kharkov"),
            .points = 10,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Frankfurt"),
            .city2 = cityIdx("Smolensk"),
            .points = 13,
        },
        DestinationTicketCard{
            .city1 = cityIdx("London"),
            .city2 = cityIdx("Berlin"),
            .points = 7,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Amsterdam"),
            .city2 = cityIdx("Pamplona"),
            .points = 7,
        },
    };

constexpr std::array<DestinationTicketCard, NumLongDestinationTicketCards>
    standardLongCards{
        DestinationTicketCard{
            .city1 = cityIdx("Edinburgh"),
            .city2 = cityIdx("Athína"),
            .points = 21,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Brest"),
            .city2 = cityIdx("Petrograd"),
            .points = 20,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Cádiz"),
            .city2 = cityIdx("Stockholm"),
            .points = 21,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Lisboa"),
            .city2 = cityIdx("Danzig"),
            .points = 20,
        },
        DestinationTicketCard{
            .city1 = cityIdx("København"),
            .city2 = cityIdx("Erzurum"),
            .points = 21,
        },
        DestinationTicketCard{
            .city1 = cityIdx("Palermo"),
            .city2 = cityIdx("Moskva"),
            .points = 20,
        },
    };

struct DestinationTicketCardDeck {
  // Cards in face-down pile: [0, N).
  // Drawn cards: [N, 40). Don't read from here.
  std::array<DestinationTicketCard, NumRegularDestinationTicketCards>
      regularCards;
  uint8_t n;

  // Undrawn cards: [0, M).
  // Drawn cards: [M, 6). Don't read from here.
  std::array<DestinationTicketCard, NumLongDestinationTicketCards> longCards;
  uint8_t m;

  DestinationTicketCardDeck();

  DestinationTicketCard drawRegularCard();
  DestinationTicketCard drawLongCard();
  void returnRegularCard(DestinationTicketCard card);
};