#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;
class Game {
public:
    string board;
    char PlayerA;
    char PlayerB;
    char Unowned = '.';
    Game(string board, char PlayerA, char PlayerB) : board(board), PlayerA(PlayerA), PlayerB(PlayerB) {}
};
class Player {
public:
    int id;
    char gameid;
    int wins;
    int draws;
    int loses;
    int points;

    Player(int id, char gameid)
        : id(id), gameid(gameid) {}
    void Update(int win, int draw, int loss) {
        wins += win;      
        draws += draw;   
        loses += loss;    
        points +=  win+ draw+ loss;
    }
};
bool CheckWin(Game game, char playerID) {
    for (int i = 0; i < 3; ++i) {
        if (game.board[i * 3] == playerID && game.board[i * 3 + 1] == playerID && game.board[i * 3 + 2] == playerID) {
            return true; 
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (game.board[i] == playerID && game.board[i + 3] == playerID && game.board[i + 6] == playerID) {
            return true; 
        }
    }
    if ((game.board[0] == playerID && game.board[4] == playerID && game.board[8] == playerID) ||
        (game.board[2] == playerID && game.board[4] == playerID && game.board[6] == playerID)) {
        return true; 
    }
    return false; 
}
int main() {
    vector<Game> games;
    vector<Player> players; 
    ifstream inputFilePlayer("players.txt"); 
    ifstream inputFileGame("tictactoe.txt");
    if (!inputFilePlayer || !inputFileGame) {
        cerr << "Unable to open file!" << endl;
        return 1;
    }
    int i = 0;
    string line;
    while (getline(inputFilePlayer, line)) {
        istringstream iss(line);
        int id;
        char gameid;
        if (!(iss >> id >> gameid)) {
            cerr << "Invalid player data format: " << line << endl;
            continue;
        }
        if (to_string(id).length() != 6) {
            cerr << "Invalid player ID: " << id << endl;
            continue;
        }
        players.push_back(Player(id, gameid));
    }
    char playera = ' ', playerb = ' ';
    string board = "";
    while (getline(inputFileGame, line)) {
        if (line == "") {
            for (const auto& ch : board) {
                if ((ch != '.') && (playera == ' ')) {
                    playera = ch;
                }
                else if ((ch != '.') && (playera != ch)) {
                    playerb = ch;
                }
            }
            games.push_back(Game(board, playera, playerb));
            board = "";
            playera = ' ';
            playerb = ' ';
        }
        else {
            board += line;
        }
    }
    int ID;
    int option;
    bool found = false;
    for (const auto& game : games) {
        bool winA = CheckWin(game, game.PlayerA);
        bool winB = CheckWin(game, game.PlayerB);
        for (auto& player : players) {
            if (player.gameid == game.PlayerA || player.gameid == game.PlayerB) {
                int w = 0, l = 0, d = 0;

                if (player.gameid == game.PlayerA) {
                    if (winA) w++;
                    else if (winB) l++;
                    else d++;
                }
                if (player.gameid == game.PlayerB) {
                    if (winB) w++;
                    else if (winA) l++;
                    else d++;
                }
                player.Update(w, d, l);
            }
        }
    }

    while (true) {
        cout << "Enter option (1-4): ";
        cin >> option;
        if (option == 1) {
            cout << "Option 1 selected." << endl;
            continue;
        }
        else if (option == 2) {
            cout << "Option 2 selected." << endl;
            for (const auto& game : games) {
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                    cout <<game.board[i+j];
                    }
                    cout << endl;
                }
                cout << endl;
            }
            continue;
        }
        else if (option == 3) {
            cout << "Choose option: 3" << endl << "Enter player ID: ";
            cin >> ID;
            found = false;
            for (const auto& player : players) {
                if (player.id == ID) {
                    cout << player.id << "  " << player.gameid << "  "
                        << player.wins << "  " << player.draws << "  "
                        << player.loses << "  " << player.points << endl;
                    found = true;
                }
            }
            if (!found) {
                cout << "No player found with ID " << ID << endl;
            }
            continue;
        }
        if (option == 4) {
            cout << "Exiting program." << endl;
            return 0;
        }
        else{
            cout << "Invalid option. Please try again." << endl;
            continue;
        }
    }
}
