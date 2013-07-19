#ifndef SPRING_GRAV_OBJ
#define SPRING_GRAV_OBJ

#include "SelectGravityObject.h"
#include <map>

class SpringObject : public SelectGravityObject
{
    public:
        SpringObject();
        SpringObject(int weight, Vector* position = new Vector(), Vector* speed = new Vector(), Vector* acceleration = new Vector(), std::map<GravityObject*,std::vector<double> >* links = 0);
        SpringObject(const SpringObject &gravObj);
        ~SpringObject();

        virtual Vector* getInfluenceFor(const GravityObject* gravObj);
        virtual void calculateGravityAcceleration(const std::vector<GravityObject*> &universe);
        virtual std::map<GravityObject*,std::vector<double> >* getLinks() const;

    private:
        std::map<GravityObject*,std::vector<double> >* _links; //k, x0

};
#endif
