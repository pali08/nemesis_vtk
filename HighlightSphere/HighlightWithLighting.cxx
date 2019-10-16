#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylindricalTransform.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMath.h>
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataSilhouette.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkWorldPointPicker.h>

namespace
{
// Handle mouse events
class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballCamera
{
public:
  static MouseInteractorHighLightActor* New();
  vtkTypeMacro(MouseInteractorHighLightActor,
      vtkInteractorStyleTrackballCamera);
  MouseInteractorHighLightActor()
  {
    //LastPickedProperty = vtkProperty::New();
  }

  virtual ~MouseInteractorHighLightActor()
  {
    //LastPickedProperty->Delete();
  }
  virtual void OnLeftButtonDown() override
  {
    int *clickPos = this->GetInteractor()->GetEventPosition();
    auto picker = //this->Interactor->GetPicker();
        vtkSmartPointer<vtkPropPicker>::New();
    picker->Pick(this->Interactor->GetEventPosition()[0], 
        this->Interactor->GetEventPosition()[1], 
        0,  // always zero.
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    if(picker->GetActor())
    {
       picker->GetActor()->GetProperty()->SetLighting(
          not picker->GetActor()->GetProperty()->GetLighting()
          );
    }
    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
  }

//private:
  //vtkProperty * LastPickedProperty;
  //std::vector<vtkActor *> PickedActors;
  //std::vector<vtkProperty *> PickedActorsProperty;
};
}
vtkStandardNewMacro(MouseInteractorHighLightActor);

