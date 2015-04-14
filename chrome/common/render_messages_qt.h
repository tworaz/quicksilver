// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Multiply-included file, no traditional include guard.
#include <string>

#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "url/gurl.h"

#define IPC_MESSAGE_START ChromeMsgStart

// JavaScript related messages -----------------------------------------------

// Provides the renderer with the results of the browser's investigation into
// why a recent main frame load failed (currently, just DNS probe result).
// NetErrorHelper will receive this mesage and replace or update the error
// page with more specific troubleshooting suggestions.
IPC_MESSAGE_ROUTED1(ChromeViewMsg_NetErrorInfo,
                    int /* DNS probe status */)

// Provides the information needed by the renderer process to contact a
// navigation correction service.  Handled by the NetErrorHelper.
IPC_MESSAGE_ROUTED5(ChromeViewMsg_SetNavigationCorrectionInfo,
                    GURL /* Navigation correction service base URL */,
                    std::string /* language */,
                    std::string /* origin_country */,
                    std::string /* API key to use */,
                    GURL /* Google Search URL to use */)
