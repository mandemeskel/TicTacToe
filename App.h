#ifndef App_hpp
#define App_hpp


#include "GlutApp.h"
#include "MyCode.h"
#include "tictactoe.h"
#include <deque>


class App: public GlutApp {

private:
    // Maintain app state here
    float mx;
    float my;
    Point mouse;
    std::deque<Shape *> shapes; // for drawing
    std::deque<Polygon *> polygons; // for click detection
    Board * board; // tictactoe ga,e board

public:
    // Constructor, to initialize state
    App(const char* label, int x, int y, int w, int h);
    
    // These are the events we want to handle
    void draw();
    void keyPress(unsigned char key);
    void mouseDown(float x, float y);
    void mouseDrag(float x, float y);
    void addShape( Shape * shape );
    void addPolygon( Polygon * poly );

};

#endif
