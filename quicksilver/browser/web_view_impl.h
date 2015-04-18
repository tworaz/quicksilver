// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_WEB_VIEW_IMPL_H_
#define QUICKSILVER_BROWSER_WEB_VIEW_IMPL_H_

#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/window_container_type.h"

namespace QuickSilver {
class WebView;
}

namespace QuickSilverImpl {

class WebViewImpl
    : public content::WebContentsDelegate,
      public content::WebContentsObserver {
 public:
  WebViewImpl(QuickSilver::WebView*);
  ~WebViewImpl() override;

  static QQuickItem* CreateNativeView(const content::WebContents*);

  void LoadURL(GURL url);
  void Reload(bool force);
  void Stop();
  void GoBackOrForward(int offset);

  const GURL& GetVisibleURL() const;
  const base::string16& GetTitle() const;
  bool CanGoForward() const;
  bool CanGoBack() const;
  bool IsLoading() const;
  double LoadProgress() const;

  // WebContentsDelegate overrides.
  void NavigationStateChanged(
      content::WebContents* source,
      content::InvalidateTypes changed_flags) override;
  bool ShouldCreateWebContents(
      content::WebContents* web_contents,
      int route_id,
      int main_frame_route_id,
      WindowContainerType window_container_type,
      const base::string16& frame_name,
      const GURL& target_url,
      const std::string& partition_id,
      content::SessionStorageNamespace* session_storage_namespace) override;
  bool WebContentsCreateAsync(WebContentsCreateCallback) override;
  void LoadProgressChanged(content::WebContents* source,
                           double progress) override;

  // WebContentsObserver overrides,
  void NavigationEntryCommitted(
      const content::LoadCommittedDetails& load_details) override;

 private:
  QuickSilver::WebView* web_view_;
  scoped_ptr<content::WebContents> web_contents_;

  bool can_go_forward_;
  bool can_go_back_;
  bool is_loading_;
  double load_progress_;

  DISALLOW_COPY_AND_ASSIGN(WebViewImpl);
};

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_BROWSER_WEB_VIEW_IMPL_H_
