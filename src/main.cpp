#include <iostream>
#ifdef _WIN32
#include <GLUT/GLUT.h>
#elif __APPLE__
#include <GLUT/GLUT.h>
#elif __linux__
#include <GL/glut.h>
#endif
#include <vector>
#include <cmath>
#include "Matrix.hpp"
#include "Point.hpp"

/**
 * Decalrations
 */
const int windowWidth = 1024;
const int windowHeight = 768;
bool displayPointsAndLines = true;
bool displayPoints = true;
bool displayLines = true;
std::vector<Point> defaultPoints {
        { 901, 616, Color(1.0, 0.0, 0.0) },
        { 114, 625, Color(1.0, 0.0, 0.0) },
        { 865, 58, Color(1.0, 0.0, 0.0) },
        { 93, 68, Color(1.0, 0.0, 0.0) }
};
std::vector<Point> points {
        { 901, 616, Color(1.0, 0.0, 0.0) },
        { 114, 625, Color(1.0, 0.0, 0.0) },
        { 865, 58, Color(1.0, 0.0, 0.0) },
        { 93, 68, Color(1.0, 0.0, 0.0) }
};
Point* clickedPoint = nullptr;

/**
 * Initalize the program
 */
void init (void) {
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, windowWidth, 0.0, windowHeight);
}

void drawGMTBSpline() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    Matrix matrixM{ 4, 4 };

    matrixM.storage[0][0] = -1.0f;
    matrixM.storage[0][1] = 3.0f;
    matrixM.storage[0][2] = -3.0f;
    matrixM.storage[0][3] = 1.0f;

    matrixM.storage[1][0] = 3.0f;
    matrixM.storage[1][1] = -6.0f;
    matrixM.storage[1][2] = 0.0f;
    matrixM.storage[1][3] = 4.0f;

    matrixM.storage[2][0] = -3.0f;
    matrixM.storage[2][1] = 3.0f;
    matrixM.storage[2][2] = 3.0f;
    matrixM.storage[2][3] = 1.0f;

    matrixM.storage[3][0] = 1.0f;
    matrixM.storage[3][1] = 0.0f;
    matrixM.storage[3][2] = 0.0f;
    matrixM.storage[3][3] = 0.0f;

    matrixM = matrixM / 6.0;

    Matrix matrixG{ 2, 4 };
    for(int i = 0; i < points.size(); i++) {
        int indexek[] = { i, i+1, i+2, i+3 };

        //Make circularity
        for(int j = 0; j < 4; j++) {
            if(points.size() <= indexek[j]) {
                indexek[j] = indexek[j] - (int) points.size();
            }
        }

        matrixG.storage[0][0] = points[indexek[0]].getx();
        matrixG.storage[1][0] = points[indexek[0]].gety();

        matrixG.storage[0][1] = points[indexek[1]].getx();
        matrixG.storage[1][1] = points[indexek[1]].gety();

        matrixG.storage[0][2] = points[indexek[2]].getx();
        matrixG.storage[1][2] = points[indexek[2]].gety();

        matrixG.storage[0][3] = points[indexek[3]].getx();
        matrixG.storage[1][3] = points[indexek[3]].gety();

        Matrix matrixGM = matrixG * matrixM;
        Color(0.0, 0.0, 1.0).setGlColor();

        glLineWidth(4.0);
        glBegin(GL_LINE_STRIP);

        Color(0.0, 1.0, 0.0).setGlColor();

        for (float t = 0.0; t <= 1.0; t += 0.01) {
            Matrix matrixT{ 4, 1 };
            matrixT.storage[0][0] = std::powf(t, 3);
            matrixT.storage[1][0] = std::powf(t, 2);
            matrixT.storage[2][0] = t;
            matrixT.storage[3][0] = 1;

            Matrix matrixOfPoints = matrixGM * matrixT;
            glVertex2f(matrixOfPoints.storage[0][0], matrixOfPoints.storage[1][0]);
        }
        glEnd();
    }
    if(displayPointsAndLines) {
        if(displayLines) {
            glLineWidth(2.0);
            Color(0.0, 0.0, 1.0).setGlColor();
            glBegin(GL_LINE_STRIP);
            for (auto& p: points) {
                glVertex2f(p.getx(), p.gety());
            }
            glEnd();
        }
        if(displayPoints) {
            glPointSize(5.0);
            glBegin(GL_POINTS);
            for (auto& p: points) {
                p.getColor().setGlColor();
                glVertex2f(p.getx(), p.gety());
            }
            glEnd();
        }
    }
    glutSwapBuffers();
}

void keyUp (unsigned char key, int x, int y) {
    if(key == 'r') {
        points = defaultPoints;
    }
    if(key == 'd') {
        displayPointsAndLines = !displayPointsAndLines;
        displayPoints = true;
        displayLines = true;
    }
    if(key == 'p') {
        displayPoints = !displayPoints;
    }
    if(key == 'l') {
        displayLines = !displayLines;
    }
    if(key == 'i') {
        std::cout << "======== Begin of points vector: ======================" << std::endl;
        for(Point& p : points) {
            std::cout << "Point: X: " << p.getx() << " Y: " << p.gety() << std::endl;
        }
    }
    glutPostRedisplay( );
}

void motionHandler(int x, int y) {
    y = windowHeight - y;
    if (clickedPoint != nullptr) {
        clickedPoint->setx(x);
        clickedPoint->sety(y);
        glutPostRedisplay();
    }
}

void mouseHandler(int button, int state, int x, int y) {
    if (button == GLUT_LEFT) {
        if (state == GLUT_DOWN) {

            Point clickedPointOnWindows{ (float)x, (float)windowHeight - y, Color(1.0,0.0,0.0) };

            for (Point &p: points) {
                if (p.distanceFromPoint(clickedPointOnWindows) < 10.0) {
                    clickedPoint = &p;
                    clickedPoint->setColor(Color(1.0,1.0,0.0));
                }
            }

            if(clickedPoint == nullptr) {
                points.push_back(clickedPointOnWindows);
            }

        } else if (state == GLUT_UP && clickedPoint != nullptr) {
            clickedPoint->setColor(Color(1.0,0.0,0.0));
            clickedPoint = nullptr;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("B-spline curve");
    init();
    glutDisplayFunc(drawGMTBSpline);
    glutMotionFunc(motionHandler);
    glutMouseFunc(mouseHandler);
    glutKeyboardUpFunc(keyUp);
    glutMainLoop ();
    return 0;
}