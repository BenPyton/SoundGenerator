/*
* @author PELLETIER Benoit
*
* @file SquareComponent.h
*
* @date 19/01/2019
*
* @brief Implementation of squared signal generator component
*
*/

#ifndef SQUARECOMPONENT_H
#define SQUARECOMPONENT_H

#include "GeneratorComponent.h"

class SquareComponent : public GeneratorComponent
{
public:
    SquareComponent();
    virtual qreal getOutput(qreal _time) override;
};

#endif // SQUARECOMPONENT_H
