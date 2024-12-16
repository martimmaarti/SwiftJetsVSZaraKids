#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

// Base class: Card
class Card {
protected:
    string name;
    string type;

public:
    Card(const string& name, const string& type)
        : name(name), type(type) {}

    virtual void display() const {
        cout << "Card: " << name << ", Type: " << type << endl;
    }

    virtual void applyEffect(int& currentMoralitos, int& opponentMoralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const = 0;

    virtual ~Card() = default;
};

// Derived class: Action Card
class ActionCard : public Card {
private:
    int attackThreshold;
    int healThreshold;

public:
    ActionCard(int attack, int heal)
        : Card("Action", "Attack/Healing"), attackThreshold(attack), healThreshold(heal) {}

    void display() const override {
        Card::display();
        cout << "Attack Threshold: " << attackThreshold
            << ", Heal Threshold: " << healThreshold << endl;
    }

    void applyEffect(int& currentMoralitos, int& opponentMoralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const override {
        char choice;
        char useArbolito;
         do {
             cout << "Choose action (A: Attack, H: Heal): ";
             cin >> choice;
             if (choice == 'A' || choice == 'a') {
                 if (arbolitos > 0) {
                     cout << "You have " << arbolitos << " arbolitos. Use one to succeed the attack? (Y/N): ";
                     cin >> useArbolito;
                     if (useArbolito == 'Y' || useArbolito == 'y') {
                         arbolitos--;
                         opponentMoralitos += attackThreshold;
                         cout << "Healing successful using an arbolito! Remaining arbolitos: " << arbolitos << endl;
                     }
                     else if (useArbolito == 'N' || useArbolito == 'n') {
                         if (diceRoll >= attackThreshold) {
                             opponentMoralitos -= attackThreshold;
                             cout << "Attack successful! Dice roll: " << diceRoll << endl << " -" << attackThreshold << " moralitos to opponent." << endl;
                         }
                         else {
                             cout << "Attack failed! Dice roll: " << diceRoll << endl;
                         }
                     }
                 }
                 else {
                     if (diceRoll >= attackThreshold) {
                         opponentMoralitos -= attackThreshold;
                         cout << "Attack successful! Dice roll: " << diceRoll << endl << " -" << attackThreshold << " moralitos to opponent." << endl;
                     }
                     else {
                         cout << "Attack failed! Dice roll: " << diceRoll << endl;
                     }
                 }
             }
             else if (choice == 'H' || choice == 'h') {
                 if (arbolitos > 0) {
                     cout << "You have " << arbolitos << " arbolitos. Use one to succeed the healing? (Y/N): ";
                     cin >> useArbolito;
                     if (useArbolito == 'Y' || useArbolito == 'y') {
                         arbolitos--;
                         currentMoralitos += healThreshold;
                         cout << "Healing successful using an arbolito! Remaining arbolitos: " << arbolitos << endl;
                     }
                     else if (useArbolito == 'N' || useArbolito == 'n') {
                         if (diceRoll >= healThreshold) {
                             currentMoralitos -= healThreshold;
                             cout << "Healing successful! Dice roll: " << diceRoll << endl << " +" << healThreshold << " moralitos to self." << endl;
                         }
                         else {
                             cout << "Healing failed! Dice roll: " << diceRoll << endl;
                         }
                     }
                 }
                 else {
                     if (diceRoll >= healThreshold) {
                         currentMoralitos -= healThreshold;
                         cout << "Healing successful! Dice roll: " << diceRoll << endl << " +" << healThreshold << " moralitos to self." << endl;
                     }
                     else {
                         cout << "Healing failed! Dice roll: " << diceRoll << endl;
                     }
                 }
             }
             else {
                cout << "Invalid choice, try again." << endl;
             }
         } while (choice != 'a' && choice != 'A' && choice != 'h' && choice != 'H');
    }
};

// Derived class: Event Card
class EventCard : public Card {
private:
    int effect;

public:
    EventCard(const string& name, int effect)
        : Card(name, "Event"), effect(effect) {}

