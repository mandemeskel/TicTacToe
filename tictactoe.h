#ifndef TICTACTOE
#define TICTACTOE

#include "MyCode.h"

using namespace std;


class Tile;
class Player;
class Board;


enum GameMode {
    PVP = 0,
    PVE = 1   
};


class Tile : public Rect {

    private:
        static int tiles;
        Player * owner;
        // float x;
        // float y;
        float width;
        // Rect * rect;

    public:
        int id;
        Tile();
        // TODO
        void setOwner( Player * );
        Player * getOwner() const;
        void reset();
        void draw() const;
        // TODO
        bool hasOwner() const;
        // TODO
        bool contains( float, float );
        // TODO
        void click() const;

};



class Player {

    private:
        bool is_human;
        bool is_turn;
        Point * color;

    public:
        Player( bool, bool );
        ~Player();
        void move( Board * );
        Tile * aiMove( Board * );
        // void setTurn( bool );
        // bool getTurn() const;
        void setColor( float, float, float );
        Point * getColor() const;
        // TODO
        void startTurn();
        // TODO
        void endTurn();
        void reset();

};



class Board {

    private:
        const int num_tiles = 9
        Tile * tiles[ num_tiles ];
        Rect * menu_p1;
        Rect * menu_p2;
        Rect * menu_pvp;
        Rect * menu_pve;
        Player * player1;
        Player * player2;
        Player * current_player;
        GameMode mode;
        int moves;
        bool start;
        float x;
        float y;
        float len;

    public:
        Board();
        ~Board();
        // TODO
        bool checkForWinner( Tile * ) const;
        void click( float, float );
        void click( Tile * );
        void draw() const;
        void endTurn();
        void startGame();
        void menu();
        void menuClick( float, float );
        void menuDraw() const;
        void promptUser();
        void reset();
        // checks if person clicked on the board 
        bool contains( float, float );
        // checks if move is legal
        bool isLegalMove( Player *, Tile * );
        // end current player's turn and go to next player 
        void changeTurn();

};



#endif