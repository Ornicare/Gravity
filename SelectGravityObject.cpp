#include <cmath>
#include "SelectGravityObject.h"

using namespace std;

SelectGravityObject::SelectGravityObject():GravityObject()
{
    initialize();
}

SelectGravityObject::SelectGravityObject(int weight, Vector* position, Vector* speed, Vector* acceleration):GravityObject(weight, position, speed, acceleration)
{
    initialize();
}

SelectGravityObject::SelectGravityObject(const SelectGravityObject &gravObj):GravityObject(static_cast<GravityObject>(gravObj)), _selectRadius(gravObj.getSelectRadius()), _squareSelectRadius(gravObj.getSquareSelectRadius()), _selected(gravObj.isSelected())
{

}

SelectGravityObject::~SelectGravityObject()
{
    delete _position, _speed, _acceleration;
}

void SelectGravityObject::setWeight(int weight)
{
    _weight = weight;
    initialize();
}

void SelectGravityObject::initialize()
{
    _selectRadius = sqrt(_weight/2);
    _squareSelectRadius = _weight/2;
    _selected = false;
}

bool SelectGravityObject::isSelected() const
{
    return _selected;
}

double SelectGravityObject::getSelectRadius() const
{
    return _selectRadius;
}

double SelectGravityObject::getSquareSelectRadius() const
{
    return _squareSelectRadius;
}

bool SelectGravityObject::isInZone(double x, double y) const
{
    return Vector(x,y).squareDistanceTo(*_position) <= _squareSelectRadius;
}

void SelectGravityObject::setSelected(bool selected)
{
    _selected = selected;
}

void SelectGravityObject::draw(SDL_Surface* screen, const Parameters* params) const
{
    GravityObject::draw(screen,params);

    double x = _position->getX();
    double y = _position->getY();
    double w = (double)_weight;


    if (isSelected())
    {
        circleRGBA(screen, (x+params->screenX)/params->zoomFactor, (y+params->screenY)/params->zoomFactor, sqrt(w/2)/params->zoomFactor,(cos(SDL_GetTicks()/5000.0*2*params->pi)+1)/2*255*w/1000.0,
            (sin(SDL_GetTicks()/(7000.0*(1+w/1000.0))*2*params->pi)+1)/2*255,(cos(SDL_GetTicks()/11000.0*2*params->pi)+1)/2*255,(cos(SDL_GetTicks()/17000.0*2*params->pi)+1)/2*255);
    }
}


//TODO éviter de le refaire ?
//void SelectGravityObject::calculateGravityAcceleration(const vector<SelectGravityObject*> &universe)
//{
//    vector<GravityObject*> convertUniverse;
//
//    vector<SelectGravityObject*>::const_iterator gravObjPtr;
//    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
//    {
//        convertUniverse.push_back(dynamic_cast<GravityObject*> (*gravObjPtr));
//    }
//    GravityObject::calculateGravityAcceleration(convertUniverse);
//}
