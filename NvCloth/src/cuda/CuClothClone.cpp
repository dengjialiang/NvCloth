// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2017 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.

#include "ClothClone.h"

#include "CuFactory.h"
#include "CuFabric.h"
#include "CuCloth.h"

using namespace physx;

namespace nv
{
namespace cloth
{
Range<const PhaseConfig> getPhaseConfigs(const CuCloth& cloth)
{
	return makeRange(cloth.mHostPhaseConfigs);
}
void setPhaseConfigs(CuCloth& cloth, Range<const PhaseConfig> phaseConfigs)
{
	cloth.setPhaseConfig(phaseConfigs);
}
Range<const PxVec4> getParticleAccelerations(const CuCloth& cloth)
{
	return makeRange(cloth.mParticleAccelerationsHostCopy);
}
Range<const uint32_t> getSelfCollisionIndices(const CuCloth& cloth)
{
	return makeRange(cloth.mSelfCollisionIndicesHost);
}

template <>
Cloth* ClothImpl<CuCloth>::clone(Factory& factory) const
{
	if (&mCloth.mFactory == &factory)
		return NV_CLOTH_NEW(ClothImpl<CuCloth>)(factory, *this); // copy construct directly

	switch(factory.getPlatform())
	{
	case Platform::CPU:
		return convertCloth(static_cast<SwFactory&>(factory), *this);
	case Platform::CUDA:
		return convertCloth(static_cast<CuFactory&>(factory), *this);
	case Platform::DX11:
	default:
		return NULL;
	}
}

Cloth* CuFactory::clone(const Cloth& cloth)
{
	if (cloth.getFactory().getPlatform() == Platform::CPU)
		return convertCloth(*this, static_cast<const SwClothImpl&>(cloth));

	return cloth.clone(*this);
}

} // namespace cloth
} // namespace nv
