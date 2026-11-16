#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <windows.h> // Required for SetConsoleOutputCP
using namespace std;

/// @brief contains parameters for cards such as their suits ranks and values
struct Card {
    string suit;
    string rank;
    int value;
};

/// @brief contains gamesettings
struct GameSettings {
    int numberOfDecks = 1;
    bool hitSoft17 = false;
};

/// @brief checks if the input is integer, if not, gives error text
/// @param prompt input text, example: "Place your bet: "
/// @return returns integer value
int getValidInt(const string& prompt) {
    int value = 0;
    while (true) {
        cout << prompt;
        try {
            if (!(cin >> value)) {
                throw runtime_error("Invalid input! Please enter a number.");
            }
            return value;
        }
        catch (const exception& e) {
            cout << "\nError: " << e.what() << "\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

/// @brief checks if the input is number (especially double), if not, gives error text
/// @param prompt input text, example: "Place your bet: "
/// @return returns double value
double getValidDouble(const string& prompt) {
    double value = 0;
    while (true) {
        cout << prompt;
        try {
            if (!(cin >> value)) {
                throw runtime_error("Invalid input! Please enter a number.");
            }
            return value;
        }
        catch (const exception& e) {
            cout << "\nError: " << e.what() << "\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


/// @brief game settings function
/// @param gamesets call the gamesettings to change
void settings(GameSettings& gamesets){
    int set_choice, decknum;

    do{
        cout << "\n=====Settings=====\n";
        cout << "1. Number of decks: " << gamesets.numberOfDecks << endl;
        cout << "2. Hit soft on 17: " << ((gamesets.hitSoft17) ? "On": "Off") << endl;
        cout << "3. Back to main menu." << endl;
        set_choice = getValidInt("Enter operation: ");

        switch (set_choice)
            {
            case 1:
                decknum = getValidInt("\nNumber of decks(1-4): ");
                
                if (decknum < 5 && decknum > 0){
                    gamesets.numberOfDecks = decknum;
                }
                else{
                    cout << "\nError: Out of range! Enter a valid number(1-4).\n";
                }
                break;

            case 2:
                gamesets.hitSoft17 = !gamesets.hitSoft17;
                break;

            case 3:
                cout << "\nReturning to main menu . . .\n";
                break;

            default:
                cout << "\nInvalid operation! Choose a valid option (1-3).";
            }
        
        
    } while (set_choice != 3);
    
    
}


/// @brief creates a perfect deck
/// @param deck is a reference to the empty deck we are going to use
/// @param gamesets inserts gamesettings
void create_deck(vector<Card>& deck, GameSettings& gamesets){
    vector<string> suits = {
        "♥", "♦",
        "♣", "♠"
    };
    vector<string> ranks = {
        "A", "2", "3", "4", "5",
        "6", "7", "8", "9", "10",
        "J", "Q", "K"
    };
    
    int deck_num = gamesets.numberOfDecks;

    ///Creates the deck
    for (int d = 0; d < deck_num; d++){
        for (string suit : suits) {
            for (string rank : ranks) {

                int value;

                if (rank == "A") {
                    value = 11;
                }
                else if (rank == "J" ||
                        rank == "Q" ||
                        rank == "K") {
                    value = 10;
                }
                else {
                    value = stoi(rank);
                }

                deck.push_back({suit, rank, value});
            }
        }
    }
}

/// @brief shuffles the deck
/// @param deck includes the deck
void shuffleDeck(vector<Card>& deck) {

    random_device rd;
    mt19937 gen(rd());

    shuffle(deck.begin(), deck.end(), gen);
}


/// @brief finds sum of cards in a hand
/// @param deck takes the needed deck
/// @return returns the sum of the hand's value
int hand_sum(vector<Card>& deck){
    int sum = 0, aces = 0;
    for (int i = 0; i < deck.size(); i++){
        sum += deck[i].value;
        if (deck[i].rank == "A") aces++;
    }
    while (sum > 21 && aces > 0)
    {
        sum -= 10;
        aces--;
    }
    
    return sum;
}

/// @brief checks if the hand is soft
/// @param deck takes the needed deck
/// @return returns true if the hand is soft
bool isSoft(vector<Card>& deck){
    int sum = 0, aces = 0;
    for (int i = 0; i < deck.size(); i++){
        sum += deck[i].value;
        if (deck[i].rank == "A") aces++;
    }
    while (sum > 21 && aces > 0)
    {
        sum -= 10;
        aces--;
    }
    if (aces == 0) return false;
    else return true;
}

/// @brief prints the cards
/// @param deck which deck to show (table, player)
/// @param prompt What to show ("Table:")
void show_hand(vector<Card>& deck, string prompt){
    cout << prompt << endl;
    for (int i = 0; i < deck.size(); i++){
        cout << deck[i].rank << deck[i].suit << " ";
    }
    cout << "- " << hand_sum(deck) << endl;
}

/// @brief used for the player's turn
/// @param player_hand inserts player's initial cards
/// @param deck inserts the initial deck
void player_play(vector<Card>& player_hand, vector<Card>& deck){
    bool run = true;
    int operation;

    while (run)
    {
        cout << "\n1. Hit    2. Stand\n";
        operation = getValidInt(": ");

        switch (operation)
        {
        case 1:
            player_hand.push_back(deck.back()); deck.pop_back();
            show_hand(player_hand, "Your hand:");
            if (hand_sum(player_hand) >= 21) run = false;

            break;

        case 2:
            show_hand(player_hand, "Your hand:");
            run = false;
            break;
        default:
            cout << "\nInvalid operation! Choose a valid option (1-2).\n";
            break;
        }
    }
    
}

/// @brief used for the dealer's turn
/// @param table inserts dealer's initial cards
/// @param deck inserts the initial deck
/// @param gamesets inserts game settings
void table_play(vector<Card>& table, vector<Card>& deck, GameSettings gamesets){
    show_hand(table, "Table:");
    while (hand_sum(table) < 17 || (isSoft(table) && hand_sum(table) == 17 && gamesets.hitSoft17)){
        table.push_back(deck.back()); deck.pop_back();
        show_hand(table, "Table");
    }
}


/// @brief starts the game
/// @param gamesets sets the initially decided settings for game
void gameloop(GameSettings& gamesets){
    int game_options, turns = 0, controls, fin, tab_sum; 
    /// game options-gameloop menu choices: controls - inside game choices: fin - determines the number of inside game choices
    bool game_run = true, validcontrol = false;
    double player_money = 2500, bet = 0;
    string prompt;
    vector<Card> deck;
    vector<Card> player_hand, player_hand1, player_hand2; ///p_h1 and p_h2 is used for splitting
    vector<Card> table;
    
    create_deck(deck, gamesets);
    shuffleDeck(deck);
    
    cout << "\n=====BLACKJACK=====\n";

    do{
        cout << "\nBalance: " << player_money << "$" << endl;
        cout << "1. Start game\n";
        cout << "2. Return to main menu\n";
        game_options = getValidInt("Enter operation: ");
        

        switch (game_options)
            {
            case 1:
                turns += 1;
                game_run = true;
                validcontrol = false;

                bet = getValidDouble("\nPlace your bet: ");
                while (bet <= 0 || bet > player_money){
                    cout << "\nYou don't have enough balance!\n";
                    bet = getValidDouble("\nPlace your bet: ");
                }

                ///creates initial hands for table and player
                player_hand.push_back(deck.back()); deck.pop_back(); player_hand.push_back(deck.back()); deck.pop_back();
                table.push_back(deck.back()); deck.pop_back(); table.push_back(deck.back()); deck.pop_back();
                
                show_hand(player_hand, "Your hand:");
                cout << "Table:\n** " << table[1].rank << table[1].suit << " - " << table[1].value << endl;
                
                /// if the player gets blackjack on the first distribution, game ends here
                if (hand_sum(player_hand) == 21 && hand_sum(table) != 21){
                    game_run = false;

                    cout <<"\n**Blackjack**\n";
                    show_hand(player_hand, "Your hand:");
                    show_hand(table, "Table:");

                    cout << "\nYou Won!!\n+" << bet*1.5 << "$\n";
                    player_money += bet*1.5;
                }
                else if(hand_sum(player_hand) == 21 && hand_sum(table) == 21){
                    game_run = false;

                    cout <<"\n**Blackjack**\n";
                    show_hand(player_hand, "Your hand:");
                    show_hand(table, "Table:");

                    cout << "\nPush\n+0$\n";
                }

                /// checks if the game still runs (player didn't get blackjack)
                if (game_run){
                
                fin = 2;
                cout << "\n1. Hit    2. Stand    ";
                if (bet*2 <= player_money){
                    cout << "3. Double    ";
                    fin++;
                    if (player_hand[0].rank == player_hand[1].rank)
                    {
                        cout << "4. Split";
                        fin++;
                    }    
                }
                cout << "\n";
                

                while (!validcontrol){
                
                controls = getValidInt(": ");

                switch (controls)
                {
                case 1: ///first hit
                    validcontrol = true;

                    player_hand.push_back(deck.back()); deck.pop_back();

                    if (hand_sum(player_hand) > 21){
                        show_hand(player_hand, "Your hand:");
                        show_hand(table, "Table:");
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                        game_run = false;
                    }
                    else show_hand(player_hand, "Your hand:");

                    break;
                case 2:///first stand
                    validcontrol = true;

                    show_hand(player_hand, "Your hand:");
                    table_play(table, deck, gamesets);

                    tab_sum = hand_sum(table);

                    if (tab_sum > 21 || tab_sum < hand_sum(player_hand)){
                        cout << "\nYou Won!!\n+" << bet << "$\n";
                        player_money += bet;
                    }
                    else if (tab_sum > hand_sum(player_hand))
                    {
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                    }
                    else{
                        cout << "\nPush\n+0$\n";
                    }
                    
                    game_run = false;

                    break;
                case 3:///doubling
                    if (fin > 2){
                        validcontrol = true;

                        player_hand.push_back(deck.back()); deck.pop_back();

                        bet *= 2;
                        cout << "\nYour new bet: " << bet << endl;

                        show_hand(player_hand, "Your hand: ");


                        if (hand_sum(player_hand) > 21){
                        show_hand(table, "Table:");
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                        }
                        else{
                        table_play(table, deck, gamesets);
                        
                        tab_sum = hand_sum(table);

                        if (tab_sum > 21 || tab_sum < hand_sum(player_hand)){
                        cout << "\nYou Won!!\n+" << bet << "$\n";
                        player_money += bet;
                        }

                        else if (tab_sum > hand_sum(player_hand)){
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                        }

                        else{
                        cout << "\nPush\n+0$\n";
                        }
                        }

                        game_run = false;

                    }

                    else cout << "\nInvalid operation! Choose a valid option (1-2).\n";

                    break;
                case 4:///splitting
                    if (fin > 3){
                        player_hand1.push_back(player_hand[0]); player_hand2.push_back(player_hand[1]);
                        player_hand1.push_back(deck.back()); deck.pop_back();
                        player_hand2.push_back(deck.back()); deck.pop_back();

                        


                        show_hand(player_hand1, "Your hand: ");
                        cout << "Table:\n** " << table[1].rank << table[1].suit << " - " << table[1].value << endl;
                        player_play(player_hand1, deck);


                        show_hand(player_hand2, "Your hand: ");
                        cout << "Table:\n** " << table[1].rank << table[1].suit << " - " << table[1].value << endl;
                        player_play(player_hand2, deck);

                        table_play(table, deck, gamesets);
                        tab_sum = hand_sum(table);


                        show_hand(player_hand2, "Your hand: ");
                        show_hand(table, "Table:");

                        if (hand_sum(player_hand2) > 21){
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                        }

                        else{
                        if (tab_sum > 21 || tab_sum < hand_sum(player_hand2)){
                        cout << "\nYou Won!!\n+" << bet << "$\n";
                        player_money += bet;
                        }

                        else if (tab_sum > hand_sum(player_hand2)){
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                        }

                        else{
                        cout << "\nPush\n+0$\n";
                        }
                        }

                        show_hand(player_hand1, "Your hand: ");
                        show_hand(table, "Table:");

                        if (hand_sum(player_hand1) > 21){
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                        }

                        else{
                        if (tab_sum > 21 || tab_sum < hand_sum(player_hand1)){
                        cout << "\nYou Won!!\n+" << bet << "$\n";
                        player_money += bet;
                        }

                        else if (tab_sum > hand_sum(player_hand1)){
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                        }

                        else{
                        cout << "\nPush\n+0$\n";
                        }
                        }

                        game_run = false;
                        validcontrol = true;
                    }
                    else cout << "\nInvalid operation! Choose a valid option (1-3).\n";
                    break;
                default:
                    cout << "\nInvalid operation! Choose a valid option (1-" << fin << "),\n";
                    break;
                }
                }

                ///after first hit, program continues from here
                if(game_run){
                    player_play(player_hand, deck);
                    
                    if (hand_sum(player_hand) > 21){
                        show_hand(table, "Table:");
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                    }
                    else{
                        table_play(table, deck, gamesets);
                        
                        tab_sum = hand_sum(table);

                        if (tab_sum > 21 || tab_sum < hand_sum(player_hand)){
                        cout << "\nYou Won!!\n+" << bet << "$\n";
                        player_money += bet;
                        }

                        else if (tab_sum > hand_sum(player_hand)){
                        cout << "\nDealer wins!!\n-" << bet << "$\n";
                        player_money -= bet;
                        }

                        else{
                        cout << "\nPush\n+0$\n";
                        }
                    }
                    
                    
                    
                    game_run = false;
                }

                }
                

                
                ///at the end of the game, cards from table and player are taken away
                table.clear();
                player_hand.clear();
                
                /// after every 3 turns the deck is reshuffled
                if (turns == 3){
                    turns = 0;
                    cout << "\nReshuffling . . .\n";
                    deck.clear();
                    create_deck(deck, gamesets);
                    shuffleDeck(deck);
                }
                
                if (player_money == 0) cout << "\nYou are out of money!!\n";
                break;

            case 2:
                cout << "Returning to main menu . . .\n";
                break;

            default:
                cout << "\nInvalid operation! Choose a valid option (1-2).\n";
                break;
            }
        
        
    }while(game_options != 2 && player_money > 0);


}

/// @brief main menu
int main(){
    SetConsoleOutputCP(CP_UTF8);

    bool menu_run = true;
    int menu_choices;

    GameSettings gamesets;

    /// main menu loop
    while (menu_run){
        cout << "\n=====Main Menu=====\n";
        cout << "1. Play" << endl;
        cout << "2. Settings" << endl;
        cout << "3. Quit" << endl;
        menu_choices = getValidInt("Enter operation: ");

        switch (menu_choices)
            {
            case 1:
                gameloop(gamesets);
                break;
            case 2:
                settings(gamesets);
                break;
            case 3:
                menu_run = false;
                cout << "\nQuiting . . .\n";
                break;
            default:
                cout << "\nError: Invalid operation! Choose a valid option (1-3).\n";
            }
        
    }

    return 0;
} 