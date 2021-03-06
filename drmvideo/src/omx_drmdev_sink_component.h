/**
 * src/omx_drmdev_sink_component.h
 *
 * This OpenMAX component implements a video sink that uses the DRM to display
 * the incoming buffers. It has been based in the OpenMAX FBDEV sink component,
 * which has originally been developed by Peter Littlefield
 *
 * Copyright (C) 2007-2009  STMicroelectronics and Agere Systems
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 */

#ifndef _OMX_DRMDEV_SINK_COMPONENT_H_
#define _OMX_DRMDEV_SINK_COMPONENT_H_

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_Video.h>
#include <OMX_IVCommon.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include <bellagio/omx_base_video_port.h>
#include <bellagio/omx_base_sink.h>
#include <linux/fb.h>

#include "xf86drm.h"
#include "xf86drmMode.h"
#include <drm/exynos_drm.h>

/**  Filename of devnode for framebuffer device
  *  Should somehow be passed from client
  */

#define DRM_MODULE "exynos"
#define DRM_MAX_NUMBER_OF_BUFFERS 32
#define DRM_DEFAULT_NUMBER_OF_BUFFERS 2
#define DRM_CON_ID 11
#define DRM_CON_CRTC 3


/** DRMDEV sink port component port structure.
  */
DERIVEDCLASS(omx_drmdev_sink_component_PortType, omx_base_video_PortType)
#define omx_drmdev_sink_component_PortType_FIELDS omx_base_video_PortType_FIELDS \
  /** @param omxConfigCrop Crop rectangle of image */ \
  OMX_CONFIG_RECTTYPE omxConfigCrop; \
  /** @param omxConfigRotate Set rotation angle of image */ \
  OMX_CONFIG_ROTATIONTYPE omxConfigRotate; \
  /** @param omxConfigMirror Set mirroring of image */ \
  OMX_CONFIG_MIRRORTYPE omxConfigMirror; \
  /** @param omxConfigScale Set scale factors */ \
  OMX_CONFIG_SCALEFACTORTYPE omxConfigScale; \
  /** @param omxConfigOutputPosition Top-Left offset from intermediate buffer to output buffer */ \
  OMX_CONFIG_POINTTYPE omxConfigOutputPosition;
ENDCLASS(omx_drmdev_sink_component_PortType)

/** DRMDEV sink port component private structure.
  * see the define above
  * @param drm_fd The file descriptor for the framebuffer
  * @param vscr_info The fb_var_screeninfo structure for the framebuffer
  * @param fscr_info The fb_fix_screeninfo structure for the framebuffer
  * @param scr_ptr Pointer to the mmapped memory for the framebuffer
  * @param fbpxlfmt frame buffer pixel format
  * @param fbwidth frame buffer display width
  * @param fbheight frame buffer display height
  * @param fbbpp frame buffer pixel depth
  * @param fbstride frame buffer display stride
  * @param xScale the scale of the media clock
  * @param eState the state of the media clock
  * @param product frame buffer memory area
  * @param frameDropFlag the flag active on scale change indicates that frames are to be dropped
  * @param dropFrameCount counts the number of frames dropped
  * @param bDoubleBuffering double buffering flag
  * @param drm_resources resources of the DRM
  * @param drm_gem gem buffers
  * @param drm_mmap gem buffers mmaped address
  * @param drm_fb DRM framebuffer
  * @param drm_crtc crtc of the DRM
  * @param drm_num_of_bufs number of allocated buffers
  * @param drm_current_buf current buffer indicator
  * @param tunneledOutput tunneled output flag
  * @param lastDrmBuffer last DRM buffer indicator
  */
DERIVEDCLASS(omx_drmdev_sink_component_PrivateType, omx_base_sink_PrivateType)
#define omx_drmdev_sink_component_PrivateType_FIELDS omx_base_sink_PrivateType_FIELDS \
  int						   drm_fd; \
  int                          fd; \
  struct                       fb_var_screeninfo vscr_info; \
  struct                       fb_fix_screeninfo fscr_info; \
  unsigned char                *scr_ptr; \
  OMX_COLOR_FORMATTYPE         fbpxlfmt; \
  OMX_U32                      fbwidth; \
  OMX_U32                      fbheight; \
  OMX_U32                      fbbpp; \
  OMX_S32                      fbstride; \
  OMX_S32                      xScale; \
  OMX_TIME_CLOCKSTATE          eState; \
  OMX_U32                      product;\
  OMX_BOOL                     frameDropFlag;\
  int                          dropFrameCount; \
  OMX_BOOL                     bDoubleBuffering; \
  drmModeRes 		  		   *drm_resources; \
  struct drm_exynos_gem_create drm_gem[DRM_MAX_NUMBER_OF_BUFFERS]; \
  struct drm_exynos_gem_mmap   drm_mmap[DRM_MAX_NUMBER_OF_BUFFERS]; \
  unsigned int				   drm_fb[DRM_MAX_NUMBER_OF_BUFFERS]; \
  int						   drm_crtc[DRM_MAX_NUMBER_OF_BUFFERS]; \
  int						   drm_num_of_bufs; \
  int						   drm_current_buf; \
  OMX_BOOL					   tunneledOutput; \
  OMX_BUFFERHEADERTYPE* 	   lastDrmBuffer;
