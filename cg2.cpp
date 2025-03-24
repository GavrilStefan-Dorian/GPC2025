/* The GLUT library (GL/glut.h) already includes
   GL/glu.h and GL/gl.h .
   Functions from each library are prefixed by: gl, glu, or glut.

   In Visual Studio: create an empty CPP project, then
   extract the glut library package (containing
   glut.h, glut.lib, glut.dll - or their 32 bit versions).
   Copy them so that the .h and .lib files are in the same directory
   as your .cpp file, and the .dll is together with your .exe .
   If you choose the 32-bit version of files, make sure you're
   also building a 32-bit version of the project.

   On most GNU/Linux distro, MacOS, Codeblocks, Visual Studio Code + mingw:
   GLUT is implelmented by freeglut.
   In the case of Debian / Ubuntu and variants, the packages are
   'freeglut3' and 'freeglut3-dev'.
   Manually building the binary can be done like so:
   g++ -o binary -lGL -lGLU -lglut source.cpp
   Nota bene: GL and GLU are ALLCAPS, glut is not.
   You can also use the enclosed Makefile.
*/

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <complex>
#include <cfloat>

using namespace std::complex_literals;


#include "glut.h" //MSVC local library install
//#include <GL/glut.h> //system-wide install (or compiler default path)

float circle = atan(1) * 8; // 2 * PI = TAU
float halfCircle = atan(1) * 4; // PI

int g_w = 800, g_h = 800;

unsigned char prevKey;


template <typename FloatType>
class RasterScreen {
protected:
    FloatType m_width, m_height;
    int m_pixelSize;
public:
    RasterScreen(FloatType width, FloatType height, int pixelSize = 1) :
        m_width(width),
        m_height(height),
        m_pixelSize(pixelSize){
    }

    void drawGrid(int horizontalCells, int verticalCells) {
        glLineWidth(1);
        glColor3f(0.7, 0.7, 0.7);

        float size = 0.9f;

        float stepy = (2.0f * size) / horizontalCells;
        float stepx = (2.0f * size) / verticalCells;

        float aspectRatio = (float)g_w / (float)g_h;
        if (aspectRatio > 1.0f) {
            stepx /= aspectRatio; 
        }
        else {
            stepy *= aspectRatio;  
        }

        glBegin(GL_LINES);

        float xmin = -stepx * horizontalCells / 2;
        float xmax = stepx * horizontalCells / 2;
        float ymin = -stepy * verticalCells / 2;
        float ymax = stepy * verticalCells / 2;


        for (int i = 0; i <= horizontalCells; i++) {
            float x = xmin + i * stepx;
            glVertex2f(x, ymin);
            glVertex2f(x, ymax);
        }

        for (int i = 0; i <= verticalCells; i++) {
            float y = ymin + i * stepy;
            glVertex2f(xmin, y);
            glVertex2f(xmax, y);
        }

        glEnd();
    }

    void drawPixel(int x, int y) {
        glBegin(GL_POINT);
        glVertex2d()
        glEnd();
    }

};

RasterScreen<double> rs(g_w * 0.95, g_h * 0.95);

void Display1() {
    rs.drawGrid(30, 30);
}

void Display2() {

}

void Display3() {
  
}

void Display4() {

}

void Display5() {

}

void Display6() {
}

void Display7() {

}

void Display8() {

}

void init(void) {
    // The screen is cleared using the function glClear()
    // This sets the screen to the RGBA colour specified below.
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(3);
    glPointSize(4);
    /*
      void glPolygonMode (GLenum face, GLenum mode)
      face: the face of the polygon (according to the right-hand rule)
      GL_FRONT
      GL_BACK
      GL_FRONT_AND_BACK
      mode: GL_POINT (only draw the vertices as points)
      GL_LINE (only draw the edges)
      GL_FILL (also fill the inside pixels)
    */
    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    std::cout << ("Call Display") << std::endl;
    // Clear the buffer. See init();
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    case '3':
        Display3();
        break;
    case '4':
        Display4();
        break;
    case '5':
        Display5();
        break;
    case '6':
        Display6();
        break;
    case '7':
        Display7();
        break;
    case '8':
        Display8();
        break;
    default:
        break;
    }

    //Flushes all buffers, and forces the image to be sent to the screen.
    glFlush();
}

