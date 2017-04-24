//
//  Point.hpp
//  BevgrafNumFour
//
//  Created by Lajos Kovács on 2016. 12. 11..
//  Copyright © 2016. Diwin. All rights reserved.
//

#ifndef Point_hpp
#define Point_hpp

#include <cmath>
#include <iostream>
#include "Color.hpp"
#include "Matrix.hpp"
class Point;
class PointH;

class PointH {
public:
    PointH() {}
    PointH(float x1, float x2, float x3, float x4) : x1(x1), x2(x2) ,x3(x3), x4(x4), color(0.0,0.0,0.0) {}
    PointH(float x1, float x2, float x3, float x4, Color c) : x1(x1), x2(x2) ,x3(x3), x4(x4), color(c) {}
    ~PointH(){}

    PointH operator+(PointH rhs);
    PointH operator*(Matrix rhs);
    PointH operator*(float rhs);
    PointH operator/(float rhs);
    bool operator==(PointH rhs);

    float distanceFromCamera();
    float distanceFromPoint(PointH& rhs);
    Point toPoint();
    void setColor(Color c);
    void toString();

    float x1, x2, x3, x4;
    Color color;
};

class Point {

public:
    Point() : color(0,0,0) {}
    Point(float x, float y, float z) : x(x),y(y),z(z), color(1.0,1.0,1.0) {}
    Point(float x, float y, float z, Color c) : x(x),y(y),z(z), color(c) {}

    Point operator+(Point rhs);
    Point operator+(float rhs);
    Point operator-(float rhs);
    Point operator-(Point rhs);
    Point operator*(float rhs);
    Point operator*(Point rhs);
    Point operator/(float rhs);
    Point operator/(Point rhs);
    float distanceFromPoint(Point rhs);
    float dotProduct(Point rhs);
    float selfDotProduct();
    Point normalize();
    Point crossProduct(Point rhs);
    ~Point(){}
    float getx();
    float gety();
    float getz();
    Color getColor();
    void setColor(Color c);
    void setx(float newx);
    void sety(float newy);
    void setz(float newz);
    void toString();

    PointH toHomogen();
private:
    float x, y, z;
    Color color;

};

#endif /* Point_hpp */
