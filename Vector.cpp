#include "Vector.h"
#include <cmath>

Vector::Vector():_x(0),_y(0)
{

}

Vector::Vector(double x, double y):_x(x),_y(y)
{

}

Vector::Vector(const Vector &myVector):_x(myVector.getX()),_y(myVector.getY())
{

}

Vector::~Vector()
{

}

double Vector::getX() const
{
    return _x;
}

void Vector::setX(double x)
{
    _x = x;
}


double Vector::getY() const
{
    return _y;
}

void Vector::setY(double y)
{
    _y = y;
}

void Vector::operator+=(const Vector& a)
{
    _x+=a.getX();
    _y+=a.getY();
}

Vector& Vector::operator+(Vector const& a)
{
    Vector* copie = new Vector(_x, _y);
    (*copie)+=a;
    return *copie;
}

void Vector::operator-=(const Vector& a)
{
    _x-=a.getX();
    _y-=a.getY();
}

Vector& Vector::operator-(Vector const& a)
{
    Vector* copie = new Vector(_x, _y);
    (*copie)-=a;
    return *copie;
}

Vector& Vector::multiply(const double &a)
{
    _x*=a;
    _y*=a;
    return *this;
}

double Vector::distanceTo(const Vector &myVector) const
{
    double retour = sqrt(pow(_x-myVector.getX(),2)+pow(_y-myVector.getY(),2));
    return retour;
}

double Vector::squareDistanceTo(const Vector &myVector)
{
    double x1 = _x-myVector.getX();
    double y1 = _y-myVector.getY();
    double retour = x1*x1 + y1*y1;
    return retour;
}

Vector& Vector::normalize()
{
    double norm = getNorm();
    if(norm>0) this->multiply(1.0/norm);
    return *this;
}

double Vector::getNorm() const
{
    return this->distanceTo(Vector(0,0));
}
