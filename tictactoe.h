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


class Tile {

    private:
        static int tiles;
        Player * owner;
        float x;
        float y;
        float width;
        Rect * rect;

    public:
        int id;
        Tile();
        void setOwner( Player * );
        Player * getOwner() const;
        void reset();
        void draw() const;
        bool hasOwner() const;
        bool contains( float, float );

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

};



class Board {

    private:
        Tile * tiles[9];
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
        
    public:
        Board();
        ~Board();
        bool checkForWinner( Tile * ) const;
        void click( float, float );
        void click( Tile * );
        void draw() const;
        void startGame();
        void menu();
        void menuClick( float, float );
        void menuDraw() const;
        void promptUser();
        void reset();

};



#endif