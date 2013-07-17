#ifndef VECTOR
#define VECTOR

#include <iostream>
#include <string>

class Vector
{
    public:
        Vector();
        Vector(double x, double y);
        Vector(const Vector &myVector);
        ~Vector();

        virtual double getX() const;
        virtual void setX(double x);
        virtual double getY() const;
        virtual void setY(double y);

        virtual Vector& normalize();
        virtual double getNorm() const;
        virtual double distanceTo(const Vector &myVector) const;
        virtual double squareDistanceTo(const Vector &myVector);
        virtual Vector& multiply(const double &a);

        void operator+=(const Vector& a);
        Vector& operator+(Vector const& a);
        void operator-=(const Vector& a);
        Vector& operator-(Vector const& a);
        friend std::ostream& operator<<(std::ostream &a, const Vector &v)
        {
            a<<"("<<v.getX()<<","<<v.getY()<<")";
            return a;
        }

    protected:
        double _x;
        double _y;

};

#endif

