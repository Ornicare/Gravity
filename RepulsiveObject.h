#ifndef R_GRAV_OBJ
#define R_GRAV_OBJ

#include "SelectGravityObject.h"

class RepulsiveObject : public SelectGravityObject
{
    public:
        RepulsiveObject();
        RepulsiveObject(int weight, Vector* position = new Vector(), Vector* speed = new Vector(), Vector* acceleration = new Vector());
        RepulsiveObject(const RepulsiveObject &gravObj);
        ~RepulsiveObject();

        virtual Vector* getInfluenceFor(const GravityObject* gravObj);
        virtual void calculateGravityAcceleration(const std::vector<GravityObject*> &universe);

};
#endif
