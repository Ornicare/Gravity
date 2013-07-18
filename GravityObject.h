#ifndef GRAV_OBJ
#define GRAV_OBJ

#include <iostream>
#include <string>
#include <vector>
#include "Vector.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "Parameters.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "Vector.h"
#include <SDL/SDL_gfxPrimitives.h>
#include <cmath>
#include <sstream>
#include <SDL/SDL_rotozoom.h>
#include "Parameters.h"

class GravityObject
{
    public:
        GravityObject();
        GravityObject(int weight, Vector* position = new Vector(), Vector* speed = new Vector(), Vector* acceleration = new Vector());
        GravityObject(const GravityObject &gravObj);
        ~GravityObject();

        virtual int getWeight() const;
        virtual void setWeight(int weight);

        virtual Vector* getPosition() const;
        virtual Vector* getSpeed() const;
        virtual void addSpeed(Vector v);
        virtual void addAcceleration(Vector v);
        virtual Vector* getAcceleration() const;

        virtual void calculateGravityAcceleration(const std::vector<GravityObject*> &universe);
        virtual void calculateSpeed();
        virtual void calculatePosition();

        virtual Vector* getInfluenceFor(const GravityObject* gravObj);

        virtual void draw(SDL_Surface* screen, const Parameters* params) const;

        static const double G = 10e-5;

    protected:
        int _weight;
        Vector* _temporaryAcceleration;
        Vector* _position;
        Vector* _speed;
        Vector* _acceleration;

};
#endif
