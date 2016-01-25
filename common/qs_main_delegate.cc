// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/common/qs_main_delegate.h"

#include "base/command_line.h"
#include "base/cpu.h"
#include "base/files/file_path.h"
#include "base/path_service.h"
#include "cc/base/switches.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "content/public/common/content_switches.h"
#include "ozone_qt/platform/ozone_qt_switches.h"
#include "quicksilver/browser/qs_content_browser_client.h"
#include "quicksilver/common/qs_profile.h"
#include "quicksilver/renderer/qs_content_renderer_client.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_switches.h"
#include "ui/native_theme/native_theme_switches.h"

using namespace content;

namespace QuickSilverImpl {

// XXX: Use kExtensionSheme if chromium extensions are ever supported.
const char kQuickSilverExtensionScheme[] = "quicksilver-extension";

namespace {

const base::FilePath::CharType kRendererExeName[] =
    FILE_PATH_LITERAL("quicksilver_worker");

base::FilePath GetRendererPath() {
  static base::FilePath path;
  if (path.empty()) {
    PathService::Get(base::DIR_EXE, &path);
    path = path.Append(kRendererExeName);
  }
  return path;
}

void InitLogging() {
  // Configure chromium logger
  logging::LoggingSettings settings;
  settings.logging_dest = logging::LOG_TO_SYSTEM_DEBUG_LOG;
  logging::InitLogging(settings);
  logging::SetLogItems(true, true, true, true);
}

} // namespace

QSMainDelegate::QSMainDelegate() {
}

QSMainDelegate::~QSMainDelegate() {
}

bool QSMainDelegate::BasicStartupComplete(int* exit_code) {
  InitLogging();

  base::CommandLine* cmdline = base::CommandLine::ForCurrentProcess();

  cmdline->AppendSwitch(switches::kInProcessGPU);
  cmdline->AppendSwitch(switches::kUIPrioritizeInGpuProcess);

  ProfileType profile = CurrentProfile();

  if (profile == ProfileType::MOBILE || profile == ProfileType::TABLET) {
    cmdline->AppendSwitch(switches::kMainFrameResizesAreOrientationChanges);
    cmdline->AppendSwitch(switches::kEnablePinch);
    cmdline->AppendSwitch(switches::kEnableOverlayScrollbar);
    cmdline->AppendSwitch(switches::kEnableGpuRasterization);
    cmdline->AppendSwitch(switches::kEnableLinkDisambiguationPopup);
    cmdline->AppendSwitch(switches::kEnableLowResTiling);
    cmdline->AppendSwitchASCII(switches::kScrollEndEffect, "1");
  }

#if defined(QUICKSILVER_DATA_DIR)
  base::FilePath path = base::FilePath(QUICKSILVER_DATA_DIR);
  PathService::Override(base::DIR_EXE, path);
#endif

  ContentSettingsPattern::SetNonWildcardDomainNonPortScheme(
      kQuickSilverExtensionScheme);

  content::SetContentClient(&content_client_);

  return false;
}

void QSMainDelegate::PreSandboxStartup() {
#if defined(ARCH_CPU_ARM_FAMILY) && defined(OS_LINUX)
  // Create an instance of the CPU class to parse /proc/cpuinfo and cache
  // cpu_brand info.
  base::CPU cpu_info;
#endif

  PathService::Override(base::FILE_EXE, GetRendererPath());
  base::CommandLine::ForCurrentProcess()->AppendSwitchPath(
      switches::kBrowserSubprocessPath, GetRendererPath());

  base::FilePath pak_file;
  PathService::Get(base::DIR_EXE, &pak_file);
  pak_file = pak_file.Append(FILE_PATH_LITERAL("quicksilver.pak"));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(pak_file);
}

//ContentBrowserClient* QSMainDelegate::CreateContentBrowserClient() {
//  browser_client_.reset((new QSContentBrowserClient));
//  return browser_client_.get();
//}

ContentRendererClient* QSMainDelegate::CreateContentRendererClient() {
  renderer_client_.reset((new QSContentRendererClient));
  return renderer_client_.get();
}

} // namespace QuickSilverImpl
