#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

// Base class: Card
class Card {
protected:
    string name;
    string type;

public:
    Card(const string& name, const string& type)
        : name(name), type(type) {} //constructor

    virtual void display() const {
        cout << "Card: " << name << ", Type: " << type << endl;
    } //muetra en pantalla la carta

    virtual void applyEffect(int& moralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const = 0;

    virtual ~Card() = default;
};

// Derived class: Attack/Healing Card
class ActionCard : public Card {
private:
    int attackThreshold;
    int healThreshold;

public:
    ActionCard(int attack, int heal) 
        : Card("Action", "Attack/Healing"), attackThreshold(attack), healThreshold(heal) {} //constructor

    void display() const override {
        Card::display();
        cout << "Attack Threshold: " << attackThreshold
            << ", Heal Threshold: " << healThreshold << endl;
    } //muestra en pantalla la carta

    void applyEffect(int& moralitos, int& arbolitos, int& diceRoll, bool& wildcardUsed) const override {
        char choice;
        do{
            cout << "Choose action (A: Attack, H: Heal): ";
            cin >> choice;
            if (choice == 'A' || choice == 'a') {
                if (diceRoll >= attackThreshold) {
                    moralitos -= attackThreshold;
                    cout << "Attack failed! Dice roll: " << diceRoll << endl;
                    cout << "Attack successful! -" << attackThreshold << " moralitos to opponent." << endl;
                }
                else {
                    cout << "Attack failed! Dice roll: " << diceRoll << endl;
                }
            } //si decide atacar
            else if (choice == 'H' || choice == 'h') {
                if (diceRoll >= healThreshold) {
                    moralitos += healThreshold;
                    cout << "Attack failed! Dice roll: " << diceRoll << endl;
                    cout << "Healing successful! +" << healThreshold << " moralitos to self." << endl;
                }
                else {
                    cout << "Healing failed! Dice roll: " << diceRoll << endl;
                }
            } //si decide atacar
            else {
                cout << "Invalid choice, try again." << endl;
            }
        } while (choice != 'a' || choice != 'A' || choice != 'h' || choice != 'H');
    }
};

// Derived class: Event Card
class EventCard : public Card {
private:
    int effect;

public:
    EventCard(const std::string& name, int effect)
        : Card(name, "Event"), effect(effect) {} //constructor

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
        : Card("Arbolito", "Renewable Energy") {} //constructor

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
    vector<Card*> cards;

public:
    void addCard(Card* card) {
        cards.push_back(card);
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    Card* drawCard() {
        if (cards.empty()) return nullptr;
        Card* card = cards.front();
        cards.erase(cards.begin());
        return card;
    }

    bool isEmpty() const {
        return cards.empty();
    }

    ~Deck() {
        for (Card* card : cards) {
            delete card;
        }
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

// Main function
int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generator

    // Create players
    Player player1("Taylor Swift");
    Player player2("Amancio Ortega");

    // Create and shuffle the deck
    Deck deck;

    // Add action cards
    for (int attack = 1, heal = 1; attack <= 5; ++attack, heal++) {
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
        cout << "\n--- Turn of " << currentPlayer->name << " ---" << std::endl;
        currentPlayer->displayStatus();
        opponent->displayStatus();

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

    return 0;
}
