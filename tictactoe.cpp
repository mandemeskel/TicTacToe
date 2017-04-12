#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "MyCode.h"
#include "tictactoe.h"
#include <iostream>

using namespace std;

bool DEBUGGING = true;


/** Button Class **/
Button::Button( float x_, float y_, float width, float height, string label ):Rect( x_, y_, width, height ) {
    this->label = label;
}

// BUGG: Does not draw label
// http://waltermilner.com/joglblogl/simpleText.htm
// http://freeglut.sourceforge.net/docs/api.php#FontRendering
void Button::draw() const {

    // Draw the label
    // http://www-etud.iro.umontreal.ca/~clavetsi/api/glut/glutStrokeCharacter.html
    // glEnable( GL_BLEND );
    // glEnable( GL_LINE_SMOOTH );
    glPushMatrix();
    glColor3f( 0, 0, 0 );
    glLineWidth( 2 );
    glTranslatef( this->upper_left.x, this->upper_left.y, 0 );
    for( int n = 0; n < this->label.length(); n++ )
        glutStrokeCharacter( 
            GLUT_STROKE_ROMAN,  
            this->label[n]
        );
    glPopMatrix();

    // Draw the lines for the button
    for( int n = 0; n < Rect::sides; n++ )
        this->lines[n].draw( true );

}


/** Tile Class **/
int Tile::tiles = 0;

Tile::Tile() {
    this->id = Tile::tiles++;
}

Tile::Tile( float x_, float y_, float width ):Rect( x_, y_, width, width ) {
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
    if( player == 0 )
        this->setColor( 0.0f, 0.0f, 0.0f );
    else
        this->setColor( player->getColor() );
}

void Tile::reset() {
    this->owner = 0;
    this->setColor( 0.0f, 0.0f, 0.0f );
}

// checks if tile is owned by player
bool Tile::isOwner( Player * player ) const {
    return this->owner == player;
}



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

void Player::startTurn( Board * board ) {
    
    if( DEBUGGING )
        cout << "startTurn" << endl;
    
    if( DEBUGGING )
        cout << "startTurn is_human " << this->is_human << endl;

    this->is_turn = true;

    // let ai move
    if( !this->is_human ) {
        
        Tile * tile = this->aiMove( board );
        const Point * point = tile->getCenter();
        board->tileClicked( 
            point->x,
            point->y
        );
    
    }

}

void Player::endTurn() {
    this->is_turn = false;
}

void Player::reset() {
    this->is_turn = false;
    this->is_human = false;
    // this->color->r = 0;
    // this->color->g = 0;
    // this->color->b = 0;
}

void Player::setIsHuman( bool human ) {
    this->is_human =  human;
}



/** Board Class **/
Board::Board() {

    // create tiles
    for( int n = 0; n < 9; n++ )
        this->tiles[n] = new Tile(
            -0.40 + 0.3 * (n%3),      // x
             0.40 - 0.3 * (int)(n/3), // y
             0.20                   // width
        );

    // create menu buttons
    this->menu();

    // creates players
    this->player1 = new Player( true, true );
    this->player1->setColor( 0.2, 0.6, 1 );
    this->player2 = new Player( true, true );
    this->player2->setColor( 1, 0.6, 0.2 );

    // number of moves
    this->moves = 0;

    // status of game, don't start game on init
    this->game_started = false;
    
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
    
    if( DEBUGGING )
        cout << "checkForWinner " << tile->id << endl;

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

    if( winner ) return true;

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

    if( winner ) return true;


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

    // if( winner ) cout << "winning tile: " << id << endl;

    return winner;

}

// create menu to control game
void Board::menu() {
    this->menu_p1 = new Button( -0.5, -0.5, 0.25, 0.15, "P1" );
    this->menu_p2 = new Button( -0.25, -0.5, 0.25, 0.15, "P2" );
    this->menu_pvp = new Button( 0.0, -0.5, 0.25, 0.15, "PVP" );
    this->menu_pve = new Button( 0.25, -0.5, 0.25, 0.15, "PVE" );
}

