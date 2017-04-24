//
//  Point.cpp
//  BevgrafNumFour
//
//  Created by Lajos Kovács on 2016. 12. 11..
//  Copyright © 2016. Diwin. All rights reserved.
//

#include "Point.hpp"

Point Point::operator+(Point rhs) {
    return Point(x + rhs.getx(), y + rhs.gety(), z + rhs.getz());
}

Point Point::operator+(float rhs) {
    return Point(x + rhs, y + rhs, z + rhs);
}

Point Point::operator-(float rhs) {
    return Point(x-rhs, y-rhs, z-rhs);
}

Point Point::operator-(Point rhs) {
    return Point(x-rhs.getx(), y-rhs.gety(), z-rhs.getz());
}

Point Point::operator*(float rhs) {
    return Point(x * rhs, y * rhs, z*rhs);
}

Point Point::operator*(Point rhs) {
    return Point(x * rhs.getx(), y * rhs.gety(), z*rhs.getz());
}

Point Point::operator/(float rhs) {
    return Point(x / rhs, y / rhs, z / rhs);
}

Point Point::operator/(Point rhs) {
    return Point(x / rhs.getx(), y / rhs.gety(), z / rhs.getz());
}

float Point::distanceFromPoint(Point rhs) {
    return (float)std::sqrt(std::pow(x  - rhs.getx(), 2) + std::pow(y - rhs.gety(), 2) + std::pow(z - rhs.getz(), 2));
}

float Point::dotProduct(Point rhs) {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

float Point::selfDotProduct() {
    return x * x + y * y + z * z;
}

Point Point::normalize() {
    float lengthOfVector = std::sqrt(selfDotProduct());
    return Point(x / lengthOfVector, y / lengthOfVector, z / lengthOfVector);
}

Point Point::crossProduct(Point rhs) {
    return Point(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
    );
}

float Point::getx() {
    return x;
}

float Point::gety() {
    return y;
}

float Point::getz() {
    return z;
}

void Point::setx(float newx) {
    x = newx;
}

void Point::sety(float newy) {
    y = newy;
}
void Point::setz(float newz) {
    z = newz;
}

Color Point::getColor() {
    return color;
}

void Point::setColor(Color c) {
    color = c;
}

void Point::toString() {
    std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;
}

void PointH::toString() {
    std::cout << "X: " << x1 << " Y: " << x2 << " Z:" << x3 << " H: " << x4  << std::endl;
}

PointH Point::toHomogen() {
    return PointH(x,y,z,1, color);
}
void PointH::setColor(Color c) {
    color = c;
}

Point PointH::toPoint() {
    if(x4 != 0) {
        return Point(x1/x4, x2/x4, x3/x4, color);
    }
    return Point(-1, -1, -1);
}

PointH PointH::operator*(Matrix rhs) {
    PointH P_;
    P_.x1 = rhs.storage[0][0]*x1 + rhs.storage[0][1]*x2 + rhs.storage[0][2]*x3 + rhs.storage[0][3]*x4;
    P_.x2 = rhs.storage[1][0]*x1 + rhs.storage[1][1]*x2 + rhs.storage[1][2]*x3 + rhs.storage[1][3]*x4;
    P_.x3 = rhs.storage[2][0]*x1 + rhs.storage[2][1]*x2 + rhs.storage[2][2]*x3 + rhs.storage[2][3]*x4;
    P_.x4 = rhs.storage[3][0]*x1 + rhs.storage[3][1]*x2 + rhs.storage[3][2]*x3 + rhs.storage[3][3]*x4;
    return P_;
}

PointH PointH::operator*(float rhs) {
    return PointH(x1 * rhs, x2 * rhs, x3*rhs, x4);
}

PointH PointH::operator/(float rhs) {
    return PointH(x1 / rhs, x2 / rhs, x3/rhs, x4);
}

PointH PointH::operator+(PointH rhs) {
    return PointH(x1+rhs.x1, x2+rhs.x2, x3+rhs.x3, x4);
}

float PointH::distanceFromPoint(PointH& rhs) {
    return (float) std::sqrt(std::pow(x1  - rhs.x1, 2) + std::pow(x2 - rhs.x2, 2));
}

float PointH::distanceFromCamera() {
    return std::sqrtf(std::powf(x1, 2)
                     + std::powf(x2, 2) +
                     + std::powf(x3, 2));
}

bool PointH::operator==(PointH rhs) {
    if(x1 != rhs.x1) {
        return false;
    }
    if(x2 != rhs.x2) {
        return false;
    }
    if(x3 != rhs.x3) {
        return false;
    }
    if(x4 != rhs.x4) {
        return false;
    }
    return true;
}

