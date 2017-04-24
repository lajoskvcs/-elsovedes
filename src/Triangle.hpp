//
// Created by Lajos Kovács on 2017. 04. 10..
//

#ifndef BBB_TRIANGLE_H
#define BBB_TRIANGLE_H

#include "Color.hpp"
#include "Point.hpp"
#include "Matrix.hpp"

class Triangle {
public:
    PointH first;
    PointH second;
    PointH third;
    Color color;
    Triangle();
    Triangle(PointH first, PointH second, PointH third);
    Triangle(PointH first, PointH second, PointH third, Color c);

    void setColor(Color c);
    bool isDrawable();
    Triangle trsf(Matrix transform);
    void draw();
    float distanceFromCamera();
};
#endif //BBB_TRIANGLE_H
