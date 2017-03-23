#include "MyCode.h"

using namespace std;


int Tile::tiles = 0;

Tile::Tile() {
    this->id = Tile::tiles++;
}

Player * Tile::getOwner() const {
    return this->owner;
}

bool Tile::hasOwner() const {
    return this->owner != NULL;
}

void Tile::setOwner( Player * player ) {
    this->owner = player;
    this->setColor( player->getColor() )
}

void reset() {
    this->owner = NULL;
    this->setColor( &Point( 0, 0, 0 ) );
}



Player::Player( bool human, bool turn ) {
    this->is_huma = human;
    this->is_turn = turn;
    this->color = new Point( 0, 0, 0 );
}

Player::~Player() {
    delete this->color;
}

void setColor( float red, float green, float blue ) {
    this->color->r = red;
    this->color->g = green;
    this->color->b = blue;
}

Point * getColor() const {
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



Board::Board() {

    // create tiles
    for( int n = 0; n < 9; n++ )
        this->tiles[n] = new Tile();

    // draw menu buttons
    this->menu();

}

Board::~Board() {

    for( int n = 0; n < 9; n++ )
        delete this->tiles[n];

    for( int n = 0; n < 4; n++ )
        delete this->menu[n];

    delete this->menu_p1;
    delete this->menu_p2;
    delete this->menu_pvp;
    delete this->menu_pve;

    if( this->player1 != NULL ) {

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
    if( id >= 0 && id <= 2 ) {

        winner = ( tiles[0]->get0wner() == tiles[1]->get0wner()  
        && tiles[0]->get0wner() == tiles[2]->get0wner() );
        
    // 2nd row
    } else if( id >= 3 && id <= 5 ) {

        winner = ( tiles[3]->get0wner() == tiles[4]->get0wner()  
        && tiles[3]->get0wner() == tiles[5]->get0wner() );
    
    // 3rd row
    } else {

        winner = ( tiles[6]->get0wner() == tiles[7]->get0wner()  
        && tiles[6]->get0wner() == tiles[8]->get0wner() );

    }


    // check 1st collumn
    if( (id % 3) == 0 ) ) {

        winner = ( tiles[0]->get0wner() == tiles[3]->get0wner()  
        && tiles[0]->get0wner() == tiles[6]->get0wner() );
        
    // 2nd collumn
    } else if( (id % 3) == 1 ) {

        winner = ( tiles[1]->get0wner() == tiles[4]->get0wner()  
        && tiles[1]->get0wner() == tiles[7]->get0wner() );
    
    // 3rd collumn
    } else {

        winner = ( tiles[2]->get0wner() == tiles[5]->get0wner()  
        && tiles[2]->get0wner() == tiles[8]->get0wner() );

    }


    // check diagnols
    switch( id ) {
        // backward diagnol 
        case 0:
        case 4:
        case 8:

            winner = ( tiles[0]->get0wner() == tiles[4]->get0wner()  
        && tiles[0]->get0wner() == tiles[8]->get0wner() );

        break;
        // forward diagnol
        case 2:
        case 4:
        case 6:

            winner = ( tiles[2]->get0wner() == tiles[4]->get0wner()  
        && tiles[2]->get0wner() == tiles[6]->get0wner() );

        break;
    }


    if( winner ) cout << "winning tile: " << id << endl;

    return winner;

}

// handels user click on board
void Board::click( float x, float y ) {

    Tile tile;
    for( int ntile = 0; ntile < 9; ntile++ ) {

        tile = this->tiles[ ntile ];
        if( !tile.contains( x, y ) ) continue;
        this->click( &tile );    

    }

}

// handels when tile is selected
void Board::click( Tile * tile ) {
    if( tile->hasOwner() ) continue;

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

        this->end = true;
        this->checkForWinner( tile );

    // check for a winner
    } else if( this->moves > 3 && this->checkForWinner( tile ); ) {
        
        this->end = true;

    // continue with game
    } else 
        // set current player to move, also activates AI
        this->current_player.move( this );
}

// draw menu to control game
void Board::menu() {

    this->menu_p1 = new Rect( -0.5, -0.5, 0.25, 0.25 );
    this->menu_p2 = new Rect( -0.25, -0.5, 0.25, 0.25 );
    this->menu_pvp = new Rect( 0.0, -0.5, 0.25, 0.25 );
    this->menu_pve = new Rect( 0.25, -0.5, 0.25, 0.25 );

}

