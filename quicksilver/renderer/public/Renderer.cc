// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/renderer/public/Renderer.h"

#include "base/command_line.h"
#include "content/public/app/content_main.h"
#include "quicksilver/common/content_client_qs.h"
#include "quicksilver/common/content_main_delegate_qs.h"

namespace QuickSilver {

int RendererMain(int argc, const char* argv[]) {
  QuickSilverImpl::ContentClientQS content_client;
  QuickSilverImpl::ContentMainDelegateQS delegate;

  base::CommandLine::Init(argc, argv);
  content::SetContentClient(&content_client);

  content::ContentMainParams params(&delegate);
  params.argc = argc;
  params.argv = argv;

  return content::ContentMain(params);
}

} // namespace QuickSilver
