#include <iostream>
#include "cmd_utils.h"

enum Player{
    PlayerOne, PlayerTwo
};

Player PitZone(int pit) {
    return pit < 6? PlayerOne : PlayerTwo;
}

Player Opponent(Player p) {
    return p == PlayerOne? PlayerTwo : PlayerOne;
}

typedef struct Range {
    int begin; // The first value of the range, inclusive. -1 if invalid range
    int end; // The last value of the range, inclusive

    Range(int begin, int end): begin(begin), end(end) {}
    bool IsValid() {
        return begin != -1;
    }
} Range;

typedef struct Range CaptureRange;

typedef struct Gameboard {
    int p1_score;
    int p2_score;
    // TODO must be in struct Game {}
    int pits[12];

    Gameboard(): p1_score(0), p2_score(0) {
        for(int i = 0; i < 12; i++) {
            pits[i] = 4;
        }
    }

    bool IsOver() {
        return p1_score == 25 || p2_score == 25 || (p1_score == 24 && p2_score == 24);
    }

    bool HasSeeds(Player p) {
        int start = p == PlayerOne? 0 : 6;
        for(int i = start; i < start+6; i++) {
            if(pits[i] != 0) return true;
        }
        return false;
    }

    bool IsSowable(Player p, int pit) {
        bool in_player_zone = PitZone(pit) == p;
        bool in_enemy_zone = !in_player_zone;

        // Must be a nonempty pit in the current player zone
        if(pits[pit] == 0 || !in_player_zone) return false;
        // If the enemy has nonempty pits, all is fine
        if(HasSeeds(Opponent(p))) return true;
        // Otherwise, the player must make a move that gives
        // the opponent seeds.
        int player_zone_end_pit = p == PlayerOne? 5 : 11;
        int seeds = pits[pit];
        return player_zone_end_pit - pit + 1 < seeds;
    }

    CaptureRange Sow(int pit) {
        int seeds = pits[pit];
        pits[pit] = 0;
        int current_pit = pit;
        int capture_begin = -1; // -1 means no capture will happen
        while(seeds != 0) {
            current_pit++;
            if(current_pit == 12) current_pit = 0;
            if(current_pit == pit) continue;
    
            pits[current_pit]++;
            seeds--;
            if(pits[current_pit] == 2 || pits[current_pit] == 3) {
                if(capture_begin == -1) capture_begin = current_pit;
            } else {
                capture_begin = -1;
            }
        }
        int capture_end = current_pit;

        // TODO trim capture range to enemy field
        return CaptureRange(capture_begin, capture_end);
    }

    // TODO board has info of current player. This fucntion should either
    // be in a struct without that info or not receive Player p.
    bool IsCapturable(Player p, CaptureRange r) {
        if(!r.IsValid()) return false;
        int i = p == PlayerOne? 6 : 0;
        int end = i + 6;
        while(i < end) {
            if(i == r.begin) {
                i = r.end + 1;
            } else {
                if(pits[i] != 0) return true;
                i++;
            }
        }
        return false;
    }

    // make sure the player doesn't capture himself...
    void Capture(Player p, CaptureRange r) {
        int score;

        for(int i = r.begin; i <= r.end; i++) {
            score += pits[i];
            pits[i] = 0;
        }

        if(p == PlayerOne) {
            p1_score += score;
        } else {
            p1_score = score;
        }
    }

    void Draw() {
        std::cout << "              F  E  D  C  B  A" << std::endl;
        std::cout << "Player Two    ";
        for(int i = 11; i >= 6; i--) {
            std::cout << pits[i] << "  ";
        }
        std::cout << "   Score: " << p2_score;
        std::cout << std::endl << "         -----------------------------" << std::endl;
        std::cout << "Player One    ";
        for(int i = 0; i <= 5; i++) {
            std::cout << pits[i] << "  ";
        }
        std::cout << "   Score: " << p1_score;
        std::cout << std::endl << "              a  b  c  d  e  f" << std::endl;
    }
} Gameboard;

typedef struct Game {
    Gameboard board;
    Player current_player;

    Game(): current_player(PlayerOne), board(Gameboard()) {}
} Game;

int CharToPit(char c) {
    switch(c) {
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        case 'd': return 3;
        case 'e': return 4;
        case 'f': return 5;
        case 'A': return 6;
        case 'B': return 7;
        case 'C': return 8;
        case 'D': return 9;
        case 'E': return 10;
        case 'F': return 11;
        default: return -1;
    }
}

int main() {
    Game game = Game();
    char user_input;

    while(true) {
        clrscr();
        game.board.Draw();
        std::cout << std::endl << std::endl;
        bool playing_p1 = game.current_player == PlayerOne;
        std::cout << "Player " << (playing_p1? 1 : 0) << " is playing this turn." << std::endl;
        std::cout << "Input pit letter (" << (playing_p1? "a-f" : "A-F") << "): ";
        // TODO improve error handling for invalid inputs (ex. multiple inputs separated by space, non chars etc)
        std::cin >> user_input;
        int chosen_pit = CharToPit(user_input);
        if (chosen_pit != -1 && game.board.IsSowable(game.current_player, chosen_pit)) {
            CaptureRange capture = game.board.Sow(chosen_pit);
            if(capture.IsValid() && game.board.IsCapturable(game.current_player, capture)) {
                game.board.Capture(game.current_player, capture);
            }

            game.current_player = Opponent(game.current_player);
        } else {
            // TODO error message
        }
    }
}
