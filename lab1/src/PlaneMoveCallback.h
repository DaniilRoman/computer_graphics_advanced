//
// Created by daniil on 13.04.2020.
//

#include <vtkImplicitPlaneWidget2.h>
#include "vtkImplicitPlaneRepresentation.h"
#include <vtkPlane.h>
#include <vtkActor.h>
#include "vtkCommand.h"

#ifndef VTK_TUTORIALS_PLANEMOVECALLBACK_H
#define VTK_TUTORIALS_PLANEMOVECALLBACK_H

class PlaneMoveCallback : public vtkCommand
{
public:
    static PlaneMoveCallback *New()
    {
        return new PlaneMoveCallback;
    }
    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        vtkImplicitPlaneWidget2 *planeWidget =
                reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
        vtkImplicitPlaneRepresentation *rep =
                reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
        rep->GetPlane(this->Plane);
    }

    PlaneMoveCallback() :Plane(0), Actor(0) {}
    vtkPlane *Plane;
    vtkActor *Actor;
};

#endif //VTK_TUTORIALS_PLANEMOVECALLBACK_H