// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
#include "ipc/ipc_message_macros.h"
#include "ui/gfx/geometry/size_f.h"
#include "ui/gfx/geometry/vector2d_f.h"
#include "ui/gfx/ipc/gfx_param_traits.h"

#define IPC_MESSAGE_START QuickSilverMsgStart

IPC_MESSAGE_ROUTED2(QuickSilverMsg_ViewScrollParamsChanged,
		    gfx::SizeF /* size of the content in pixels */,
		    float /* total scale factor */)
