#include "MyCode.h"
#include "tictactoe.h"
#include <iostream>

using namespace std;



/** Tile Class **/
int Tile::tiles = 0;

Tile::Tile() {
    this->id = Tile::tiles++;
}

Player * Tile::getOwner() const {
    return this->owner;
}

bool Tile::hasOwner() const {
    return this->owner != 0;
}

void Tile::setOwner( Player * player ) {
    this->owner = player;
    this->setColor( player->getColor() );
}

void Tile::reset() {
    this->owner = 0;
    this->setColor( 0.0f, 0.0f, 0.0f );
}

// bool Tile::contains( float x, float y ) {
//     return rect->contains( x, y );
// }

// void Tile::draw() const {
//     this->rect->draw();
// }



/** Player Class **/ 
Player::Player( bool human, bool turn ) {
    this->is_human = human;
    this->is_turn = turn;
    this->color = new Point( 0, 0, 0, 0, 0 );
}

Player::~Player() {
    delete this->color;
}

void Player::setColor( float red, float green, float blue ) {
    this->color->r = red;
    this->color->g = green;
    this->color->b = blue;
}

Point * Player::getColor() const {
    return this->color;
}

// moves player
void Player::move( Board * board ) {

    // let ai move
    if( !this->is_human )
        board->click( this->aiMove( board ) );
    
}

// calculates and returns AI move
Tile * Player::aiMove( Board * board ) {

}

void Player::startTurn() {
    this->is_turn = true;
}

void Player::endTurn() {
    this->is_turn = false;
}

void Player::reset() {
    this->is_turn = false;
    this->is_human = false;
    this->color->r = 0;
    this->color->g = 0;
    this->color->b = 0;
}



/** Board Class **/
Board::Board() {

    // create tiles
    for( int n = 0; n < 9; n++ )
        this->tiles[n] = new Tile();

    // create menu buttons
    this->menu();

    // number of moves
    this->moves = 0;

    // status of game
    this->start = true;

}

Board::~Board() {

    // free tiles
    for( int n = 0; n < 9; n++ )
        delete this->tiles[n];

    // free menu
    delete this->menu_p1;
    delete this->menu_p2;
    delete this->menu_pvp;
    delete this->menu_pve;

    // free players
    if( this->player1 != 0 ) {

        delete this->player1;
        delete this->player2;

    }
    
}

// checks for winner by using the current tile in play
bool Board::checkForWinner( Tile * tile ) const {
    bool winner;
    int id = tile->id;

    // make sure the tile has an owner
    if( tile->hasOwner() == false ) return false;

    // check 1st row
    if( id <= 2 ) {

        winner = ( tiles[0]->getOwner() == tiles[1]->getOwner()  
        && tiles[0]->getOwner() == tiles[2]->getOwner() );
        
    // 2nd row
    } else if( id <= 5 ) {

        winner = ( tiles[3]->getOwner() == tiles[4]->getOwner()  
        && tiles[3]->getOwner() == tiles[5]->getOwner() );
    
    // 3rd row
    } else {

        winner = ( tiles[6]->getOwner() == tiles[7]->getOwner()  
        && tiles[6]->getOwner() == tiles[8]->getOwner() );

    }


    // check 1st collumn
    if( (id % 3) == 0 ) {

        winner = ( tiles[0]->getOwner() == tiles[3]->getOwner()  
        && tiles[0]->getOwner() == tiles[6]->getOwner() );
        
    // 2nd collumn
    } else if( (id % 3) == 1 ) {

        winner = ( tiles[1]->getOwner() == tiles[4]->getOwner()  
        && tiles[1]->getOwner() == tiles[7]->getOwner() );
    
    // 3rd collumn
    } else {

        winner = ( tiles[2]->getOwner() == tiles[5]->getOwner()  
        && tiles[2]->getOwner() == tiles[8]->getOwner() );

    }


    // check diagnols
    // backward diagnol 
    if( (id % 4) == 0 ) {
    
        winner = ( tiles[0]->getOwner() == tiles[4]->getOwner()  
        && tiles[0]->getOwner() == tiles[8]->getOwner() );

    // forward diagnol
    } else if( (id % 2) == 0 ) {
    
        winner = ( tiles[2]->getOwner() == tiles[4]->getOwner()  
        && tiles[2]->getOwner() == tiles[6]->getOwner() );

    }


    if( winner ) cout << "winning tile: " << id << endl;

    return winner;

}

