#include "GravityObject.h"

using namespace std;

GravityObject::GravityObject():_weight(0)
{

}

GravityObject::GravityObject(int weight, Vector* position, Vector* speed, Vector* acceleration):_weight(weight), _position(position), _speed(speed), _acceleration(acceleration)
{

}

GravityObject::GravityObject(const GravityObject &gravObj):_weight(gravObj.getWeight()), _position(new Vector(*gravObj.getPosition())), _speed(new Vector(*gravObj.getSpeed())), _acceleration(new Vector(*gravObj.getAcceleration()))
{

}

GravityObject::~GravityObject()
{
    delete _position, _speed, _acceleration;
}

int GravityObject::getWeight() const
{
    return _weight;
}

void GravityObject::setWeight(int weight)
{
    _weight = weight;
}

Vector* GravityObject::getPosition() const
{
    return _position;
}

Vector* GravityObject::getSpeed() const
{
    return _speed;
}

Vector* GravityObject::getAcceleration() const
{
    return _acceleration;
}

void GravityObject::calculateGravityAcceleration(const vector<GravityObject*> &universe)
{
    double acceleration;
    _temporaryAcceleration = new Vector();

    vector<GravityObject*>::const_iterator gravObjPtr;
    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
    {
        double distance = (_position->squareDistanceTo((*(*gravObjPtr)->getPosition())));
        if ((*gravObjPtr)!=this && distance>0) //TODO fusion ?
        {
            acceleration = GravityObject::G*_weight*(*gravObjPtr)->getWeight()/distance;
            Vector* v = new Vector(*(*gravObjPtr)->getPosition());

            //NOTICE memory leak !!!!! *t+=(*_position-*v);


            Vector* t = new Vector(*_position);
            *t-=*v;
//            *t+=(*_position-*v);
//            t->setX(_position->getX()-v->getX());
//            t->setY(_position->getY()-v->getY());

            t->multiply(-acceleration);
            *_temporaryAcceleration+=*t;
            delete t;
            delete v;
        }
    }

}

void GravityObject::addSpeed(Vector v)
{
    *_speed+=v;
}

void GravityObject::addAcceleration(Vector v)
{
    *_acceleration+=v;
}

void GravityObject::calculateSpeed()
{
    _temporaryAcceleration->multiply(1.0/_weight);
    *_speed+=*_temporaryAcceleration;
    delete _temporaryAcceleration;
}

void GravityObject::calculatePosition()
{
    *_position+=*_speed;
}
