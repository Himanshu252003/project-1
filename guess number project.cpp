#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

using namespace std;

class GuessNumberGame {
public:
    GuessNumberGame(int lower_bound = 1, int upper_bound = 100) :
        lower_bound(lower_bound), upper_bound(upper_bound), attempts(0) {
        srand(time(0));
        target_number = rand() % (upper_bound - lower_bound + 1) + lower_bound;
    }

    string guess(int number) {
        attempts++;
        if (guesses.count(number) > 0) {
            return "You've already guessed " + to_string(number) + ". Try a different number.";
        }

        if (number < lower_bound || number > upper_bound) {
            return "Your guess is out of bounds. Please guess a number between " + to_string(lower_bound) + " and " + to_string(upper_bound) + ".";
        }

        guesses.insert(number);

        if (number == target_number) {
            return "Congratulations! You've guessed the correct number " + to_string(target_number) + " in " + to_string(attempts) + " attempts.";
        } else if (number < target_number) {
            return "Too low! Try again.";
        } else {
            return "Too high! Try again.";
        }
    }

    string get_hint() const {
        int mid = (lower_bound + upper_bound) / 2;
        if (target_number <= mid) {
            return "The number is between " + to_string(lower_bound) + " and " + to_string(mid) + ".";
        } else {
            return "The number is between " + to_string(mid + 1) + " and " + to_string(upper_bound) + ".";
        }
    }

    int get_attempts() const {
        return attempts;
    }

private:
    int lower_bound;
    int upper_bound;
    int target_number;
    int attempts;
    set<int> guesses;
};

class Leaderboard {
public:
    Leaderboard(const string& filename = "leaderboard.txt") : filename(filename) {
        load_leaderboard();
    }

    void add_entry(const string& player_name, int attempts) {
        leaderboard.push_back({player_name, attempts});
        sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second < b.second;
        });
        save_leaderboard();
    }

    void display() const {
        if (leaderboard.empty()) {
            cout << "No entries in the leaderboard yet." << endl;
        } else {
            cout << "Leaderboard:" << endl;
            for (const auto& entry : leaderboard) {
                cout << entry.first << ": " << entry.second << " attempts" << endl;
            }
        }
    }

private:
    string filename;
    vector<pair<string, int>> leaderboard;

    void load_leaderboard() {
        ifstream file(filename);
        if (file.is_open()) {
            string name;
            int attempts;
            while (file >> name >> attempts) {
                leaderboard.push_back({name, attempts});
            }
        }
    }

    void save_leaderboard() const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& entry : leaderboard) {
                file << entry.first << " " << entry.second << endl;
            }
        }
    }
};

void show_menu() {
    cout << "\nOptions:" << endl;
    cout << "1. Play the game" << endl;
    cout << "2. View leaderboard" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    Leaderboard leaderboard;

    while (true) {
        show_menu();
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter your name: ";
            string player_name;
            cin.ignore(); // Ignore the leftover newline character from previous input
            getline(cin, player_name);

            GuessNumberGame game;

            cout << "I have selected a number between 1 and 100. Can you guess it?" << endl;
            cout << "Type 'hint' to get a hint about the range of the number." << endl;

            while (true) {
                cout << "Enter your guess: ";
                string input;
                cin >> input;

                if (input == "hint") {
                    cout << game.get_hint() << endl;
                } else {
                    try {
                        int guess = stoi(input);
                        string feedback = game.guess(guess);
                        cout << feedback << endl;
                        if (feedback.find("Congratulations") != string::npos) {
                            leaderboard.add_entry(player_name, game.get_attempts());
                            break;
                        }
                    } catch (invalid_argument&) {
                        cout << "Invalid input. Please enter a number or type 'hint' for a hint." << endl;
                    }
                }
            }

        } else if (choice == 2) {
            leaderboard.display();
        } else if (choice == 3) {
            cout << "Thank you for playing! Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
        }
    }

    return 0;
}