/*
  Callback upon window reshape.
*/
//void Reshape(int w, int h) {
//    std::cout << "Window reshape. Width: " << w << " height: " << h << std::endl;
//    /* void glViewport (GLint x, GLint y,
//                        GLsizei width, GLsizei height)
//       reshapes the surface onto which we draw so that it matches the inside
//       of the on-desktop window. (x, y) define the lower-left pixel.
//       Use (0, 0, w, h) for a full-window drawing surface.
//     */
//    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//}
void Reshape(int w, int h) {
    g_w = w;
    g_h = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/*
  Callback upon key press. We receive the keycode, and the
  mouse cursor position.
*/
void KeyboardFunc(unsigned char key, int x, int y) {
    std::cout << "Key: " << key << " has been pressed at coordinates: ";
    std::cout << x << " x " << y << std::endl;
    //Save the last-pressed key in the global variable, so we can switch
    //display functions.
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    //The proper way to ask glut to redraw the window.
    glutPostRedisplay();
}

/*
  Callback upon mouse press or release.
  The button can be:
   GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
   (and further for mousewheel and other mouse buttons)
  The state can be either GLUT_DOWN or  GLUT_UP, for
  a pressed or released button.
  (x, y) are the coordinates of the mouse.
*/
void MouseFunc(int button, int state, int x, int y) {
    std::cout << "Mouse button ";
    std::cout << ((button == GLUT_LEFT_BUTTON) ? "left" : ((button == GLUT_RIGHT_BUTTON) ? "right" : "middle")) << " ";
    std::cout << ((state == GLUT_DOWN) ? "pressed" : "released");
    std::cout << " at coordinates: " << x << " x " << y << std::endl;
}

int main(int argc, char** argv) {
    /*Initialise the GLUT library.
      void glutInit(int *argcp, char **argv)
      GLUT may read and change/remove arguments from the command line.
      You should not have any glut* function call before this one.
    */
    glutInit(&argc, argv);

    //Set window size, in pixels.
    glutInitWindowSize(g_w, g_h);

    /* Set window position, in pixels from the top-left corner of the deskop
       to the top-left corner of the window.
       (-1, -1) means let the window manager decide.
     */
    glutInitWindowPosition(-1, -1);

    /*
      void glutInitDisplayMode (unsigned int mode)
      Request a specific display mode, with specific
      capabilities. Capability flags are bitmasks.
      GLUT_SINGLE or  GLUT_DOUBLE : single buffering or
        double-buffering (for animations).
      GLUT_RGB or GLUT_RGBA : Alpha: enable transparency
      Multisampling and other advanced modes can be enabled here.
     */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    /*
    int glutCreateWindow (char *name)
    Attempt to create a window with the provided name.
    */
    glutCreateWindow(argv[0]);


    //Our custom on-initialisation operations.
    init();

    /*Callback functions: we can write our own behaviour in
      functions, then register those function pointers in GLUT,
      so that the GLUT event loop calls our code when appropriate.
     */

     // void glutReshapeFunc (void (*Reshape)(int width, int height))
    glutReshapeFunc(Reshape);

    // void glutKeyboardFunc (void (*KeyboardFunc)(unsigned char,int,int))
    glutKeyboardFunc(KeyboardFunc);

    // void glutMouseFunc (void (*MouseFunc)(int,int,int,int))
    glutMouseFunc(MouseFunc);

    /* void glutDisplayFunc (void (*Display)(void))
     Display is also a callback. GLUT calls it whenever it needs to
     redraw pixels - window resize, window dragged from off-screen,
     unminimised, or if another window partially covered it,
     and now it (partially) uncovered it.
    */
    glutDisplayFunc(Display);

    /*
      void glutIdleFunc (void (*IdleFunc) (void))
      Whenever GLUT has some spare time, it calls this function.
      This is an easy way to display animations at a fluid framerate.
     */
     //glutIdleFunc(Display);

     /* After the previous initialisation, the glut main loop starts. This
       event loop will process external events, and call our assigned callback
       functions as appropriate. We surrender control of the program flow to it.
     */
    glutMainLoop();

    return 0;
}
