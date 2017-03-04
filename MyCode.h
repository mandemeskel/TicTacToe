#ifndef MYCODE
#define MYCODE


class Shape {

    public:
        Shape();
        void invertColor();
        virtual void draw() const;
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
        Line( Point *, Point * );
        Line( Point *, float, Direction );
        void draw();
        void draw( Point * );
        void draw( bool ) const;
        void updateColor( Point * );
        ~Line();
};


class Polygon : public Shape {
    protected:
        Point center;
        Point origin;    // top left corner
        float length;
        static const int sides = 0;
        Line lines[sides];
        virtual void setUpLines();

    public:
        Polygon();
        void draw();
        void invertColor();

};


class Rect : public Polygon {

    protected:
        static const int sides = 4;

    private:
        Point upper_left;
        float width;
        float height;

    public:
        Rect( float, float, float, float );
        bool contains( Point );
        bool contains( float x, float y );
        void draw() const;

};


#endif