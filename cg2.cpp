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
double tau = circle; // 2 * PI = TAU
double pi = halfCircle; // TAU / 2 = PI

int g_w = 800, g_h = 800;

unsigned char prevKey;

void swap(int &x, int &y) {
    x = x ^ y;
    y = x ^ y;
    x = x ^ y;
}

template <typename FloatType>
class RasterScreen {
protected:
    FloatType m_width, m_height;
    int lineThickness;
    float aspectRatio;
    float stepx, stepy;
    int** grid;
    int rows;
    int cols;
    float xmin;
    float xmax;
    float ymin;
    float ymax;

public:
    RasterScreen(FloatType width, FloatType height, int lineThickness = 3) :
        m_width(width),
        m_height(height),
        lineThickness(lineThickness){
    }

    void setLineThickness(int thickness = 3) {
        lineThickness = thickness % 2 ? thickness : thickness - 1;
    }

    void drawGrid(int horizontalCells, int verticalCells) {
        glLineWidth(1);
        glColor3f(0.7, 0.7, 0.7);

        grid = new int* [horizontalCells + 1];
        for (int i = 0; i <= horizontalCells; i++) {
            grid[i] = new int[verticalCells + 1];
        }

        for (int i = 0; i <= horizontalCells; i++) {
            for (int j = 0; j <= verticalCells; j++) {
                grid[i][j] = 0;
            }
        }

        rows = horizontalCells;
        cols = verticalCells;

        float size = 0.9f;

        stepy = (2.0f * size) / horizontalCells;
        stepx = (2.0f * size) / verticalCells;

        aspectRatio = (float)g_w / (float)g_h;

        if (aspectRatio > 1.0f) {
            stepx /= aspectRatio; 
        }
        else {
            stepy *= aspectRatio;  
        }

        glBegin(GL_LINES);

        xmin = -stepx * horizontalCells / 2;
        xmax = stepx * horizontalCells / 2;
        ymin = -stepy * verticalCells / 2;
        ymax = stepy * verticalCells / 2;


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

    void drawPixel(int x, int y, float radiusx = 0.02, float radiusy = 0.02) {
        glColor3f(0, 0, 0);
        int i;
        int triangleAmount = 70; 

        double twicePi = 2 * pi;


        aspectRatio = (float)g_w / (float)g_h;

        if (aspectRatio > 1.0f) {
             radiusx /= aspectRatio;
        }
        else {
            radiusy *= aspectRatio;
        }
    
        grid[y + cols / 2][x + rows / 2] = 1;

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(stepx * x, stepy * y);
        for (i = 0; i <= triangleAmount; i++) {
            glVertex2f(
                stepx * x + (radiusx * cos(i * twicePi / triangleAmount)),
                stepy * y + (radiusy * sin(i * twicePi / triangleAmount))
            );
        }
        glEnd();
    }

    void drawThicknessPixels(int x, int y) {
        drawPixel(x, y);

        for (int i = 1, j = 1; i < lineThickness; i += 2, j++) {
            drawPixel(x - j, y);
            drawPixel(x + j, y);
            drawPixel(x, y - j);
            drawPixel(x, y + j);
        }
    }

    void drawLineSegmentPrimitive(int x1, int y1, int x2, int y2) {
        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
        glVertex2f(x1 * stepx, y1 * stepy);
        glVertex2f(x2 * stepx, y2 * stepy);
        glEnd();
    }

    void drawCirclePrimitive(int r) {
        double angle, x, y;
        glColor3f(0, 0, 1);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= 100; i++) {
            angle = 2 * pi * i / 100;
            x = r * cos(angle);
            y = r * sin(angle);
            glVertex2d(stepx * x, stepy * y);
        }
        glEnd();
        glLineWidth(1);
    }

    int determineOctave(int x0, int y0, int xn, int yn) {
        int octave = 1;
        int dx = abs(x0 - xn);
        int dy = abs(y0 - yn);

        if (x0 <= xn) {
            if (y0 <= yn) {
                if (dx >= dy)
                    octave = 1;
                else
                    octave = 2;
            }
            else {
                if (dx >= dy)
                    octave = 8;
                else
                    octave = 7;
            }
        }
        else {
            if (y0 <= yn) {
                if (dx >= dy)
                    octave = 3;
                else
                    octave = 4;
            }
            else {
                if (dx >= dy)
                    octave = 5;
                else
                    octave = 6;
            }
        }
        return octave;
    }

