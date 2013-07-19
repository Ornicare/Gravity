#include "ImmovableObject.h"

using namespace std;

ImmovableObject::ImmovableObject():SelectGravityObject()
{

}

ImmovableObject::ImmovableObject(int weight, Vector* position, Vector* speed, Vector* acceleration):SelectGravityObject(weight, position, speed, acceleration)
{

}

ImmovableObject::ImmovableObject(const ImmovableObject &gravObj):SelectGravityObject(static_cast<SelectGravityObject>(gravObj))
{

}

ImmovableObject::~ImmovableObject()
{
    delete _position, _speed, _acceleration;
}

/**
 * Repulsive gravitation (null if we are in the object)
 */
Vector* ImmovableObject::getInfluenceFor(const GravityObject* gravObj)
{
    double distance = (_position->squareDistanceTo(*(gravObj->getPosition())));
    double acceleration;

    if(distance<_weight/2) return new Vector();

    acceleration = GravityObject::G*_weight*gravObj->getWeight()/distance;
    Vector* v = new Vector(*(gravObj)->getPosition());

    //NOTICE memory leak !!!!! *t+=(*_position-*v);

    Vector* t = new Vector(*_position);
    *t-=*v;
    delete v;
    t->multiply(acceleration);
    return t;
}

void ImmovableObject::calculateGravityAcceleration(const vector<GravityObject*> &universe)
{

}

void ImmovableObject::calculateSpeed()
{

}

void ImmovableObject::calculatePosition()
{

}


