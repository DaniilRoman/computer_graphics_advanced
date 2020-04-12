#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include "vtkPlane.h"
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkImplicitPlaneWidget2.h>
#include "vtkImplicitPlaneRepresentation.h"
#include "PlaneMoveCallback.h"

int main ( int argc, char *argv[] )
{
    std::string inputFilename = "../data/mask.stl";

    /* ================= Reader ================= */

    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(inputFilename.c_str());
    reader->Update();

    /* ================= Filter ================= */

    // Triangulate the grid points
//    vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter =
//            vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//    smoothFilter->SetInputConnection(reader->GetOutputPort());
//    smoothFilter->SetNumberOfIterations(2);
//    smoothFilter->SetRelaxationFactor(0.5);
//    smoothFilter->FeatureEdgeSmoothingOff();
//    smoothFilter->BoundarySmoothingOn();
//    smoothFilter->Update();
//
//    // Update normals on newly smoothed polydata
//    vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
//    normalGenerator->SetInputConnection(smoothFilter->GetOutputPort());
//    normalGenerator->ComputePointNormalsOn();
//    normalGenerator->ComputeCellNormalsOn();
//    normalGenerator->Update();

    /* Figure for cutting */

    vtkSmartPointer<vtkPlane> plane =
            vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(1.0, 1.5, 2.0);
    plane->SetNormal(0.4, 0.2, 1.0);

    /* ================= Mapper ================= */

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());
    mapper->AddClippingPlane(plane);

    /* ================= Actor ================= */

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    /* ================= Renderer ================= */

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(.0, .0, .0);

    /* ================= Window ================= */

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    /* ================= Interactor ================= */

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    /* The callback will do the work */
    vtkSmartPointer<PlaneMoveCallback> myCallback =
            vtkSmartPointer<PlaneMoveCallback>::New();
    myCallback->Plane = plane;
    myCallback->Actor = actor;

    vtkSmartPointer<vtkImplicitPlaneRepresentation> rep =
            vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
    rep->SetPlaceFactor(1.25); // This must be set prior to placing the widget
    rep->PlaceWidget(actor->GetBounds());
    rep->SetNormal(plane->GetNormal());
    rep->SetOrigin(plane->GetOrigin());

    vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidget =
            vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
    planeWidget->SetInteractor(renderWindowInteractor);
    planeWidget->SetRepresentation(rep);
    planeWidget->AddObserver(vtkCommand::InteractionEvent, myCallback);
    planeWidget->On();

    /* ================= Run pipeline ================= */

    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}