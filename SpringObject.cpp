#include "SpringObject.h"

using namespace std;

SpringObject::SpringObject():SelectGravityObject()
{

}

SpringObject::SpringObject(int weight, Vector* position, Vector* speed, Vector* acceleration, map<GravityObject*,vector<double> >* links):SelectGravityObject(weight, position, speed, acceleration), _links(links)
{

}

SpringObject::SpringObject(const SpringObject &gravObj):SelectGravityObject(static_cast<SelectGravityObject>(gravObj)),  _links(gravObj.getLinks())
{

}

SpringObject::~SpringObject()
{
    delete _position, _speed, _acceleration, _links;
}

map<GravityObject*,vector<double> >* SpringObject::getLinks() const
{
    return _links;
}

/**
 * Attractive gravitation (null if we are in the object) plus spring effect (links)
 */
Vector* SpringObject::getInfluenceFor(const GravityObject* gravObj)
{
    double distance = (_position->squareDistanceTo(*(gravObj->getPosition())));
    double acceleration;

    if(distance<_weight/2) return new Vector();

    acceleration = GravityObject::G*_weight*gravObj->getWeight()/distance;
    Vector* v = new Vector(*(gravObj)->getPosition());

    //NOTICE memory leak !!!!! *t+=(*_position-*v);

    Vector* t = new Vector(*_position);
    *t-=*v;
    Vector* k = new Vector(*t);
    delete v;
    t->multiply(-acceleration);


    if(_links!=0) {
        if((*_links).find(const_cast<GravityObject*>(gravObj))!=(*_links).end())
        {
            vector<double> spring = (*((*_links).find(const_cast<GravityObject*>(gravObj)))).second;
            k->normalize();
            k->multiply(spring[0]*(sqrt(distance)-spring[1]));
//            cout <<sqrt(distance)-spring[1]<<endl;
        }
        *t+=*k;
    }

    delete k;
    return t;
}

void SpringObject::calculateGravityAcceleration(const vector<GravityObject*> &universe)
{
    _temporaryAcceleration = new Vector();

    vector<GravityObject*>::const_iterator gravObjPtr;
    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
    {
        if ((*gravObjPtr)!=this) //TODO fusion ? //TODO pas de gravité à l'intérieur
        {
            if(_links!=0) {
                if((*_links).find(const_cast<GravityObject*>(*gravObjPtr))!=(*_links).end())
                {
                    Vector* t = this->getInfluenceFor(*gravObjPtr);
                    *_temporaryAcceleration-=*t;
                    delete t;
                }
            }
        }
    }
}



