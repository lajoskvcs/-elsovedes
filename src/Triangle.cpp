//
// Created by Lajos Kovács on 2017. 04. 10..
//

#include "Triangle.hpp"

Triangle::Triangle() {
    this->color=Color(0.0,0.0,1.0);
}
Triangle::Triangle( PointH first,  PointH second,  PointH third) {
    this->first = first;
    this->second = second;
    this->third = third;
    this->color = Color(0.0, 0.0, 1.0);
}

Triangle::Triangle( PointH first,  PointH second,  PointH third, Color c) {
    this->first = first;
    this->second = second;
    this->third = third;
    this->color = c;
}

void Triangle::setColor(Color c) {
    color=c;
}


bool Triangle::isDrawable() {
    Point firstV = first.toPoint();
    Point secondV = second.toPoint();
    Point thirdV = third.toPoint();
    auto toCentre = Point{ 0, 0, 6 } - secondV;
    firstV = firstV - secondV;
    thirdV = thirdV - secondV;
    Point normalVector = thirdV.crossProduct(firstV).normalize();
    
    return toCentre.dotProduct(normalVector) >= 0;
}

Triangle Triangle::trsf(Matrix transform) {
    Triangle result = *this;

    result.first = first * transform;
    result.second = second * transform;
    result.third = third * transform;

    return result;
}

void Triangle::draw() {
    color.setGlColor();
    glBegin(GL_TRIANGLES);

    Point fd = first.toPoint();
    Point sd = second.toPoint();
    Point td = third.toPoint();

    glVertex2f(fd.getx(), fd.gety());
    glVertex2f(sd.getx(), sd.gety());
    glVertex2f(td.getx(), td.gety());

    glEnd();
}

float Triangle::distanceFromCamera() {
    return std::sqrt(
            std::powf((first.x1 + second.x1 + third.x1) / 3.0f, 2) +
            std::powf((first.x2 + second.x2 + third.x2) / 3.0f, 2) +
            std::powf((first.x3 + second.x3 + third.x3) / 3.0f, 2)
    );
}