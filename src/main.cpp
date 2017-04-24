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
#include "Triangle.hpp"

const int windowWidth = 800;
const int windowHeight = 800;

int keyStates[256];

float S = 6;

float radius = 5.0;

float magassag = 0;

float delta = 0;

bool displayLinesAndPoints = true;
bool displaySurface = true;
bool surfaceFirst = true;

Point camera = Point(radius * cosf(delta), radius * sinf(delta), magassag);

PointH* clickedPoint = nullptr;

std::vector<PointH> defaultControlPoints {
        {-2, -2, 0.8, 1.0},
        {-1, -2, 3.1, 1.0},
        {1, -2, 2.4, 1.0},
        {2, -2, 0.3, 1.0},

        {-2, -1, -4.1f, 1.0},
        {-1, -1, -0.5f, 1.0},
        {1, -1, 3.0, 1.0},
        {3, -1, 0.0, 1.0},

        {-4, 1, 0.0, 1.0},
        {-1, 1, 0.0, 1.0},
        {1, 1, 0.6, 1.0},
        {4, 1, 0.0, 1.0},

        {-3, 2, -1.2f, 1.0},
        {-1, 3, 1.2, 1.0},
        {1, 4, 0.6, 1.0},
        {3, 2, -1.6f, 1.0},

};

std::vector<PointH> controlPoints {
        {-2, -2, 0.8, 1.0},
        {-1, -2, 3.1, 1.0},
        {1, -2, 2.4, 1.0},
        {2, -2, 0.3, 1.0},

        {-2, -1, -4.1f, 1.0},
        {-1, -1, -0.5f, 1.0},
        {1, -1, 3.0, 1.0},
        {3, -1, 0.0, 1.0},

        {-4, 1, 0.0, 1.0},
        {-1, 1, 0.0, 1.0},
        {1, 1, 0.6, 1.0},
        {4, 1, 0.0, 1.0},

        {-3, 2, -1.2f, 1.0},
        {-1, 3, 1.2, 1.0},
        {1, 4, 0.6, 1.0},
        {3, 2, -1.6f, 1.0},

};

std::vector<std::vector<PointH>> calculatedSurfacePoints;

std::vector<Triangle> triangles;

Matrix WindowToViewportMatrix(Point wmax, Point wmin, Point vmax, Point vmin);

Matrix Vc{4,4};

Matrix WTV = WindowToViewportMatrix(Point(3, 3, 0), Point(-3, -3, 0), Point(800, 800, 0),Point(0, 0, 0));

Matrix K{4,4};

Matrix M{4,4};

float b(int i, float t) {
    switch(i) {
        case 0:
            return -std::powf(t,3) + 3*std::powf(t,2) - 3*t + 1;
        case 1:
            return 3*std::powf(t,3) - 6*std::powf(t,2) + 3*t;
        case 2:
            return -3*std::powf(t,3) + 3*std::powf(t,2);
        case 3:
            return std::powf(t,3);
        default:
            return 0.0;
    }
}

void init (void) {
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, windowWidth, 0.0, windowHeight);
}

void keyPressed (unsigned char key, int x, int y) {
    keyStates[key] = 1;
    glutPostRedisplay();
}

void keyUp (unsigned char key, int x, int y) {
    keyStates[key] = 0;
    glutPostRedisplay();
}

Matrix WindowToViewportMatrix(Point wmax, Point wmin, Point vmax, Point vmin) {
    Matrix WTV{4,4};

    WTV.storage[0][0] = (vmax.getx() - vmin.getx()) / (wmax.getx() - wmin.getx());
    WTV.storage[0][1] = 0;
    WTV.storage[0][2] = 0;
    WTV.storage[0][3] = vmin.getx() - wmin.getx() * ((vmax.getx() - vmin.getx()) / (wmax.getx() - wmin.getx()));

    WTV.storage[1][0] = 0;
    WTV.storage[1][1] = (vmax.gety() - vmin.gety()) / (wmax.gety() - wmin.gety());
    WTV.storage[1][2] = 0;
    WTV.storage[1][3] = vmin.gety() - wmin.gety() * ((vmax.gety() - vmin.gety()) / (wmax.gety() - wmin.gety()));

    WTV.storage[2][0] = 0;
    WTV.storage[2][1] = 0;
    WTV.storage[2][2] = 1;
    WTV.storage[2][3] = 0;

    WTV.storage[3][0] = 0;
    WTV.storage[3][1] = 0;
    WTV.storage[3][2] = 0;
    WTV.storage[3][3] = 1;
    return WTV;
}

