/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbHooverConfusionMatrix.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkLabelMap.h"
#include "itkLabelObject.h"
#include "itkLabelImageToLabelMapFilter.h"

int otbHooverConfusionMatrix(int argc, char* argv[])
{
  typedef itk::LabelObject<unsigned int,2>          LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>            LabelMapType;
  typedef otb::HooverConfusionMatrix<LabelMapType>  HooverConfusionMatrixType;
  typedef otb::Image<unsigned int, 2>               ImageType;
  typedef itk::LabelImageToLabelMapFilter
    <ImageType, LabelMapType>                       ImageToLabelMapFilterType;
  typedef otb::ImageFileReader<ImageType>           ImageReaderType;
  typedef HooverConfusionMatrixType::MatrixType     MatrixType;
  
  if(argc != 4)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " segmentationGT segmentationMS HooverMatrix.txt" << std::endl;
    return EXIT_FAILURE;
    }

  ImageReaderType::Pointer gt_reader = ImageReaderType::New();
  gt_reader->SetFileName(argv[1]);
  
  ImageReaderType::Pointer ms_reader = ImageReaderType::New();
  ms_reader->SetFileName(argv[2]);
  
  ImageToLabelMapFilterType::Pointer gt_filter = ImageToLabelMapFilterType::New();
  gt_filter->SetInput(gt_reader->GetOutput());
  gt_filter->SetBackgroundValue(0);
  
  ImageToLabelMapFilterType::Pointer ms_filter = ImageToLabelMapFilterType::New();
  ms_filter->SetInput(ms_reader->GetOutput());
  ms_filter->SetBackgroundValue(0);
  
  HooverConfusionMatrixType::Pointer hooverFilter = HooverConfusionMatrixType::New();
  hooverFilter->SetGroundTruthLabelMap(gt_filter->GetOutput());
  hooverFilter->SetMachineSegmentationLabelMap(ms_filter->GetOutput());
  
  hooverFilter->Update();
  
  std::ofstream outputFile;
  outputFile.open(argv[3]);
  
  MatrixType &mat = hooverFilter->GetHooverConfusionMatrix();
  unsigned int n = mat.Rows(), p = mat.Cols();
  for (int i=0;i<n;i++)
    {
    for (int j=0;j<p;j++)
      {
      outputFile << mat(i,j);
      if (j == (p-1))
        {
        outputFile << "\n";
        }
      else
        {
        outputFile << "\t";
        }
      }
    }
  
  return EXIT_SUCCESS;
}
