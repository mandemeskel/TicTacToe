#include "MyCode.h"


/**
    Shape class
**/
Shape::Shape() {}
Shape::~Shape() {}



/**
    Pointtor class
**/
Point::Point() {
    this->x = 0;
    this->y = 0;
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

Point::Point( float x, float y ) {
    this->x = x;
    this->y = y;
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

Point::Point(float x, float y, float r, float g, float b) {
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;
}

float Point::distance( Point * point ) const {

    float x_diff = this->x - point->x;
    float y_diff = this->y - point->y;
    float sum = pow( x_diff, 2) + pow( y_diff, 2 );

    return sqrt( sum );

}

void Point::draw() const {
    // Set the vertex color to be whatever we stored in the point
    glColor3f(this->r, this->g, this->b);

    glBegin(GL_POINTS);

    // Draw the vertex in the right position
    glVertex2f(this->x, this->y);

    glEnd();
}

void Point::invertColor() {
    this->r = abs( this->r - 1 );
    this->g = abs( this->g - 1 );
    this->b = abs( this->b - 1 );
}



/**
    Line class
**/
Line::Line() {
    this->start = Point( 0, 0 );
    this->end = Point( 0, 0 );
    this->length = 0;
}

Line::Line( Point * start_point, Point * end_point ) {
    this->start = *start_point;
    this->end = *end_point;
    // TODO: Calculate length
    this->length = this->start.distance( &this->end ); 
}

Line::Line( Point * point, float length, Direction axis ) {

    this->start = *point;
    this->end = *point;
    this->length = length;

    if( axis == X_AXIS ) {

        this->end.x += this->length;

    } else {

        this->end.y += this->length;

    }

} 

void Line::draw() {

    glColor3f( this->start.r, this->start.g, this->start.b );
    glBegin( GL_LINES );

    glVertex2f( this->start.x, this->start.y );
    glVertex2f( this->end.x, this->end.y );

    glEnd();

}

void Line::draw( Point * color ) {

    glColor3f( color->r, color->g, color->b );
    glBegin( GL_LINES );

    glVertex2f( this->start.x, this->start.y );
    glVertex2f( this->end.x, this->end.y );

    glEnd();

}

void Line::draw( bool supress_color = false ) {

    // hide the lines natural color, this is for polygons
    // who want to control their color
    if( !supress_color )
        glColor3f( this->start.r, this->start.g, this->start.b );
    
    glBegin( GL_LINES );

    glVertex2f( this->start.x, this->start.y );
    glVertex2f( this->end.x, this->end.y );

    glEnd();

}

void Line::updateColor( Point * color ) {

    this->start.r = color->r;
    this->start.g = color->g;
    this->start.b = color->b;

}

Line::~Line() {

    // free( this->start );
    // free( this->end );

}



/**
    Polygon Class Definitions
**/
Polygon::Polygon() {

    this->origin = Point( 0, 0 );
    this->length = 0;

}

void Polygon::draw() {

    // Set the vertex color to be whatever we stored in the point
    glColor3f(
        this->origin.r, 
        this->origin.g, 
        this->origin.b
    );

    // Draw the lines for the square
    for( int n = 0; n < Square::sides; n++ )
        this->lines[n].draw( true );

}

void Polygon::invertColor() {

    this->origin.r = abs( this->origin.r - 1 );
    this->origin.g = abs( this->origin.g - 1 );
    this->origin.b = abs( this->origin.b - 1 );

    // update colors of the polygon's lines
    // TODO: this fails for some reason, buttons color does not get updated
    // on cick
    // for( int n = 0; n < this->sides; n++ )
    //     this->lines[n].updateColor( &this->origin );
    
}



/**
    Rect class
**/
Rect::Rect( float _x, float _y, float _width, float _height ) {

    this->upper_left = Point( _x, _y );
    this->width = _width;
    this->height = _height;
    this->setUpLines();

}

bool Rect::contains( Point point ) {
    
    if( point.x < this->upper_left.x || point.y > this->upper_left.y )
        return false;

    Point framed = Point( 
        point.x - this->upper_left.x,
        point.y - this->upper_left.y
    );
    double angle_r = atan2( 
        (double)framed.y, 
        (double)framed.x 
    );
    double angle_d = abs( angle_r * 180 / M_PI );
    float max_distance = 0;

    if( angle_d > 45 )
        max_distance = this->width / cos( angle_r );
    else if( angle_d < 45 )
        max_distance = this->height / sin( angle_r );
    else
        max_distance = sqrt( 
            this->width * this->width +
            this->height * this->height 
        );

    float x = framed.x * framed.x;
    float y = framed.y * framed.y;
    float actual_distance = sqrt( x + y );

    return actual_distance <= max_distance;

}

bool Rect::contains( float x, float y ) {

    return this->contains( Point( x, y ) );

}

void Rect::Rect setUpLines() {

    

}
