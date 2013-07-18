#ifndef S_GRAV_OBJ
#define S_GRAV_OBJ

#include "GravityObject.h"

class SelectGravityObject : public GravityObject
{
    public:
        SelectGravityObject();
        SelectGravityObject(int weight, Vector* position = new Vector(), Vector* speed = new Vector(), Vector* acceleration = new Vector());
        SelectGravityObject(const SelectGravityObject &gravObj);
        ~SelectGravityObject();

        virtual void setWeight(int weight);
        virtual double getSelectRadius() const;
        virtual double getSquareSelectRadius() const;
        virtual bool isInZone(double x, double y) const;
        virtual bool isSelected() const;
        virtual void setSelected(bool selected);

        virtual void draw(SDL_Surface* screen, const Parameters* params) const;

//        virtual void calculateGravityAcceleration(const std::vector<SelectGravityObject*> &universe);

    protected:
        double _selectRadius;
        double _squareSelectRadius;
        bool _selected;

    private:
        void initialize();
};
#endif
