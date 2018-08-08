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

#ifndef otbWrapperExpParameterGroup_h
#define otbWrapperExpParameterGroup_h

#include "otbWrapperExpTypes.h"
#include "otbWrapperExpParameter.h"
#include "OTBApplicationEngineExport.h"
#include "boost/core/noncopyable.hpp"

namespace otb
{
namespace WrapperExp
{

class OTBApplicationEngine_EXPORT ParameterGroup : 
public Parameter
{
public:
  typedef ParameterGroup               Self;
  typedef Parameter                    Superclass;
  typedef std::shared_ptr<Self>        Pointer;
  typedef std::shared_ptr<const Self>  ConstPointer;

  // itkNewMacro( Self );
  // static Pointer New() 
  //   {
  //   return std::shared_ptr<Self>( new Self() );
  //   }

  itkTypeMacro( ParameterGroup , Parameter );

  bool HasValue() const
  {return false;}

  ~ParameterGroup() override = default;
protected:
  ParameterGroup() = default ;

private:
};

} // end namespace WrapperExp
} // end namespace otb

#endif