PointH Scalc(float s, float t) {
    PointH p{ 0, 0, 0, 1};
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            int index = i * 4 + j;
            p = p + controlPoints[index] * b(i,s) * b(j,t);
        }
    }
    return p;
}


void calcSurfacePoints() {
    calculatedSurfacePoints.clear();

    int i = 0;

    for (float s = 0.0; s < 1.0 || std::fabs(s - 1.0) < 0.001; s += 0.1, i += 1) {
        calculatedSurfacePoints.push_back(std::vector<PointH>{});

        for (float t = 0.0; t < 1.0 || std::fabs(t - 1.0) < 0.001; t += 0.1) {
            calculatedSurfacePoints[i].push_back(Scalc(s, t));
        }
    }

}

void calcSurfaceTriangles() {
    triangles.clear();

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {

            Triangle first {
                    calculatedSurfacePoints[i][j],
                    calculatedSurfacePoints[i][j + 1],
                    calculatedSurfacePoints[i + 1][j + 1]
            };
            Triangle second {
                    calculatedSurfacePoints[i][j],
                    calculatedSurfacePoints[i + 1][j + 1],
                    calculatedSurfacePoints[i + 1][j]
            };

            triangles.push_back(first);
            triangles.push_back(second);
        }
    }

}

void calculateMatrix() {
    Vc.storage = {{1,0,0,0},{0,1,0,0},{0,0,0,0},{0,0,-1/S,1}};
    Point up = Point(0, 0, 1);
    Point P = Point(0,0,0);

    Point PC = camera-P;
    Point w = PC.normalize();

    Point u = up.crossProduct(w).normalize();
    Point v = w.crossProduct(u).normalize();
    K.storage = {
            {u.getx() , u.gety() , u.getz() , (float)-camera.dotProduct(u)},
            {v.getx() , v.gety() , v.getz() , (float)-camera.dotProduct(v)},
            {w.getx() , w.gety() , w.getz() , (float)-camera.dotProduct(w)},
            {0 , 0 , 0 , 1}
    };

    M = WTV * Vc * K;
}

void specialKeyHandler(int key, int x, int y) {
    std::cout << key << std::endl;
}

void keyOperations ( ) {

    if(keyStates['x']) {
        controlPoints = defaultControlPoints;
    }
    if (keyStates['a']) {
        camera.setx(radius * cosf(delta));
        camera.sety(radius * sinf(delta));
        camera.setz(magassag);
        delta -= 0.1;
    }
    if (keyStates['d']) {
        camera.setx(radius * cosf(delta));
        camera.sety(radius * sinf(delta));
        camera.setz(magassag);
        delta += 0.1;
    }
    if (keyStates['w']) {
        if(magassag < 10) {
            magassag += 0.09;
            camera.setx(radius * cosf(delta));
            camera.sety(radius * sinf(delta));
            camera.setz(magassag);
        }
    }
    if (keyStates['s']) {
        if(magassag > -10) {
            magassag -= 0.09;
            camera.setx(radius * cosf(delta));
            camera.sety(radius * sinf(delta));
            camera.setz(magassag);
        }
    }
    if (keyStates['e']) {
        if(radius >= 0.2) {
            radius -= 0.1;
            camera.setx(radius * cosf(delta));
            camera.sety(radius * sinf(delta));
            camera.setz(magassag);
        }
    }
    if (keyStates['q']) {
        if(radius <= 6.0) {
            radius += 0.1;
            camera.setx(radius * cosf(delta));
            camera.sety(radius * sinf(delta));
            camera.setz(magassag);
        }
    }
    if(keyStates['u']) {
        surfaceFirst = !surfaceFirst;
    }
    if(keyStates['p']) {
        displayLinesAndPoints = !displayLinesAndPoints;
    }
    if(keyStates['o']) {
        displaySurface = !displaySurface;
    }
    if(keyStates['i']) {
        for(auto point : controlPoints) {
            point.toString();
        }
    }

    if(clickedPoint != nullptr) {
        if (keyStates['r']) {
            clickedPoint->x1 -= 0.1;
        }
        if (keyStates['t']) {
            clickedPoint->x1 += 0.1;
        }

        if (keyStates['f']) {
            clickedPoint->x2 -= 0.1;
        }
        if (keyStates['g']) {
            clickedPoint->x2 += 0.1;
        }

        if (keyStates['v']) {
            clickedPoint->x3 -= 0.1;
        }
        if (keyStates['b']) {
            clickedPoint->x3 += 0.1;
        }
        calcSurfacePoints();
        calcSurfaceTriangles();
    }
}