    void display() const override {
        Card::display();
        cout << "Effect on moralitos: " << effect << endl;
    }

    void applyEffect(int& currentMoralitos, int& opponentMoralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const override {
        currentMoralitos += effect;
        cout << "Event effect applied! Moralitos change: " << effect << endl;
    }
};

// Derived class: Arbolito Card
class ArbolitoCard : public Card {
public:
    ArbolitoCard()
        : Card("Arbolito", "Renewable Energy") {}

    void display() const override {
        Card::display();
        cout << "Effect: +1 arbolito" << endl;
    }

    void applyEffect(int& currentMoralitos, int& opponentMoralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const override {
        arbolitos++;
        cout << "Arbolito gained! Total arbolitos: " << arbolitos << endl;
    }
};

// Deck class
class Deck {
private:
    Card** cards;
    int capacity;
    int size;

    void shuffleDeck() {
        for (int i = 0; i < size; ++i) {
            int j = rand() % size;
            swap(cards[i], cards[j]);
        }
    }

public:
    Deck(int capacity) : capacity(capacity), size(0) {
        cards = new Card * [capacity];
    }

    void addCard(Card* card) {
        if (size < capacity) {
            cards[size++] = card;
        }
    }

    Card* drawCard() {
        if (size == 0) return nullptr;
        return cards[--size];
    }

    bool isEmpty() const {
        return size == 0;
    }

    void shuffle() {
        shuffleDeck();
    }

    ~Deck() {
        for (int i = 0; i < size; ++i) {
            delete cards[i];
        }
        delete[] cards;
    }
};

// Player class
class Player {
public:
    string name;
    int moralitos;
    int arbolitos;

    Player(const string& name, int moralitos = 20)
        : name(name), moralitos(moralitos), arbolitos(0) {}

    void displayStatus() const {
        cout << name << " - Moralitos: " << moralitos << ", Arbolitos: " << arbolitos << endl;
    }
};

// Dice roll function
int rollDice() {
    return rand() % 6 + 1; // Generates a number between 1 and 6
}

void displayPlayers(const Player& player1, const Player& player2) {
    player1.displayStatus();
    player2.displayStatus();
}

// Main function
int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generator

    // Create players
    Player player1("Taylor Swift");
    Player player2("Amancio Ortega");

    // Create and shuffle the deck
    Deck deck(40);

    // Add action cards
    for (int attack = 1; attack <= 5; ++attack) {
        for (int heal = 1; heal <= 5; ++heal) {
            deck.addCard(new ActionCard(attack, heal));
        }
    }

    // Add event cards
    deck.addCard(new EventCard("Greta Thunberg accuses of greenwashing", -3));
    deck.addCard(new EventCard("Hacienda finds tax evasion", -2));
    deck.addCard(new EventCard("Hacienda finds no issues", +1));
    deck.addCard(new EventCard("Beyonce inspires donation", +3));
    deck.addCard(new EventCard("Jeque arabe gives free gasoline", -5));
    deck.addCard(new EventCard("Donald Trump invites you to a party, and you make contacts that boost your finances", -4));
    deck.addCard(new EventCard("You take your private jet, and a jet-tracking Twitter account exposes you", -1));
    deck.addCard(new EventCard("You fall in love with tea and start sponsoring eco-friendly brands offering the best quality tea", +2));
    deck.addCard(new EventCard("You meet a kind woman with a small business and decide to help her resist a multinational's monopoly", +5));
    deck.addCard(new EventCard("You offer scholarships to underprivileged students to enroll at CITM, in exchange for participating in a tree-planting charity event", +4));

    // Add arbolito cards
    for (int i = 0; i < 5; ++i) {
        deck.addCard(new ArbolitoCard());
    }

    deck.shuffle();

    // Game loop
    Player* currentPlayer = &player1;
    Player* opponent = &player2;
    int turn = 1;

