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

/**
 * Normal gravitation (null if we are in the object)
 */
Vector* GravityObject::getInfluenceFor(const GravityObject* gravObj)
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

void GravityObject::calculateGravityAcceleration(const vector<GravityObject*> &universe)
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
            *_temporaryAcceleration+=*t;
            delete t;
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

void GravityObject::draw(SDL_Surface* screen, const Parameters* params) const
{
    double x = _position->getX();
    double y = _position->getY();
    double w = (double)_weight;

//    cout << (x+params->screenX)/(params->zoomFactor) << " " << y << " " << _weight << endl;

    filledCircleRGBA(screen, (x+params->screenX)/(params->zoomFactor), (y+params->screenY)/(params->zoomFactor), sqrt(w/2)/(params->zoomFactor),50,50,50,128);
}
