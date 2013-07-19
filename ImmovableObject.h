#ifndef I_GRAV_OBJ
#define I_GRAV_OBJ

#include "SelectGravityObject.h"

class ImmovableObject : public SelectGravityObject
{
    public:
        ImmovableObject();
        ImmovableObject(int weight, Vector* position = new Vector(), Vector* speed = new Vector(), Vector* acceleration = new Vector());
        ImmovableObject(const ImmovableObject &gravObj);
        ~ImmovableObject();

        virtual Vector* getInfluenceFor(const GravityObject* gravObj);
        virtual void calculateGravityAcceleration(const std::vector<GravityObject*> &universe);
        virtual void calculateSpeed();
        virtual void calculatePosition();

};
#endif
