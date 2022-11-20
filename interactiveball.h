#ifndef INTERACTIVEBALL_H
#define INTERACTIVEBALL_H


#include "vertex.h"
#include "material.h"
#include "oktaederball.h"


class InteractiveBall : public OktaederBall
{
public:
    InteractiveBall(float xPos = 0, float yPos = 0, float zPos = 0, int n = 1, QString name = "Ball", QString type = "InteractiveBall");


private:


};

#endif // INTERACTIVEBALL_H