// checks if the board is clicked
bool Board::click( float x, float y ) {
    bool clicked = false;
    Tile tile;

    for( int ntile = 0; ntile < 9; ntile++ ) {

        tile = *(this->tiles[ ntile ]);
        
        if( !tile.contains( x, y ) ) continue;
        
        this->gameTileClick( &tile );    
        clicked = true;

    }

    return clicked;

}

// handels when tile is selected
void Board::gameTileClick( Tile * tile ) {

    // bad move, tile has an owner
    if( tile->hasOwner() )  {

        cout << "tile has an owner, illegal move" << endl;

    } else {

        // update board
        tile->setOwner( this->current_player );

        // change palyer
        if( this->current_player == this->player1 )
            this->current_player = this->player2;
        else
            this->current_player = this->player2;

        // updates number of moves and checks for a winner
        this->moves++;

        // game has ended
        if( this->moves == 9 ) {

            this->start = false;
            this->checkForWinner( tile );

        // check for a winner
        } else if( this->moves > 2 && this->checkForWinner( tile ) ) {
            
            this->start = false;

        // continue with game
        } else 
            // set current player to move, also activates AI
            this->current_player->move( this );

    }
}

// create menu to control game
void Board::menu() {
    this->menu_p1 = new Rect( -0.5, -0.5, 0.25, 0.25 );
    this->menu_p2 = new Rect( -0.25, -0.5, 0.25, 0.25 );
    this->menu_pvp = new Rect( 0.0, -0.5, 0.25, 0.25 );
    this->menu_pve = new Rect( 0.25, -0.5, 0.25, 0.25 );
}

// handle clicks for menu
void Board::menuClick( float x, float y ) {
    // create pvp game
    if( this->menu_pvp->contains( x, y ) ) {

        this->mode = PVP;
        cout << "PvP" << endl;

    // create pv3 game
    } else if( this->menu_pve->contains( x, y ) ) {

        this->mode = PVE;
        cout << "PvE" << endl;

    }

    // start game if it has not
    if( this->start == false ) this->startGame();

}

// draw menu to control game
void Board::menuDraw() const {
    this->menu_p1->draw();
    this->menu_p2->draw();
    this->menu_pvp->draw();
    this->menu_pve->draw();
}

// draw board 
void Board::draw() const {
    // draw tiles
    for( int n = 0; n < 9; n++ )
        this->tiles[n]->draw();

    // draw menu
    this->menuDraw();
}

// start the game, create players 
void Board::startGame() {

    this->start = true;

}

// checks if person clicked on the board 
bool Board::contains( float x_, float y_ ) {

    bool clicked = false;

    // check if the mouse was within the board
    if( this->x < x_ && (this->x + this->len) > x_ ) {

        if( this->y < y_ && (this->y + this->len) > y_ ) {

            clicked = true;

        }

    }

    // check if the mouse clicked a specific tile
    if( clicked ) {
        Tile * tile;

        for( int n = 0; n < this->num_tiles; n++ ) {

            tile = this->tiles[ n ];

            if( !tile->contains( x, y ) ) continue;

            // call click function
            tile->click();

            // check if legal move
            if( this->isLegalMove( this->current_player, tile ) ) {

                tile->setOwner( this->current_player );

                // check for a winnger 
                if( this->checkForWinner( tile ) )
                    this->delcareWinner();
                else
                    // change turn to next player
                    this->changeTurn();

            }

        }

    }

    return clicked;

}


// checks if move is legal
bool Board::isLegalMove( Player * player, Tile * tile ) {

    bool legal = false;

    if( !tile->hasOwner() )
        legal = true;

    return legal;

}


// end current player's turn and go to next player 
void Board::changeTurn() {
    
    this->moves++;

    this->current_player->endTurn();
    
    if( this->current_player == this->player1 ) 
        
        this->current_player = this->player2;
    
    else
        
        this->current_player = this->player1;  

    this->current_player->startTurn();

}