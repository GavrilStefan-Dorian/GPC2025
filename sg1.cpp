/*
  This program plots different 2D functions.
*/

#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <iostream>
#include "glut.h" //MSVC local library install
// #include <GL/glut.h> //system-wide install (or compiler default path)

double circle = atan(1) * 8;
double halfCircle = atan(1) * 4;
double tau = circle; // 2 * PI = TAU
double pi = halfCircle; // TAU / 2 = PI

//How often should the drawing algorithm sample the function.
double step = 0.05;


int defaultW = 1000, defaultH = 1000;

unsigned char prevKey;

/*
   Nicomedes' Conchoid
   $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. or
   $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. where
   $t \in (-\pi / 2, \pi / 2)$
*/
void Display1() {
    double xmax, ymax, xmin, ymin;
    /*
      Nicomedes' Conchoid is a family of functions. These two parameters,
      a and b, choose a specific 2D function from that family.
      It has two branches, so, for each y, we have two x values.
      Therefore, we're actually drawing two lines, not one.
     */
    double a = 1, b = 2;

    /*
      We shadow the global variable with a different step size,
      as values different from 0.05
      would yield a significantly different plot.
    */
    double step = 0.05;

    /*
      First, we compute the points of the function, so we can determine
      the maximal extend of the drawing.
     */
    xmax = a - b - 1;
    xmin = a + b + 1;
    ymax = ymin = 0;
    for (double t = -pi / 2 + step; t < pi / 2; t += step) {
        double x1, y1, x2, y2;
        x1 = a + b * cos(t);
        xmax = (xmax < x1) ? x1 : xmax;
        xmin = (xmin > x1) ? x1 : xmin;

        x2 = a - b * cos(t);
        xmax = (xmax < x2) ? x2 : xmax;
        xmin = (xmin > x2) ? x2 : xmin;

        y1 = a * tan(t) + b * sin(t);
        ymax = (ymax < y1) ? y1 : ymax;
        ymin = (ymin > y1) ? y1 : ymin;

        y2 = a * tan(t) - b * sin(t);
        ymax = (ymax < y2) ? y2 : ymax;
        ymin = (ymin > y2) ? y2 : ymin;
    }
    //We care about the maximal extent on each axis (from the origin).
    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    /*
      Since we have, for x and for y, the maximal absolute values,
      dividing the coordinates of the points by these values will
      ensure we draw the whole function inside [-1, 1]^2, the default
      OpenGL screen.
     */

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double t = -pi / 2 + step; t < pi / 2; t += step) {
        double x1, y1, x2, y2; //You might get some warnings in your IDE. Why?
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2d(x1, y1);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (double t = -pi / 2 + step; t < pi / 2; t += step) {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2d(x2, y2);
    }
    glEnd();
}

// $f(x) = \left| sin(x) \right| \cdot e^{-sin(x)}, x \in \left[ 0, 8 \cdot \pi \right]$, 
void Display2() {
    /*
      We can determine how far the function extends
      (and thus the needed scaling factors)
      by looking at the function and doing a bit of Calculus.
    */
    double xmax = 8 * pi;
    double ymax = exp(1.1); //Why 1.1?

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += step) {
        double x1, y1;
        x1 = x / xmax;
        y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;
        glVertex2d(x1, y1);
    }
    glEnd();
}

/* 1)
   \( f(x) =
     \left\{
       \begin{array}{cl}
         1              & x = 0   \\
         \frac{d(x)}{x} & x \gt 0 \\
       \end{array}
     \right.
   \)
 */
void Display3() {
    double xmax = 50;
    double ymax = 0;

    
    for (double x = 0; x <= xmax; x += step) {
        double x1, y1;
		if (x == 0) {
			x1 = 0;
			y1 = 1;
		}
        else {
            x1 = x / xmax;
            y1 = ((ceil(x) - x) < (x - floor(x))) ? (ceil(x) - x) / x : (x - floor(x)) / x;
        }
		ymax = (ymax < y1) ? y1 : ymax;
    }

    xmax += 0.25;
    ymax += 0.25;
    
    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x <= xmax; x += step) {
        double x1, y1;
        if (x == 0) {
            x1 = 0;
            y1 = 1;
        }
        else {
            x1 = x / xmax;
            y1 = ((ceil(x) - x) < (x - floor(x))) ? (ceil(x) - x) / (x * ymax) : (x - floor(x)) / (x * ymax);
        }
        glVertex2d(x1, y1);
    }
    glEnd();
}

