/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbOGRVersionProxy_h
#define otbOGRVersionProxy_h

#include <string>
#include <vector>
#include "otbConfigure.h"
#include "itkMacro.h"

#if defined(_MSC_VER)
#pragma warning ( push )
#pragma warning ( disable: 4251 )
#include "ogr_core.h" // OGR enums
#pragma warning ( pop )
#else
#include "ogr_core.h" // OGR enums
#endif

class GDALDataset;
class GDALDriver;
class OGRFeature;

#include "OTBGdalAdaptersExport.h"

namespace otb
{
namespace ogr
{
namespace version_proxy
{

/** 
 * This namespace holds proxy functions hiding interface changes in gdal 2.0 
 *
 * This namespace holds proxy functions hiding interface changes in OGR
 * dataset between gdal 1.x (x>10) and gdal 2.x. It defines a common
 * interface that should be used in place of calling directly the
 * wrapped gdal functions.
 * 
 * Whenever GDALDataset and GDALDriver have to be used to open a
 * vector dataset (or OGRDataSource an OGRSFDriver for gdal 1.x), one
 * should use ogr::version_proxy types GDALDataset and
 * GDALDriver.
 * 
 * See function documentation for details.
 */

  /**
   * Return the list of files composing the dataset.
   * 
   * Calls OGRDataSource::GetName() and wrap in string vector for gdal
   * 1.x implementation, and GDALDataset::GetFileList and wrap in
   * string vector for gdal 2.x implementation.
   *  
   * \param dataset Pointer to the dataset to get the file list from. Will not be
   * checked for null pointer.
   * 
   * \return A vector of string containing the list of files.
   */

}
}
} // end namespace otb

#endif
