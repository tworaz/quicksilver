// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/common/content_main_delegate_qs.h"

#include "base/command_line.h"
#include "base/cpu.h"
#include "base/path_service.h"
#include "base/files/file_path.h"
#include "cc/base/switches.h"
#include "content/public/common/content_switches.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/native_theme/native_theme_switches.h"
#include "quicksilver/browser/content_browser_client_qs.h"
#include "quicksilver/renderer/content_renderer_client_qs.h"

using namespace content;

namespace QuickSilverImpl {

namespace {

const base::FilePath::CharType kRendererExeName[] =
    FILE_PATH_LITERAL("quicksilver_renderer");

base::FilePath GetRendererPath() {
  static base::FilePath path;
  if (path.empty()) {
    PathService::Get(base::DIR_EXE, &path);
    path = path.Append(kRendererExeName);
  }
  return path;
}

} // namespace

ContentMainDelegateQS::ContentMainDelegateQS() {
}

ContentMainDelegateQS::~ContentMainDelegateQS() {
}

bool ContentMainDelegateQS::BasicStartupComplete(int* exit_code) {
  base::CommandLine* cmdline = base::CommandLine::ForCurrentProcess();
  cmdline->AppendSwitch(switches::kNoSandbox);

  cmdline->AppendSwitch(switches::kUIPrioritizeInGpuProcess);
  cmdline->AppendSwitch(cc::switches::kDisableCompositedAntialiasing);

#if defined(OS_SAILFISH)
  cmdline->AppendSwitch(switches::kEnableViewportMeta);
  cmdline->AppendSwitch(switches::kUseMobileUserAgent);
  cmdline->AppendSwitch(switches::kMainFrameResizesAreOrientationChanges);
  cmdline->AppendSwitch(switches::kEnablePinch);
#endif

  if (cmdline->HasSwitch(switches::kEnableViewportMeta))
    cmdline->AppendSwitch(switches::kEnableOverlayScrollbar);

#if defined(QUICKSILVER_DATA_DIR)
  base::FilePath path = base::FilePath(QUICKSILVER_DATA_DIR);
  PathService::Override(base::DIR_EXE, path);
#endif

  return false;
}

void ContentMainDelegateQS::PreSandboxStartup() {
  // Create an instance of the CPU class to parse /proc/cpuinfo and cache
  // cpu_brand info.
  base::CPU cpu_info;

  PathService::Override(base::FILE_EXE, GetRendererPath());
  base::CommandLine::ForCurrentProcess()->AppendSwitchPath(
      switches::kBrowserSubprocessPath, GetRendererPath());

  base::FilePath pak_file;
  PathService::Get(base::DIR_EXE, &pak_file);
  pak_file = pak_file.Append(FILE_PATH_LITERAL("quicksilver.pak"));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(pak_file);
}

ContentBrowserClient* ContentMainDelegateQS::CreateContentBrowserClient() {
  browser_client_.reset((new ContentBrowserClientQS));
  return browser_client_.get();
}

ContentRendererClient* ContentMainDelegateQS::CreateContentRendererClient() {
  renderer_client_.reset((new ContentRendererClientQS));
  return renderer_client_.get();
}

} // namespace QuickSilverImpl