void displayHelper() {
    if(displayLinesAndPoints) {
        glLineWidth(4.0);
        glColor3f(0.6, 0.2, 0.0);

        for (int i = 0; i < 4; i++) {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < 4; j++) {
                int index = i * 4 + j;

                auto drawable = (controlPoints[index].toPoint().toHomogen() * M).toPoint();

                glVertex2f(drawable.getx(), drawable.gety());
            }
            glEnd();
        }

        for (int i = 0; i < 4; i++) {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < 4; j++) {
                int index = j * 4 + i;
                auto drawable = (controlPoints[index].toPoint().toHomogen() * M).toPoint();

                glVertex2f(drawable.getx(), drawable.gety());
            }
            glEnd();
        }

        glPointSize(5.0);
        glBegin(GL_POINTS);
        for (auto point: controlPoints) {
            Point toDraw = (point.toPoint().toHomogen() * M).toPoint();
            if(clickedPoint != nullptr && point == *clickedPoint) {
                glColor3f(1.0,1.0,0.0);
            } else {
                glColor3f(1.0,1.0,1.0);
            }
            glVertex2f(toDraw.getx(), toDraw.gety());
        }
        glEnd();

        glPointSize(3.0);
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_POINTS);
        for (int i = 0; i < 10 + 1; i++) {
            for (int j = 0; j < 10 + 1; j++) {
                Point toDraw = (calculatedSurfacePoints[i][j] * M).toPoint();
                glVertex2f(toDraw.getx(), toDraw.gety());
            }
        }
        glEnd();
    }

}

void display( ) {
    glClear(GL_COLOR_BUFFER_BIT);

    keyOperations();

    PointH center{0, 0, 0, 1};
    center = center * M;

    calculateMatrix();

    if(!surfaceFirst) {
        displayHelper();
    }

    if(displaySurface) {
        auto trianglesToWork = triangles;

        int colorIncrement = 0;

        for (auto& triangle: trianglesToWork) {
            if (triangle.trsf(K).isDrawable()) {
                if (colorIncrement % 2) {
                    triangle.setColor(Color(0.3, 0.3, 0.0));
                } else {
                    triangle.setColor(Color(0.3, 0.0, 0.3));
                }
            } else {
                if (colorIncrement % 2) {
                    triangle.setColor(Color(0.6, 0.43, 0.0));
                } else {
                    triangle.setColor(Color(0.0, 0.6, 0.43));
                }
            }

            colorIncrement++;
        }

        std::sort(trianglesToWork.begin(), trianglesToWork.end(), [&](Triangle& lhs, Triangle& rhs) {
            return lhs.trsf(K).distanceFromCamera() > rhs.trsf(K).distanceFromCamera();
        });

        for (auto triangle: trianglesToWork) {
            triangle.trsf(M).draw();
        }
    }

    if(surfaceFirst) {
        displayHelper();
    }
    glutSwapBuffers( );
}

void mouseHandler(int button, int state, int x, int y) {
    Point clickPoint((float) x, (float) windowHeight - y, 0.0);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        calculateMatrix();

        auto copyPoints = controlPoints;

        std::sort(copyPoints.begin(), copyPoints.end(), [&](PointH& lhs, PointH& rhs) {
            return (lhs * K).distanceFromCamera() > (rhs * K).distanceFromCamera();
        });

        PointH selected;
        bool hasSelectedPoint = false;

        for (int i = 0; i < 16; i++) {
            if ((copyPoints[i].toPoint().toHomogen() * M).toPoint().distanceFromPoint(clickPoint) < 8.0) {
                selected = copyPoints[i];
                hasSelectedPoint = true;
            }
        }
        for (int j = 0; j < 16; ++j) {
            if(controlPoints[j] == selected) {
                clickedPoint = &controlPoints[j];

            }
        }
        if(!hasSelectedPoint) {
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
    glutCreateWindow("BBB");
    init();
    calcSurfacePoints();
    calcSurfaceTriangles();
    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutMainLoop ();
    return 0;
}