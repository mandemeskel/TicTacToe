/**
 * Michael T. Andemeskel
 * mandemeskel@ucmerced.edu
 * CSE165 Spring 2017
 * 
 * Copyright 2017
 * 
 * Lab info:
 **/

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "MyCode.h"
#include <iostream>
#include <cmath>
#define LENGTH 0.1 

using namespace std;


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
    // purple
    this->r = 1;
    this->g = 0;
    this->b = 1;
}

Point::Point( float x, float y ) {
    this->x = x;
    this->y = y;
    // purple
    this->r = 1;
    this->g = 0;
    this->b = 1;
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

float Point::getX() const {
    return this->x;
}

float Point::getY() const {
    return this->y;
}

void Point::setX( float _x ) {
    this->x = _x;
}

void Point::setY( float _y ) {
    this->y = _y;
}

float Point::getR() const {
    return this->r;
}

float Point::getG() const {
    return this->g;
}

float Point::getB() const {
    return this->b;
}

void Point::setR( float val ) {
    this->r = val;
}

void Point::setG( float val ) {
    this->g = val;
}

void Point::setB( float val ) {
    this->b = val;
}


/**
    Line class
**/
Line::Line() {
    this->start = Point( 0, 0 );
    this->end = Point( 0, 0 );
    this->length = 0;
}

Line::Line( float sx, float sy, float ex, float ey ) {
    this->start = Point( sx, sy );
    this->end = Point( ex, ey );
    this->length = this->start.distance( &this->end ); 
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

void Line::draw() const {

    glColor3f( this->start.r, this->start.g, this->start.b );
    glBegin( GL_LINES );

    glVertex2f( this->start.x, this->start.y );
    glVertex2f( this->end.x, this->end.y );

    glEnd();

}

void Line::draw( Point * color ) const {

    glColor3f( color->r, color->g, color->b );
    glBegin( GL_LINES );

    glVertex2f( this->start.x, this->start.y );
    glVertex2f( this->end.x, this->end.y );

    glEnd();

}

void Line::draw( bool supress_color = false ) const {
    
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

    this->upper_left = Point( 0, 0 );
    this->length = 0;

}

void Polygon::draw() {

    // Set the vertex color to be whatever we stored in the point
    glColor3f(
        this->upper_left.r, 
        this->upper_left.g, 
        this->upper_left.b
    );

    // Draw the lines for the square
    for( int n = 0; n < Polygon::sides; n++ )
        this->lines[n].draw( true );

}

void Polygon::invertColor( Polygon * poly ) {

    poly->upper_left.r = abs( poly->upper_left.r - 1 );
    poly->upper_left.g = abs( poly->upper_left.g - 1 );
    poly->upper_left.b = abs( poly->upper_left.b - 1 );
    
}

void Polygon::click() {
    cout << "poly click" << endl;
}

void Polygon::click( Point pnt ) {
    cout << "poly click" << endl;
}

void Polygon::click( float x, float y ) {
    cout << "poly click" << endl;
}

void Polygon::setColor( Point * color ) {

    this->upper_left.r = color->r;
    this->upper_left.g = color->g;
    this->upper_left.b = color->b;

}

void Polygon::setColor( float red, float green, float blue ) {

    this->upper_left.r = red;
    this->upper_left.g = green;
    this->upper_left.b = blue;

}

void Polygon::setOnClick( void (*callback)( Polygon *) ) {

    this->onClick = callback;

}


/**
    Rect class
**/
Rect::Rect() {

    this->upper_left = Point( 0, 0 );
    this->width = 0.1;
    this->height = 0.1;
    this->lines = new Line[Rect::sides];
    this->onClick = NULL;
    this->setUpLines();

}

Rect::Rect( float _x, float _y, float _width, float _height ) {

    this->upper_left = Point( _x, _y );
    this->width = _width;
    this->height = _height;
    this->lines = new Line[Rect::sides];
    this->onClick = NULL;
    this->setUpLines();

}

bool Rect::contains( Point point ) {
    float min_x = this->upper_left.getX();
    float max_y = this->upper_left.getY();
    float max_x, min_y;

    max_x = min_x + this->width;
    min_y = max_y - this->height;

    if( point.getX() < min_x || point.getY() > max_y )
        return false;

    if( point.getX() > max_x ||  point.getY() < min_y )
        return false;    

    return true;

}

bool Rect::contains( float x, float y ) {

    return this->contains( Point( x, y ) );

}

void Rect::draw() const {

    // Set the vertex color to be whatever we stored in the point
    glColor3f(
        this->upper_left.r, 
        this->upper_left.g, 
        this->upper_left.b
    );

    // Draw the lines for the square
    for( int n = 0; n < Rect::sides; n++ )
        this->lines[n].draw( true );

}

void Rect::setUpLines() {

    Point * left_pnt = new Point( 
        this->upper_left.x,
        this->upper_left.y
    );

    Point * right_point = new Point( 
        this->upper_left.x + this->width,
        this->upper_left.y - this->height
    );

    // top line
    this->lines[0] = Line( 
        left_pnt->getX(),
        left_pnt->getY(),
        this->upper_left.getX() + this->width,
        this->upper_left.getY()
    );

    // left line
    this->lines[1] = Line( left_pnt,
        new Point( 
            this->upper_left.x,
            this->upper_left.y - this->height
        ) 
    );
    
    // right line
    this->lines[2] = Line( right_point,
        new Point( 
            right_point->x,
            right_point->y + this->height
        )
    );

    // bottom line
    this->lines[3] = Line( right_point,
        new Point( 
            this->upper_left.x,
            this->upper_left.y - this->height
        ) 
    );

}

void Rect::click() {

    if( this->onClick )
        this->onClick( this );

}

void Rect::click( Point pnt ) {

    if( this->onClick )
        this->onClick( this );

}

void Rect::click( float x, float y ) {

    // cout << "rect click" << "( " << x;
    // cout << ", " << y << " )" << endl;
    if( this->onClick )
        this->onClick( this );

}