//3) function arguments e.g.: f(a, b, t), where a and b are function family parameters, and the is the driving variables.
void plot(double (*x)(double, double, double), double (*y)(double, double, double), double a, double b, double intervalStart, double intervalEnd, double step = 0.05, bool normaliseBoth = true, double scaleX = 1, double scaleY = 1, GLint primitive = GL_LINE_STRIP) {
    double xmin = x(a, b, intervalStart);
    double xmax = xmin;
    double ymax = y(a, b, intervalStart);
    double ymin = ymax;


    for (double t = intervalStart; t <= intervalEnd; t += step) {
        double x1, y1;
        x1 = x(a, b, t);
        y1 = y(a, b, t);
        ymin = (ymin > y1) ? y1 : ymin;
        ymax = (ymax < y1) ? y1 : ymax;

        xmin = (xmin > x1) ? x1 : xmin;
        xmax = (xmax < x1) ? x1 : xmax;
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    xmax += 0.05;
    ymax += 0.05;

    if (!normaliseBoth)
    {
        xmax = (xmax > ymax) ? xmax : ymax;
        ymax = xmax;
    }

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (double t = intervalStart; t <= intervalEnd; t += step) {
        double x1, y1;
        x1 = x(a, b, t) / xmax;
        y1 = y(a, b, t) / ymax;

        if (y1 < 0)
            glColor3f(0.8, 0.1, 0.4);
        else
            glColor3f(0.4, 0.1, 0.4);
        glVertex2d(x1, y1);
    }
    glEnd();
}

/*
  2) Circle Concoid (Limaçon, Pascal's Snail):
  \(x = 2 \cdot (a \cdot cos(t) + b) \cdot cos(t), \; y = 2 \cdot (a \cdot cos(t) + b) \cdot sin(t), \; t \in (-\pi, \pi)\) .
  For this plot, \(a = 0.3, \; b = 0.2\) .
*/
double x4(double a, double b, double t) {
    return 2 * (a * cos(t) + b) * cos(t);
}

double y4(double a, double b, double t) {
    return 2 * (a * cos(t) + b) * sin(t);
}

void Display4() {
	double a = 0.3, b = 0.2;
	plot(&x4, &y4, a, b, -pi + 0.0005, pi - 0.0005, 0.0005);
}

/*
  2) Cicloid:
  \(x = a \cdot t - b \cdot sin(t), \; y = a - b \cdot cos(t), \; t \in \mathbb{R} \) .
  For this plot, \(a = 0.1, \; b = 0.2\) .
*/

double x5(double a, double b, double t) {
    return a * t - b * sin(t);
}

double y5(double a, double b, double t) {
    return a - b * cos(t);
}
void Display5() {
    double a = 0.1, b = 0.2;
    plot(&x5, &y5, a, b, -9, 9, step, false);
}

/*
  2) Epicicloid:
  \(x = (a + b) \cdot cos\left( \frac{b}{a} \cdot t \right) - b \cdot cos\left(t + \frac{b}{a}\cdot t \right) \)
  \(y = (a + b) \cdot sin\left( \frac{b}{a} \cdot t \right) - b \cdot sin\left(t + \frac{b}{a}\cdot t \right) \)
  \( t \in \left[ 0, 2\pi \right] \) .
  For this plot, \(a = 0.1, \; b = 0.3\) .
*/
double x6(double a, double b, double t) {
    return (a + b) * cos(b / a * t) - b * cos(t + b / a * t);
}

double y6(double a, double b, double t) {
    return (a + b) * sin(b / a * t) - b * sin(t + b / a * t);
}
void Display6() {
    double a = 0.1, b = 0.3;
	plot(&x6, &y6, a, b, 0, 2 * pi, step);
}

/*
  2) Hipocicloid:
  \(x = (a - b) \cdot cos\left( \frac{b}{a} \cdot t \right) - b \cdot cos\left(t - \frac{b}{a}\cdot t \right) \)
  \(y = (a - b) \cdot sin\left( \frac{b}{a} \cdot t \right) - b \cdot sin\left(t - \frac{b}{a}\cdot t \right) \)
  \( t \in \left[ 0, 2\pi \right] \) .
  For this plot, \(a = 0.1, \; b = 0.3\) .
 */
double x7(double a, double b, double t) {
    return (a - b) * cos(b / a * t) - b * cos(t - b / a * t);
}

double y7(double a, double b, double t) {
    return (a - b) * sin(b / a * t) - b * sin(t - b / a * t);
}
void Display7() {
    double a = 0.1, b = 0.3;
	plot(&x7, &y7, a, b, 0, 2 * pi, step);
}

/*
 4) Logarithmic spiral (in polar coordinates):
 \( r = a \cdot e^{1+t}, \; t \in (0, \infty) \) .
 For this plot, \(a = 0.02\) .
*/
double x8(double a, double b, double t) {
	return a * exp(1 + t) * cos(t);
}
double y8(double a, double b, double t) {
	return a * exp(1 + t) * sin(t);
}
void Display8() {
    double a = 0.02;
    plot(&x8, &y8, a, 0, 19 + step, 23, step, false);
}

/*
  4) Sine polar plot flower:
  \( r = sin(a \cdot t), \; t \in (0, \infty)  \) .
  For this plot, \(a = 10\), and the number 'petals' is \( 2 \cdot a \). Think about why.
*/
double x9(double a, double b, double t) {
    return sin(a * t) * cos(t);
}
double y9(double a, double b, double t) {
    return sin(a * t) * sin(t);
}
void Display9() {
    double a = 10;
	plot(&x9, &y9, a, 0, 0, 2 * pi, 0.01);
}

/*
5) Longchamps' Trisectrix:
\(
x = \frac{a}{4 \cdot cos^2(t) - 3}, \;
y = \frac{a \cdot tg(t)}{4 \cdot cos^2(t) - 3}, \;
t \in (-\pi/2, \pi/2) \setminus \{ -\pi/6, \pi/6 \} \) .
For this plot, \(a = 0.2\) .
 */
double x10(double a, double b, double t) {
	return a / (4 * cos(t) * cos(t) - 3);
}
double y10(double a, double b, double t) {
	return a * tan(t) / (4 * cos(t) * cos(t) - 3);
}

void plot10(double (*x)(double, double, double), double (*y)(double, double, double), double a, double b, double intervalStart, double intervalEnd, double step = 0.001) {
    double xmin = x(a, b, intervalStart);
    double xmax = xmin;
    double ymax = y(a, b, intervalStart);
    double ymin = ymax;


    for (double t = intervalStart; t <= intervalEnd; t += step) {
        if (t == pi / 6 || t == -pi / 6)
            continue;

        double x1 = x(a, b, t);
        double y1 = y(a, b, t);

        x1 = x(a, b, t);
        y1 = y(a, b, t);

        ymin = (ymin > y1) ? y1 : ymin;
        ymax = (ymax < y1) ? y1 : ymax;

        xmin = (xmin > x1) ? x1 : xmin;
        xmax = (xmax < x1) ? x1 : xmax;
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    xmax += 0.05;
    ymax += 0.05;

	glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (double t = intervalStart; t <= intervalEnd; t += step) {
        if (t == pi / 6 || t == -pi / 6)
            continue;

        double x1 = x(a, b, t) / xmax;
        double y1 = y(a, b, t) / ymax;

        glVertex2d(x1, y1);
    }
    glEnd();

	double redHue = 0.0;

    glBegin(GL_TRIANGLES);
    for (double t = intervalStart; t <= intervalEnd; t += 2 * step) {
        if (t == pi / 6 || t == -pi / 6)
            continue;

        double x1 = x(a, b, t) / xmax;
        double y1 = y(a, b, t) / ymax;

		glColor3f(redHue, 0, 0);
        redHue += intervalEnd / t;

        glVertex2d(-0.95, 0.95);
        glVertex2d(x1, y1);

		x1 = x(a, b, t + step) / xmax;
		y1 = y(a, b, t + step) / ymax;

		glVertex2d(x1, y1);
    }

    glEnd();
}

void Display10() {
    double a = 0.2;
    plot10(&x10, &y10, a, 0, -pi / 2 + step, - pi / 6 - step, 0.0475);
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(2);
    glPointSize(1);
    //glPolygonMode(GL_FRONT, GL_LINE);
    //Enabling blending and smoothing
    glEnable(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_NICEST, GL_POINT_SMOOTH_HINT);
    glHint(GL_NICEST, GL_LINE_SMOOTH_HINT);
    glHint(GL_NICEST, GL_POLYGON_SMOOTH_HINT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
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
    case '9':
        Display9();
        break;
    case '0':
        Display10();
        break;
    default:
        break;
    }
    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}
void KeyboardFunc(unsigned char key, int x, int y) {
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
    glutInit(&argc, argv);
    glutInitWindowSize(defaultW, defaultH);
    glutInitWindowPosition(-1, -1);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(KeyboardFunc);
    glutMouseFunc(MouseFunc);
    glutDisplayFunc(Display);
    //glutIdleFunc(Display);
    glutMainLoop();

    return 0;
}