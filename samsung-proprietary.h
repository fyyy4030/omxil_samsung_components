/*
 * samsung-proprietary.h
 *
 *  Created on: Jan 4, 2012
 *      Author: Kamil Debski <k.debski@samsung.com>
 */

#ifndef SAMSUNGPROPRIETARY_H_
#define SAMSUNGPROPRIETARY_H_

typedef struct {
	OMX_U8 *yPlane;
	OMX_U8 *uvPlane;
	OMX_U32 bufferIndex;
	OMX_U32 yPlaneSize;
	OMX_U32 uvPlaneSize;
} SAMSUNG_NV12MT_BUFFER;

typedef struct {
        OMX_U8 *yPlane;
        OMX_U8 *uPlane;
	OMX_U8 *vPlane;
        OMX_U32 bufferIndex;
        OMX_U32 yPlaneSize;
        OMX_U32 uPlaneSize;
	OMX_U32 vPlaneSize;
} SAMSUNG_YUV420M_BUFFER;

typedef union
{
	SAMSUNG_NV12MT_BUFFER nv12mt;
	SAMSUNG_YUV420M_BUFFER yuv420m;
} SAMSUNG_PLANE_BUFFER;


#endif /* SAMSUNGPROPRIETARY_H_ */
