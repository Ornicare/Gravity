#include "RepulsiveObject.h"

using namespace std;

RepulsiveObject::RepulsiveObject():SelectGravityObject()
{

}

RepulsiveObject::RepulsiveObject(int weight, Vector* position, Vector* speed, Vector* acceleration):SelectGravityObject(weight, position, speed, acceleration)
{

}

RepulsiveObject::RepulsiveObject(const RepulsiveObject &gravObj):SelectGravityObject(static_cast<SelectGravityObject>(gravObj))
{

}

RepulsiveObject::~RepulsiveObject()
{
    delete _position, _speed, _acceleration;
}

/**
 * Repulsive gravitation (null if we are in the object)
 */
Vector* RepulsiveObject::getInfluenceFor(const GravityObject* gravObj)
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
    t->multiply(-acceleration);
    return t;
}

void RepulsiveObject::calculateGravityAcceleration(const vector<GravityObject*> &universe)
{

    _temporaryAcceleration = new Vector();

    vector<GravityObject*>::const_iterator gravObjPtr;
    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
    {
        if ((*gravObjPtr)!=this) //TODO fusion ? //TODO pas de gravité à l'intérieur
        {
//            acceleration = GravityObject::G*_weight*(*gravObjPtr)->getWeight()/distance;
//            Vector* v = new Vector(*(*gravObjPtr)->getPosition());
//
//            //NOTICE memory leak !!!!! *t+=(*_position-*v);
//
//
//            Vector* t = new Vector(*_position);
//            *t-=*v;
////            *t+=(*_position-*v);
////            t->setX(_position->getX()-v->getX());
////            t->setY(_position->getY()-v->getY());
//
//            t->multiply(-acceleration);
//            *_temporaryAcceleration+=*t;
//            delete t;
//            delete v;
            Vector* t = (*gravObjPtr)->getInfluenceFor(this);
            *_temporaryAcceleration-=*t;
            delete t;
        }
    }

}