    void bresenham(int x0, int y0, int xn, int yn) {
        drawLineSegmentPrimitive(x0, y0, xn, yn);

        int octave = determineOctave(x0, y0, xn, yn);

        std::cout << octave << '\n';

        int ysign = 1;
        int xsign = 1;

        if (octave == 2) {
            swap(x0, y0);
            swap(xn, yn);
        }
        if (octave == 3) {
            swap(x0, xn);
            swap(y0, yn);
            ysign = -1;
        }
        if (octave == 4) {
            swap(x0, y0);
            swap(xn, yn);
            ysign = -1;
        }
        if (octave == 5) {
            swap(x0, xn);
            swap(y0, yn);

        }
        if (octave == 6) {
            swap(x0, xn);
            swap(y0, yn);
            swap(x0, y0);
            swap(xn, yn);
        }
        if (octave == 7) {
            swap(x0, y0);
            swap(xn, yn);
            swap(x0, xn);
            swap(y0, yn);
            ysign = -1;
        }
        if (octave == 8) {
            ysign = -1;
        }
        const int dx = (xn - x0);
        const int dy = ysign * (yn - y0);
        const int dM = 2 * dy - dx;
        const int dE = 2 * dy;
        const int dNE = 2 * (dy - dx);
        int d = 0;
        /*
            Instead of adding d + dM each comparison,
            we’ ll just add it here.Thus, d becomes
            the line function through all mid−points.
            
        */
        d += dM;
        int x = x0;
        int y = y0;

        if (octave == 2 || octave == 4 || octave == 6 || octave == 7) {
            drawThicknessPixels(y, x);
            while (x < xn) {
                ++x;
                if (d <= 0) {
                    d += dE;
                }
                else {
                    d += dNE;
                    y = y + ysign;
                }
                drawThicknessPixels(y, x);
            }
        }
        else {
            drawThicknessPixels(x, y);
            while (x < xn) {
                ++x;
                if (d <= 0) {
                    d += dE;
                }
                else {
                    d += dNE;
                    y = y + ysign;
                }
                drawThicknessPixels(x, y);
            }
        }
    }

    void bresenhamOctave2With6(int r) {
        int x = 0, y = r;
        int d = 1 - r;
        int de = 3;
        int dse = -2 * r + 5;

        while (y >= x) {
            drawPixel(x, y); 
            drawPixel(-x, -y); 
            ++x;
            if (d < 0) {
                d += de;
                de += 2;
                dse += 2;
            }
            else {
                d += dse;
                de += 2;
                dse += 4;
                --y;
            }
        }
    }

    void bresenhamOctave1With5(int r) {
        int x = r, y = 0;
        int d = 1 - r;
        int dn = 3;
        int dnv = -2 * r + 5;

        while (y <= x) {
            drawPixel(x, y);
            drawPixel(-x, -y); 

            ++y;
            if (d < 0) {
                d += dn;
                dn += 2;
                dnv += 2;
            }
            else {
                d += dnv;
                dn += 2;
                dnv += 4;
                --x;
            }
        }
    }

    void bresenhamOctave8With4(int r) {
        int x = r, y = 0;
        int d = 1 - r;
        int ds = 3;
        int dsv = -2 * r + 5;

        while (-y <= x) {
            drawPixel(x, y);
            drawPixel(-x, -y);

            --y;
            if (d < 0) {
                d += ds;
                ds += 2;
                dsv += 2;
            }
            else {
                d += dsv;
                ds += 2;
                dsv += 4;
                --x;
            }
        }
    }

    void bresenhamOctave3With7(int r) {
        int x = 0, y  = r;
        int d = 1 - r;
        int dv = 3;
        int dsv = -2 * r + 5;

        while (y >= -x) {
            drawPixel(x, y);
            drawPixel(-x, -y);

            --x;
            if (d < 0) {
                d += dv;
                dv += 2;
                dsv += 2;
            }
            else {
                d += dsv;
                dv += 2;
                dsv += 4;
                --y;
            }
        }
    }

    void bresenhamCircle(int r) {
        drawCirclePrimitive(r);
        bresenhamOctave1With5(r);
        bresenhamOctave2With6(r);
        bresenhamOctave8With4(r);
        bresenhamOctave3With7(r);
    }

    void fillCircle() {
        int x = 0, y = 0;

        for (; grid[y + cols / 2][x + rows / 2] == 0; x++){
            for (int y2 = 0; grid[y2 + cols / 2][x + rows / 2] == 0; y2++) {
                drawPixel(x, y2);
                drawPixel(x, -y2);
                drawPixel(-x, y2);
                drawPixel(-x, -y2);
            }
        }
    }
};

RasterScreen<double> rs(g_w * 0.95, g_h * 0.95);

void Display1() {
    rs.drawGrid(30, 30);
    rs.bresenham(13, 2, 7, 11); 
    rs.bresenham(7, 11, -3, 13); 
    rs.bresenham(-3, 13, -12, 7);
    rs.bresenham(-12, 7, -14, -3);
    rs.bresenham(-14, -3, -8, -12);
    rs.bresenham(-8, -12, 2, -14);
    rs.bresenham(2, -14, 11, -8); 
    rs.bresenham(11, -8, 13, 2);
}

void Display2() {
    rs.drawGrid(30, 30);
    rs.bresenhamCircle(14);
}

void Display3() {
    rs.drawGrid(30, 30);
    rs.bresenhamCircle(14);
    rs.fillCircle();
}

// tester methods for drawing circles


void Display4() {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 300; i++) {
        double angle = 2 * pi * i / 300;
        double x = 10 * cos(angle);
        double y = 10 * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}

void Display5() {
    int i;
    int triangleAmount = 20;

    GLfloat twicePi = 2.0f * pi;

    int x = 0, y = 0;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + ((0 + 1) * cos(i * twicePi / triangleAmount)),
            y + ((0 - 1) * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
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