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

    Player(const string& name, int moralitos = 30)
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

       // Add event card *** Cambio realizado: Generación de 25 ActionCards con todas las combinaciones de 1 a 5 ***
 for (int attack = 1; attack <= 5; ++attack) {
     for (int heal = 1; heal <= 5; ++heal) {
         deck.addCard(new ActionCard(attack, heal));
     }
 }

    // Add event cards
    deck.addCard(new EventCard("Greta Thunberg accuses of greenwashing", -3));
    deck.addCard(new EventCard("Hacienda finds tax evasion", -2));
    deck.addCard(new EventCard("Hacienda finds no issues", +1));
    deck.addCard(new EventCard("Beyoncé inspires donation", +3));
    deck.addCard(new EventCard("Jeque árabe gives free gasoline", -5));
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
    
    // Inicializar el contador de turnos fuera del bucle
    int i = 1;

    cout << "Welcome to the SwiftJets VS ZaraKids! \nThis turn - based game pits two players against each other in a strategic battle to deplete their \nopponent's 'moralitos' (life points) and claim victory. You start with 30 moralitos (life). The game ends\nwhen one player's moralitos reach zero or all 40 cards are drawn.\nEach turn, players draw one of three card types :\n  Attack / Healing(25 cards): Damage your opponent or restore your own moralitos.\n   Events(10 cards): Trigger random scenarios with varying effects.\n   Arbolitos(5 cards): Rare and versatile, these cards can heal, manipulate outcomes, or even secure victory.\nVictory is decided through the following hierarchy:\n   Moralitos: Deplete your opponent's life points.\n   Arbolitos: If turns end, the player with more arbolitos wins.\n   Remaining Life: As a final tiebreaker, the player with higher moralitos prevails.\nGet ready to strategize, adapt, and outwit your opponent.Let the game begin!" << endl;
    
    while (!deck.isEmpty() && player1.moralitos > 0 && player2.moralitos > 0) {
        if (i%2!=0) {
            cout << endl << "--- Turn number " << i << " | Turn of " << player1.name << " ---" << endl;
        }
        else if (i%2==0) {
            cout << endl << "--- Turn number " << i << " | Turn of " << player2.name << " ---" << endl;
        }
        
        displayPlayers(player1, player2);
    
        Card* card = deck.drawCard();
        if (card) {
            card->display();
    
            // Lanzar el dado y usar el efecto de la carta
            int diceRoll = rollDice();
            bool wildcardUsed = false;
    
            // Aplicar efecto
            card->applyEffect(opponent->moralitos, currentPlayer->arbolitos, diceRoll, wildcardUsed);
    
            // Mostrar estado actualizado después de aplicar el efecto
            displayPlayers(player1, player2);
    
            // Eliminar la carta después de usarla
            delete card;
        }
    
        // Cambiar de jugador
        swap(currentPlayer, opponent);
    
        // Incrementar el contador de turnos
        i++;
        
        char ready;
        cout << "Are you ready for the next turn? Write anything and press enter to continue" << endl;
        cin >> ready;
    }
    
    // Determinar el ganador
    cout << "\n--- Game Over ---" << endl;
    if (player1.moralitos <= 0) {
        cout << player2.name << " wins by depleting Taylor Swift's moralitos!" << endl;
    } else if (player2.moralitos <= 0) {
        cout << player1.name << " wins by depleting Amancio Ortega's moralitos!" << endl;
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