ENDCLASS(omx_drmdev_sink_component_PrivateType)

/* Component private entry points declaration */
OMX_ERRORTYPE omx_drmdev_sink_component_Constructor(OMX_COMPONENTTYPE *openmaxStandComp,OMX_STRING cComponentName);
OMX_ERRORTYPE omx_drmdev_sink_component_Destructor(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_drmdev_sink_component_Init(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_drmdev_sink_component_Deinit(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_drmdev_sink_component_MessageHandler(OMX_COMPONENTTYPE* , internalRequestMessageType*);

OMX_ERRORTYPE omx_drmdev_sink_component_port_SendBufferFunction(
  omx_base_PortType *openmaxStandPort,
  OMX_BUFFERHEADERTYPE* pBuffer);

/* to handle the communication at the clock port */
OMX_BOOL omx_drmdev_sink_component_ClockPortHandleFunction(
  omx_drmdev_sink_component_PrivateType* omx_drmdev_sink_component_Private,
  OMX_BUFFERHEADERTYPE* inputbuffer);

OMX_ERRORTYPE omx_drmdev_sink_component_SetConfig(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nIndex,
  OMX_PTR pComponentConfigStructure);

OMX_ERRORTYPE omx_drmdev_sink_component_GetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR ComponentParameterStructure);

OMX_ERRORTYPE omx_drmdev_sink_component_SetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR ComponentParameterStructure);

OMX_ERRORTYPE omx_drmdev_sink_component_GetConfig(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nIndex,
  OMX_PTR pComponentConfigStructure);

/** function prototypes of some internal functions */

/** finds OpenMAX standard pixel format from screen info */
OMX_COLOR_FORMATTYPE find_omx_pxlfmt(struct fb_var_screeninfo *vscr_info);

/** finds FFmpeg supported pixel format from input OpenMAX standard pixel format */
enum PixelFormat find_ffmpeg_pxlfmt(OMX_COLOR_FORMATTYPE omx_pxlfmt);

/** finds video stride  from input dimension and color format */
OMX_S32 calcStride(OMX_U32 width, OMX_COLOR_FORMATTYPE omx_pxlfmt);

/** image copy function */
void omx_img_copy(OMX_U8* src_ptr, OMX_S32 src_stride, OMX_U32 src_width, OMX_U32 src_height,
                  OMX_S32 src_offset_x, OMX_S32 src_offset_y,
                  OMX_U8* dest_ptr, OMX_S32 dest_stride, OMX_U32 dest_width,  OMX_U32 dest_height,
                  OMX_S32 dest_offset_x, OMX_S32 dest_offset_y,
                  OMX_S32 cpy_width, OMX_U32 cpy_height, OMX_COLOR_FORMATTYPE colorformat,OMX_COLOR_FORMATTYPE fbpxlfmt);

/** Returns a time value in milliseconds based on a clock starting at
 *  some arbitrary base. Given a call to GetTime that returns a value
 *  of n a subsequent call to GetTime made m milliseconds later should
 *  return a value of (approximately) (n+m). This method is used, for
 *  instance, to compute the duration of call. */
long GetTime();

OMX_ERRORTYPE omx_drmdev_sink_component_port_FlushProcessingBuffers(omx_base_PortType *openmaxStandPort);

OMX_ERRORTYPE omx_drmdev_sink_port_AllocateTunnelBuffer(
		omx_base_PortType *openmaxStandPort,
		OMX_U32 nPortIndex);

OMX_ERRORTYPE omx_drmdev_sink_port_ComponentTunnelRequest(omx_base_PortType* openmaxStandPort,
		OMX_HANDLETYPE hTunneledComp, OMX_U32 nTunneledPort, OMX_TUNNELSETUPTYPE* pTunnelSetup);

OMX_ERRORTYPE drm_input_port_AllocateTunnelBuffer(
		omx_base_PortType *openmaxStandPort,
		OMX_U32 nPortIndex);

OMX_ERRORTYPE drm_input_port_FreeTunnelBuffer(omx_base_PortType *openmaxStandPort,OMX_U32 nPortIndex);

/* Processing takes place in this function */
void* omx_drmdev_sink_BufferMgmtFunction (void* param);

#endif
