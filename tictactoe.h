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
        
        // sets tile's owner, which also change's tiles color
        void setOwner( Player * );
        
        // returns tile's owner
        Player * getOwner() const;
        
        // resets state of the tile
        void reset();
        
        // void draw() const;
        
        // checks if the tile has an owner
        bool hasOwner() const;
        
        // bool contains( float, float );
        
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

        // TODO:
        void move( Board * );
        
        // TODO: used?
        Tile * aiMove( Board * );
        
        // void setTurn( bool );
        
        // bool getTurn() const;
        
        // DONE
        void setColor( float, float, float );
        
        // DONE, returns rgb color of player
        Point * getColor() const;
        
        // DONE
        void startTurn();
        
        // DONE
        void endTurn();
        
        // DONE
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
        
        // DONE, checks for winner of game by checking if
        // the passed tile is in a row, collumn, or diagnol
        // of three tiles
        bool checkForWinner( Tile * ) const;
        
        // checks if the board is clicked, NOT DRY, contains
        bool click( float, float );
        
        // handles click on a game tile
        void gameTileClick( Tile * );
        
        void draw() const;
        
        void endTurn();
        
        void startGame();
        
        void menu();
        
        void menuClick( float, float );
        
        void menuDraw() const;
        
        // void promptUser();
        
        void reset();
        
        // checks if person clicked on the board, NOT DRY, click
        bool contains( float, float );
        
        // checks if move is legal
        bool isLegalMove( Player *, Tile * );

        // end current player's turn and go to next player 
        void changeTurn();

};



#endif