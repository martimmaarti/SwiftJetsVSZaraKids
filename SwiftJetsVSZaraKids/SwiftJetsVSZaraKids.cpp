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

    virtual void applyEffect(int& moralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const = 0;

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

    void applyEffect(int& moralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const override {
        char choice;
        do {
            cout << "Choose action (A: Attack, H: Heal): ";
            cin >> choice;
            if (choice == 'A' || choice == 'a') {
                if (diceRoll >= attackThreshold) {
                    moralitos -= attackThreshold;
                    cout << "Attack successful! Dice roll: " << diceRoll << endl << " -" << attackThreshold << " moralitos to opponent." << endl;
                }
                else {
                    cout << "Attack failed! Dice roll: " << diceRoll << endl;
                }
            }
            else if (choice == 'H' || choice == 'h') {
                if (diceRoll >= healThreshold) {
                    moralitos += healThreshold;
                    cout << "Healing successful! Dice roll: " << diceRoll << endl << " +" << healThreshold << " moralitos to self." << endl;
                }
                else {
                    cout << "Healing failed! Dice roll: " << diceRoll << endl;
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

    void applyEffect(int& moralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const override {
        moralitos += effect;
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

    void applyEffect(int& moralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const override {
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

    Player(const string& name, int moralitos = 10)
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
    for (int attack = 1, heal = 1; attack <= 5; ++attack, ++heal) {
        deck.addCard(new ActionCard(attack, heal));
    }

    // Add event cards
    deck.addCard(new EventCard("Greta Thunberg accuses of greenwashing", -3));
    deck.addCard(new EventCard("Hacienda finds tax evasion", -2));
    deck.addCard(new EventCard("Hacienda finds no issues", +1));
    deck.addCard(new EventCard("Beyoncé inspires donation", +3));
    deck.addCard(new EventCard("Jeque árabe gives free gasoline", -5));

    // Add arbolito cards
    for (int i = 0; i < 5; ++i) {
        deck.addCard(new ArbolitoCard());
    }

    deck.shuffle();

    // Game loop
    Player* currentPlayer = &player1;
    Player* opponent = &player2;

    while (!deck.isEmpty() && player1.moralitos > 0 && player2.moralitos > 0) {
        cout << "\n--- Turn of " << currentPlayer->name << " ---" << endl;
        displayPlayers(player1, player2);

        Card* card = deck.drawCard();
        if (card) {
            card->display();

            int diceRoll = rollDice();
            bool wildcardUsed = false;
            card->applyEffect(opponent->moralitos, currentPlayer->arbolitos, diceRoll, wildcardUsed);

            delete card;
        }

        // Swap players
        swap(currentPlayer, opponent);
    }

    // Determine winner
    cout << "\n--- Game Over ---" << endl;
    if (player1.moralitos <= 0) {
        cout << player2.name << " wins by depleting moralitos!" << endl;
    }
    else if (player2.moralitos <= 0) {
        cout << player1.name << " wins by depleting moralitos!" << endl;
    }
    else {
        if (player1.arbolitos > player2.arbolitos) {
            cout << player1.name << " wins by having more arbolitos!" << endl;
        }
        else if (player2.arbolitos > player1.arbolitos) {
            cout << player2.name << " wins by having more arbolitos!" << endl;
        }
        else {
            cout << "It's a tie!" << endl;
        }
    }

    return 0;
}