    cout << "   _____         _  __ _       _      _        __      _______   ______               _  ___     _     \n";
    cout << "  / ____|       (_)/ _| |     | |    | |       \\ \\    / / ____| |___  /              | |/ (_)   | |    \n";
    cout << " | (_____      ___| |_| |_    | | ___| |_ ___   \\ \\  / / (___      / / __ _ _ __ __ _| ' / _  __| |___ \n";
    cout << "  \\___ \\ \\ /\\ / / |  _| __|   | |/ _ \\ __/ __|   \\ \\/ / \\___ \\    / / / _` | '__/ _` |  < | |/ _` / __|\n";
    cout << "  ____) \\ V  V /| | | | || |__| |  __/ |_\\__ \\    \\  /  ____) |  / /_| (_| | | | (_| | . \\| | (_| \\__ \\\n";
    cout << " |_____/ \\_/\\_/ |_|_|  \\__\\____/ \\___|\\__|___/     \\/  |_____/  /_____\\__,_|_|  \\__,_|_|\\_\\_|\\__,_|___/\n";
    cout << "                                                                                                       \n";
    cout << "                                                                                                       \n";

    cout << "This turn-based game pits two players against each other in a strategic battle to deplete their opponent's " << endl;
    cout << "'moralitos' (life points) and claim victory. You start with 20 moralitos. The game ends when one player's" << endl;
    cout << "moralitos reach zero or all 40 cards are drawn.\nEach turn, players draw one of three card types :" << endl;
    cout << "   - Attack / Healing(25 cards) : Damage your opponent or restore your own moralitos.In order to apply this" << endl;
    cout << "       effect, you must roll the dice and the number has to be higher than the amount of moralitos you heal / attack" << endl;
    cout << "   - Events(10 cards) : Trigger random scenarios with varying effects that heals or damages you." << endl;
    cout << "   - Arbolitos(5 cards) : Rare and versatile, this card can funtion as a secure for the dice roll(before its roll)." << endl;
    cout << "This card can also help you winning the game." << endl;
    cout << "Victory is decided through the following hierarchy:" << endl;
    cout << "   Moralitos: Deplete your opponent's life points." << endl;
    cout << "   Arbolitos: If turns end, the player with more arbolitos wins." << endl;
    cout << "   Remaining Life : As a final tiebreaker, the player with higher moralitos prevails." << endl;
    cout << "Get ready to strategize, adapt, and outwit your opponent.Let the game begin!" << endl;
    while (!deck.isEmpty() && player1.moralitos > 0 && player2.moralitos > 0) {
        cout << endl << "--- Turn number " << turn << " | Turn of " << currentPlayer->name << " ---" << endl;
        displayPlayers(player1, player2);

        Card* card = deck.drawCard();
        if (card) {
            card->display();

            int diceRoll = rollDice();
            bool wildcardUsed = false;

            // Apply card effect
            card->applyEffect(currentPlayer->moralitos, opponent->moralitos, currentPlayer->arbolitos, diceRoll, wildcardUsed);

            displayPlayers(player1, player2);
            delete card;
        }

        swap(currentPlayer, opponent);
        turn++;

        char ready;
        cout << "Are you ready for the next turn? Write anything and press enter to continue" << endl;
        cin >> ready;
    }

    // Determine winner
    cout << "\n--- Game Over ---" << endl;
    if (player1.moralitos <= 0) {
        cout << player2.name << " wins by depleting " << player1.name << "'s moralitos!" << endl;
    } else if (player2.moralitos <= 0) {
        cout << player1.name << " wins by depleting " << player2.name << "'s moralitos!" << endl;
    } else {
        cout << "Deck exhausted! Deciding winner by arbolitos or remaining moralitos..." << endl;
        if (player1.arbolitos > player2.arbolitos) {
            cout << player1.name << " wins by having more arbolitos!" << endl;
        } else if (player2.arbolitos > player1.arbolitos) {
            cout << player2.name << " wins by having more arbolitos!" << endl;
        } else if (player1.moralitos > player2.moralitos) {
            cout << player1.name << " wins by having more moralitos!" << endl;
        } else if (player2.moralitos > player1.moralitos) {
            cout << player2.name << " wins by having more moralitos!" << endl;
        } else {
            cout << "It's a tie!" << endl;
        }
    }

    return 0;
}

