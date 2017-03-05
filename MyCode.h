#ifndef MYCODE
#define MYCODE


// #include <iostream>
// #include "GlutApp.h"
// #include <cmath>


class Shape {
    protected:
        static const bool clickable = false;

    public:
        Shape();
        void invertColor();
        virtual void draw() const = 0;
        ~Shape();

};


// TODO: add privacy to this class
// A point data structure
class Point : public Shape {

    public:
        // The coordinates of the point
        float x;
        float y;

        // The color of the point
        float r;
        float g;
        float b;

        Point();
        Point( float, float );
        Point(float x, float y, float r, float g, float b);
        float distance ( Point * ) const;
        void draw() const;
        void invertColor();
        float getX() const;
        float getY() const;
        void setX( float );
        void setY( float );

};


// direction of line
enum Direction {
    X_AXIS = 'x',
    Y_AXIS = 'y'
};


// what will be drawn on a mouse click
enum Brush {
    POINT = 'POINT',
    SQUARE = 'SQUARE'
};


class Line : public Shape {
    
    public:
        Point start;
        Point end;
        float length;
        
        Line();
        Line( float, float, float, float );
        Line( Point *, Point * );
        Line( Point *, float, Direction );
        void draw() const;
        void draw( Point * ) const;
        void draw( bool ) const;
        void updateColor( Point * );
        ~Line();
};


class Polygon : public Shape {
    protected:
        Point center;
        Point upper_left;    // top left corner
        float length;
        static const int sides = 0;
        Line lines[sides];
        virtual void setUpLines() = 0;

    public:
        Polygon();
        void draw();
        void invertColor();
        virtual bool contains( Point ) = 0;
        virtual bool contains( float, float ) = 0;
        virtual void click();
        virtual void click( float, float );
        virtual void click( Point );

};


class Rect : public Polygon {

    protected:
        static const bool clickable = true;
        static const int sides = 4;
        void setUpLines();

    private:
        Point upper_left;
        float width;
        float height;

    public:
        Rect();
        Rect( float, float, float, float );
        bool contains( Point );
        bool contains( float, float );
        void draw() const;
        void click();
        void click( float, float );
        void click( Point );

};


#endif