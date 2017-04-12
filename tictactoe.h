#ifndef TICTACTOE
#define TICTACTOE

#include "MyCode.h"
#include <string>

using namespace std;


class Button;
class Tile;
class Player;
class Board;


enum GameMode {
    PVP = 0,
    PVE = 1   
};



class Button : public Rect {

    private:
        string label;

    public:
        Button( float, float, float, float, string );
        // ~Button();

        void draw() const;

};



class Tile : public Rect {

    private:
        static int tiles;
        Player * owner;
        // float x;
        // float y;
        // float width;
        // Rect * rect;

    public:
        int id;
        
        Tile();

        Tile( float, float, float );
        
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

        // checks if tile is owned by player
        bool isOwner( Player * ) const;

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

        // generates tile values ai moves are based on
        int generateTileValue( Tile *, Board * );
        
        // void setTurn( bool );
        
        // bool getTurn() const;
        
        // DONE
        void setColor( float, float, float );
        
        // DONE, returns rgb color of player
        Point * getColor() const;
        
        // DONE
        void startTurn( Board * );
        
        // DONE
        void endTurn();
        
        // DONE
        void reset();

        // DONE
        void setIsHuman( bool );

        // DONE
        bool getIsHuman() const;

};



class Board {

    public:
        const static int num_tiles = 9;

    private:
        Tile * tiles[ num_tiles ];
        Button * menu_p1;
        Button * menu_p2;
        Button * menu_pvp;
        Button * menu_pve;
        Player * player1;
        Player * player2;
        Player * current_player;
        GameMode mode;
        int moves;
        bool game_started;
        // float board_x;
        // float board_y;
        // float len;
        // Rect * board;
        // const static int num_menu_btns = 4;
        // Buttons * menu_btns;

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
        
        // DONE
        void startGame( GameMode );
        
        void menu();
        
        // DONE, check if menu buttons are clicked and handles click
        bool menuContains( float, float );
        
        // DONE
        void menuDraw() const;
        
        // DONE
        void reset();
        
        // checks if person clicked on the board, NOT DRY, click
        bool contains( float, float );
        
        // checks if move is legal
        bool isLegalMove( Player *, Tile * );

        // end current player's turn and go to next player 
        void changeTurn();

        // returns tile by id provided
        Tile * getTile( int );

        // check and handel tile clicks
        Tile * tileClicked( float, float );

        // needed?
        void declareWinner( Player * );

        // checks if player can win with this tile
        // Player * canWin( Tile * ) const;
        bool canWin( Tile *, Player * );

        // returns the opposing player
        Player * getEnemy( Player * );

        void endGame();

};



#endif