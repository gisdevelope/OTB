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
#ifndef __otbMPIConfig_txx
#define __otbMPIConfig_txx

#include "otbMPIConfig.h"
#include "otbImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "otbNumberOfDivisionsTiledStreamingManager.h"
#include <vector>
#include <iostream>
#include <sstream>

namespace otb {

namespace mpi {

// Update MPI
template <typename TImage> void MPIConfig::UpdateMPI (TImage *img, const std::string &prefix, bool useStreaming=true, bool writeVRTFile=false, const std::string xtArgs = "") 
{
  typedef otb::ImageFileWriter<TImage>                                           WriterType;
  typedef otb::NumberOfDivisionsTiledStreamingManager<TImage>                    StreamingManagerType;
  typedef itk::RegionOfInterestImageFilter<TImage, TImage>                       ExtractFilterType;

  // First, update infomration from image to write
  img->UpdateOutputInformation();

  // Configure streaming manager
  typename StreamingManagerType::Pointer streamingManager = StreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(m_NbProcs);
  streamingManager->PrepareStreaming(img,img->GetLargestPossibleRegion());
  unsigned int numberOfSplits = streamingManager->GetNumberOfSplits();

  // This vector will hold all regions to write for current rank
  std::vector<typename TImage::RegionType> regionsToWrite;

  // Handle both cases when there are much more (resp. less) region to
  // write than NbProcs
  if(m_MyRank < numberOfSplits)
  {
    unsigned int splitIdx = m_MyRank;
    while(splitIdx < numberOfSplits)
    {
      typename TImage::RegionType currentRegion = streamingManager->GetSplit(splitIdx);
      regionsToWrite.push_back(currentRegion);
      splitIdx+=m_NbProcs;
    }
  }

  // Now write all the regions
  for(typename std::vector<typename TImage::RegionType>::const_iterator it = regionsToWrite.begin();
      it!=regionsToWrite.end();++it)
  {
    typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
    extractFilter->SetInput(img);
    extractFilter->SetRegionOfInterest(*it);
    // Writer
    std::stringstream ss;
    ss<<prefix<<"_"<<it->GetIndex()[0]<<"_"<<it->GetIndex()[1]<<"_"<<it->GetSize()[0]<<"_"<<it->GetSize()[1]<<".tif"<<xtArgs;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(ss.str());
    writer->SetInput(extractFilter->GetOutput());

    if (!useStreaming)
    {
      writer->SetNumberOfDivisionsTiledStreaming(0);
    }
    // Pipeline execution
    try
    {
      writer->Update();
      if (writeVRTFile && (m_MyRank == 0))
      {
        // Write VRT File
        std::stringstream vrtfOut;
        vrtfOut<<prefix<<".vrt";

        int imageSizeX = img->GetLargestPossibleRegion().GetSize()[0];
        int imageSizeY = img->GetLargestPossibleRegion().GetSize()[1];

        std::ofstream ofs(vrtfOut.str().c_str());

        ofs<<"<VRTDataset rasterXSize=\""<<imageSizeX<<"\" rasterYSize=\""<<imageSizeY<<"\">"<<std::endl;

        const unsigned int nbBands = img->GetNumberOfComponentsPerPixel();

        for(unsigned int band = 1; band<=nbBands;++band)
        {
          ofs<<"\t<VRTRasterBand dataType=\"UInt32\" band=\""<<band<<"\">"<<std::endl;
          ofs<<"\t\t<ColorInterp>Gray</ColorInterp>"<<std::endl;

          typename TImage::RegionType currentRegion;
          for(unsigned int id=0; id < numberOfSplits; ++id)
          {
            currentRegion = streamingManager->GetSplit(id);
            int tileSizeX = currentRegion.GetSize()[0];
            int tileSizeY = currentRegion.GetSize()[1];
            int row = currentRegion.GetIndex()[0];
            int column = currentRegion.GetIndex()[1];
            std::stringstream tileFileName;
            tileFileName <<prefix<<"_"<<row<<"_"<<column<<"_"<<tileSizeX<<"_"<<tileSizeY<<".tif"<<xtArgs;
            ofs<<"\t\t<SimpleSource>"<<std::endl;
            ofs<<"\t\t\t<SourceFilename relativeToVRT=\"1\">"<< tileFileName.str()<<"</SourceFilename>"<<std::endl;
            ofs<<"\t\t\t<SourceBand>"<<band<<"</SourceBand>"<<std::endl;
            ofs<<"\t\t\t<SrcRect xOff=\""<<0<<"\" yOff=\""<<0<<"\" xSize=\""<<tileSizeX<<"\" ySize=\""<<tileSizeY<<"\"/>"<<std::endl;
            ofs<<"\t\t\t<DstRect xOff=\""<<row<<"\" yOff=\""<<column<<"\" xSize=\""<<tileSizeX<<"\" ySize=\""<<tileSizeY<<"\"/>"<<std::endl;
            ofs<<"\t\t</SimpleSource>"<<std::endl;
          }
          ofs<<"\t</VRTRasterBand>"<<std::endl;
        }
        ofs<<"</VRTDataset>"<<std::endl;
        ofs.close();
      }
    }
    catch (itk::ExceptionObject& err)
    {
      std::stringstream message;
      message << "ExceptionObject caught: " << err << std::endl;
      this->logError(message.str());
      this->abort(EXIT_FAILURE);
    }
  }
}


} // End namespece mpi

} // End namespace otb
#endif //__otbMPIConfig_txx