// handle clicks for menu
bool Board::menuContains( float x, float y ) {

    bool clicked = false;

    cout << "menuContains " << x << " " << y << endl;

    // create pvp game
    if( this->menu_pvp->contains( x, y ) ) {

        this->startGame( PVP );
        clicked = true;
        cout << "PvP" << endl;

    // create pve game
    } else if( this->menu_pve->contains( x, y ) ) {

        this->startGame( PVE );
        clicked = true;
        cout << "PvE" << endl;

    }

    return clicked;

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
void Board::startGame( GameMode game_mode ) {
  
    if( DEBUGGING )
        cout << "startGame" << endl;
      
    this->reset();
    this->game_started = true;
    this->mode = game_mode;

    this->player1->setIsHuman( true );
    this->current_player = this->player1;
  
    if( DEBUGGING )
        cout << "startGame current_player->getIsHuman" << this->current_player->getIsHuman() << endl;

    this->player2->endTurn();
    if( game_mode == PVP )
        this->player2->setIsHuman( true );
    else
        this->player2->setIsHuman( false );

    // start players turn
    this->current_player->startTurn( this );

}

// checks if person clicked on the board 
bool Board::contains( float mouse_x, float mouse_y ) {
    
    if( DEBUGGING )
        cout << "contains" << endl;

    bool clicked = true;

    if( DEBUGGING )
        cout << "contains " << mouse_x << " " << mouse_y << endl;

    if( DEBUGGING && this->current_player != 0 )
        cout << "contains getIsHuman() " << this->current_player->getIsHuman() << endl;

    // check if the mouse clicked a specific tile
    if( this->game_started ) {

        // check and handel tile clicked
        if( this->current_player != 0 ) {

            if( this->current_player->getIsHuman() )
                this->tileClicked( mouse_x, mouse_y );

        }

    }

    // check if menu is clicked
    if( clicked ) {

        // check if menu buttons clicked
        this->menuContains( mouse_x, mouse_y );

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

    this->current_player->startTurn( this );

}

// make it obivous who won the game 
void Board::declareWinner( Player * player ) {

    for( int n = 0; n < this->num_tiles; n++ )
        this->tiles[n]->setOwner( player );

}

// returns tile by id provided
Tile * Board::getTile( int id ) {
    
    if( id < 0 || id > 8 )
        return 0;

    return this->tiles[ id ];

}

void Board::reset() {

    for( int n = 0; n < this->num_tiles; n++ )
        this->tiles[n]->reset();

    this->player1->reset();
    this->player2->reset();

    this->game_started = false;
    this->moves = false;

}

// check and handel tile clicks
Tile * Board::tileClicked( float mouse_x, float mouse_y ) {

    if( DEBUGGING )
        cout << "tileClicked" << endl;

    // only check if game has started
    if(this->game_started == false ) return 0;

    Tile * tile;

    for( int n = 0; n < this->num_tiles; n++ ) {

        tile = this->tiles[ n ];

        if( !tile->contains( mouse_x, mouse_y ) ) continue;

        cout << "tileClicked " << tile->id << endl;

        // TODO: call click function
        // tile->click();

        // check if legal move
        if( this->isLegalMove( this->current_player, tile ) ) {

            cout << "tileClicked " << "legal move" << endl;

            tile->setOwner( this->current_player );

            cout << "tileClicked tile owner " << tile->getOwner() << endl;
            cout << "tileClicked current player " << this->current_player << endl;

            // check for a winner 
            if( this->checkForWinner( tile ) )
                
                this->declareWinner( tile->getOwner() );
            
            // end game, tie
            else if( this->moves >= 8 )
            
                this->endGame();

            // change turn to next player
            else
                
                this->changeTurn();

        }

    }

    return tile;
}

// checks if player can win with this tile
bool Board::canWin( Tile * tile, Player * player ) {

    if( DEBUGGING )
        cout << "canWin" << endl;

    if( DEBUGGING )
        cout << "canWin id " << tile->id << " player " << player << endl;

    if( tile->hasOwner() ) return false;
    
    bool winner = false;
    tile->setOwner( player );
    
    if( this->checkForWinner( tile ) )
        winner = true;

    tile->setOwner( 0 );
    return winner;
 
}

// returns the opposing player
Player * Board::getEnemy( Player * player ) {

    if( this->player1 == player )
        
        return this->player2;

    else

        return this->player1;

}

void Board::endGame() {
    
    this->game_started = false;
    this->current_player->endTurn();

}


/** AI methods **/
// calculates and returns AI move
Tile * Player::aiMove( Board * board ) {

    if( DEBUGGING )
        cout << "aiMove" << endl;

    Tile * tile;
    int max = 0;
    int temp = 0;

    for( int n = 0; n < board->num_tiles; n++ ) {

        temp = this->generateTileValue( board->getTile( n ), board );
        
        if( DEBUGGING )
            cout << "aiMove max_value " << max << " tile " << board->getTile( n )->id << " value " << temp << endl;
       
        if( max >= temp ) continue;
        tile = board->getTile( n );
        max = temp;

    }

    if( DEBUGGING )
        cout << "aiMove tile " << tile->id << endl;

    if( DEBUGGING )
        cout << endl;
    
    return tile;

}

// generates tile values ai moves are based on
int Player::generateTileValue( Tile * tile, Board * board ) {

    if( DEBUGGING )
        cout << "generateTileValue" << endl;

    int tile_value = 0;
    int tile_id = tile->id;
    Player * winning_player = 0;

    // check if the ai or player moves to tile, the game ends
    if( board->canWin( tile, this ) )

        winning_player = this;

    else if( board->canWin( tile, board->getEnemy( this ) ) )

        winning_player = board->getEnemy( this );


    if( tile->hasOwner() )
        
        tile_value = 0; // occupied tile
    
    else if( winning_player != 0 ) {

        if( winning_player == this )

            tile_value = 5; // winning move take it
        
        else
        
            tile_value = 4; // prevent loss

    } else {

        if( tile_id == 4 )

            tile_value = 3; // center

        else if( ( tile_id % 2 ) == 0 )

            tile_value = 2; // edges

        else

            tile_value = 1; // sides

    }


    if( DEBUGGING && tile_value > 3 )
        cout << "generateTileValue tile_id " << tile_id << " winning_player " << winning_player << endl;

    if( DEBUGGING )
        cout << endl;

    return tile_value;

}

bool Player::getIsHuman() const {
    return this->is_human;
}
