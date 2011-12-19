
/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


    This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageViewerManagerControllerInterface_h
#define __otbImageViewerManagerControllerInterface_h

#include "otbImageViewerFullResolutionEventsInterface.h"
#include "otbImageViewerManagerModel.h"


/** NewVisu */
#include "otbImageWidgetController.h"
#include "otbImageWidget.h"

#include "otbObjectList.h"

namespace otb
{
class ITK_EXPORT  ImageViewerManagerControllerInterface
      : public ImageViewerFullResolutionEventsInterface
{
public:
  /** Standard class typedefs */
  typedef ImageViewerManagerControllerInterface     Self;
  typedef ImageViewerFullResolutionEventsInterface  Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;
  

  typedef ImageViewerManagerModel                   ModelType;
  typedef ModelType::VisuModelType                  VisuModelType;        //RenderingImageLayer
  typedef ModelType::VisuModelPointerType           VisuModelPointerType;
  typedef ModelType::ImageType                      ImageType;
  typedef ModelType::ImagePointerType               ImagePointerType;
  typedef ModelType::OffsetType                     OffsetType;


/** NewVisu */
  typedef ImageWidgetController                     VisuControllerType;
  typedef VisuControllerType::Pointer               VisuControllerPointerType;
  
  typedef ObjectList<VisuControllerType>            VisuControllerListType;
  typedef VisuControllerListType::Pointer           VisuControllerListPointerType;
  
  typedef ImageWidgetController                     WidgetControllerType;
  typedef WidgetControllerType::Pointer             WidgetControllerPointerType;
  
  typedef ImageWidget<>                             WidgetType;
  typedef WidgetType::Pointer                       VisuWidgetPointerType;

  /** Standard type macros */
  itkTypeMacro(ImageViewerManagerControllerInterface, Superclass);

  /** Users actions */
  virtual unsigned int OpenInputImage(const char * filename) =0;
  virtual void CloseImage(unsigned int selectedItem) = 0;
  virtual void UpdateRGBChannelOrder(int redChoice , int greenChoice, int BlueChoice, unsigned int selectedItem) = 0;
  virtual void UpdateGrayScaleChannelOrder(int choice, unsigned int selectedItem) = 0;
  virtual void UpdateAmplitudeChannelOrder(int realChoice , int imChoice, unsigned int selectedItem) = 0;
  virtual void UpdatePhaseChannelOrder(int realChoice , int imChoice, unsigned int selectedItem) = 0;
  virtual void Link(unsigned int leftChoice, unsigned int rightChoice , OffsetType offset) = 0;
  virtual void UpdateImageViewController(unsigned int selectedItem) = 0;
  /** NewVisu */
  virtual VisuControllerPointerType GetVisuController()=0;
  virtual VisuControllerPointerType GetPreviewVisuController() = 0;

  

protected:
  /** Constructor */
  ImageViewerManagerControllerInterface()
  {
    m_Model = ModelType::GetInstance();
  }
  /** Destructor */
  ~ImageViewerManagerControllerInterface() {};

  /** The instance of the model (for commodity) */
  ModelType * m_Model;

private:
  ImageViewerManagerControllerInterface(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
} // end namespace otb

#endif
