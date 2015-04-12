// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/common/content_client_qs.h"

#include "base/command_line.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/user_agent.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/l10n/l10n_util.h"

namespace QuickSilverImpl {

ContentClientQS::ContentClientQS()
    : content::ContentClient() {
}

std::string ContentClientQS::GetUserAgent() const {
  std::string product = "Chrome/" CHROMIUM_VERSION;
#if defined(OS_SAILFISH)
  std::string os_info = "Maemo; Linux; U; Jolla; Sailfish";
#else
  std::string os_info = "Linux; U";
#endif
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kUseMobileUserAgent)) {
    os_info += "; Android";
    product += " Mobile";
  }

  return content::BuildUserAgentFromOSAndProduct(os_info, product);
}

base::string16 ContentClientQS::GetLocalizedString(int message_id) const {
  return l10n_util::GetStringUTF16(message_id);
}

base::StringPiece ContentClientQS::GetDataResource(
    int resource_id, ui::ScaleFactor scale_factor) const {
  return ResourceBundle::GetSharedInstance().GetRawDataResourceForScale(
      resource_id, scale_factor);
}

base::RefCountedStaticMemory* ContentClientQS::GetDataResourceBytes(
    int resource_id) const {
  return ResourceBundle::GetSharedInstance().LoadDataResourceBytes(resource_id);
}

gfx::Image& ContentClientQS::GetNativeImageNamed(int resource_id) const {
  return ResourceBundle::GetSharedInstance().GetNativeImageNamed(resource_id);
}

}
