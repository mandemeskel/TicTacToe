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
    // this->x = x_;
    // this->y = y_;
    // this->width = width;
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
        board->gameTileClick( this->aiMove( board ) );
    
}

// calculates and returns AI move
Tile * Player::aiMove( Board * board ) {
    return board->getTile( 4 );
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
    // this->color->r = 0;
    // this->color->g = 0;
    // this->color->b = 0;
}

void Player::setPlayerToAi( bool is_ai ) {
    this->is_human = !is_ai;
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
// NOT USED
bool Board::click( float x, float y ) {
    bool clicked = false;
    Tile * tile;

    for( int ntile = 0; ntile < 9; ntile++ ) {

        tile = this->tiles[ ntile ];
        
        if( !tile->contains( x, y ) ) continue;
        
        this->gameTileClick( tile );    
        clicked = true;

    }

    return clicked;

}

// handels when tile is selected
// NOT USED
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

            this->game_started = false;
            this->checkForWinner( tile );

        // check for a winner
        } else if( this->moves > 2 && this->checkForWinner( tile ) ) {
            
            this->game_started = false;

        // continue with game
        } else 
            // set current player to move, also activates AI
            this->current_player->move( this );

    }
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

    // draw the board
    // this->board->draw();

}

// start the game, create players 
void Board::startGame( GameMode game_mode ) {
    
    this->reset();
    this->game_started = true;
    this->mode = game_mode;

    this->player1->startTurn();
    this->current_player = this->player1;
    this->player1->setPlayerToAi( false );

    this->player2->endTurn();
    if( game_mode == PVP )
        this->player2->setPlayerToAi( false );
    else
        this->player2->setPlayerToAi( true );

}

// checks if person clicked on the board 
bool Board::contains( float mouse_x, float mouse_y ) {

    bool clicked = true;

    cout << "clicked " << mouse_x << " " << mouse_y << endl;

    // TODO: why do we need this
    // check if the mouse was within the board
    // if( this->x < mouse_x && (this->x + this->len) > mouse_x ) {

    //     if( this->y < mouse_y && (this->y + this->len) > mouse_y ) {

    //         clicked = true;

    //     }

    // }

    // check if the mouse clicked a specific tile
    if( clicked && this->game_started ) {

        // check and handel tile clicks
        this->tileClicked( mouse_x, mouse_y );

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

    this->current_player->startTurn();

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

    // only check if game has started
    if(this->game_started == false ) return 0;

    Tile * tile;

    for( int n = 0; n < this->num_tiles; n++ ) {

        tile = this->tiles[ n ];

        if( !tile->contains( x, y ) ) continue;

        cout << "clicked " << tile->id << endl;

        // TODO: call click function
        // tile->click();

        // check if legal move
        if( this->isLegalMove( this->current_player, tile ) ) {

            cout << "clicked " << "legal move" << endl;

            tile->setOwner( this->current_player );

            cout << "clicked tile owner " << tile->getOwner() << endl;
            cout << "clicked current player " << this->current_player << endl;

            // check for a winnger 
            if( this->checkForWinner( tile ) )
                this->declareWinner( tile->getOwner() );
            else
                // change turn to next player
                this->changeTurn();

        }

    }

    return tile;
}
