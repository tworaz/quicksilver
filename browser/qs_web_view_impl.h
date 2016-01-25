// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_BROWSER_QS_WEB_VIEW_IMPL_H_
#define _QUICKSILVER_BROWSER_QS_WEB_VIEW_IMPL_H_

#include <QtCore/QString>

#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/aura/window_observer.h"

namespace content {
class WebContents;
class RenderViewHost;
}

namespace QuickSilver {
class QSContextMenuParams;
class QSJavaScriptDialog;
class QSPermissionRequest;
class QSWebContext;
class QSWebView;
class QSWebViewDelegate;
class QSWebWindow;
}

namespace QuickSilverImpl {

class QSWebWindowImpl;
class QSJavaScriptDialogManager;

class QSWebViewImpl
    : public content::WebContentsDelegate
    , public content::WebContentsObserver
    , public aura::WindowObserver {

 using WebContents = content::WebContents;
 public:
  explicit QSWebViewImpl(QuickSilver::QSWebContext*, QuickSilver::QSWebView*);
  ~QSWebViewImpl() override;
  static QSWebViewImpl* From(WebContents*);
  static QSWebViewImpl* From(aura::Window*);

  void SetDelegate(QuickSilver::QSWebViewDelegate*);
  void SetWebWindow(QSWebWindowImpl*);

  void LoadURL(GURL url);
  void Reload(bool force = false);
  void Stop();
  void GoBackOrForward(int offset);
  void Focus();
  void Show();
  const GURL& GetVisibleURL() const;
  const base::string16& GetTitle() const;
  bool CanGoForward() const;
  bool CanGoBack() const;
  bool IsLoading() const;
  bool IsVisible() const;
  bool IsFullscreen() const;
  bool HasRenderView() const;
  double LoadProgress() const;
  double OverscrollY() const;
  gfx::Vector2dF ScrollOffset() const;
  gfx::SizeF WebContentSize() const;

  void ShowContextMenu(const QuickSilver::QSContextMenuParams&) const;
  bool ShowJavaScriptDialog(QuickSilver::QSJavaScriptDialog*) const;
  void CancelAllJavaScriptDialogs() const;
  bool RequestPermission(QuickSilver::QSPermissionRequest*) const;
  void CancelPermissionRequest(int request_id) const;

  // Out of memory handling.
  void DiscardRenderer();

  void WindowInsetsChanged();

  WebContents* web_contents() const;

 private:
  QSWebViewImpl(WebContents*);

  // WebContentsDelegate:
  void NavigationStateChanged(WebContents* source,
                              content::InvalidateTypes changed_flags) override;
  bool ShouldCreateWebContents(
      WebContents* web_contents,
      int route_id,
      int main_frame_route_id,
      WindowContainerType window_container_type,
      const std::string& frame_name,
      const GURL& target_url,
      const std::string& partition_id,
      content::SessionStorageNamespace* session_storage_namespace) override;
  void WebContentsCreated(WebContents* source_contents,
                          int opener_render_frame_id,
                          const std::string& frame_name,
                          const GURL& target_url,
                          WebContents* new_contents) override;
  void LoadProgressChanged(WebContents* source,
                           double progress) override;
  content::JavaScriptDialogManager* GetJavaScriptDialogManager(
      WebContents* source) override;
  bool CanOverscrollContent() const override;
  void OverscrollUpdate(float delta_y) override;
  void OverscrollComplete() override;
  void EnterFullscreenModeForTab(WebContents* web_contents,
                                 const GURL& origin) override;
  void ExitFullscreenModeForTab(WebContents*) override;
  bool IsFullscreenForTabOrPending(const WebContents*) const override;
  void CloseContents(WebContents* source) override;

  // WebContentsObserver:
  void NavigationEntryCommitted(
      const content::LoadCommittedDetails& load_details) override;
  void RenderViewReady() override;
  void RenderViewDeleted(content::RenderViewHost*) override;
  void RenderFrameHostChanged(content::RenderFrameHost* old_host,
                              content::RenderFrameHost* new_host) override;
  bool OnMessageReceived(const IPC::Message& message) override;
  void WasShown() override;

  // aura::WindowObserver:
  void OnWindowVisibilityChanged(aura::Window*, bool visible) override;

  // IPC message handlers
  void OnViewScrollParamsChanged(const gfx::SizeF&, const float&);

  void UpdateScrollOffset();

  QuickSilver::QSWebView* web_view_; // Not owned
  QuickSilver::QSWebViewDelegate* delegate_; // Not owned
  QSWebWindowImpl* web_window_; // Not owned
  scoped_ptr<QSJavaScriptDialogManager> js_dialog_manager_;
  scoped_ptr<WebContents> web_contents_;

  bool can_go_forward_;
  bool can_go_back_;
  bool is_loading_;
  bool visible_;
  bool fullscreen_;
  bool has_render_view_;
  double load_progress_;
  double overscroll_y_;
  float total_scale_factor_;
  gfx::Vector2dF scroll_offset_;
  gfx::SizeF web_content_size_;

  static std::map<WebContents*, QSWebViewImpl*> view_map_;

  DISALLOW_COPY_AND_ASSIGN(QSWebViewImpl);
};

} // namespace QuickSilverImpl

#endif // !_QUICKSILVER_BROWSER_QS_WEB_VIEW_IMPL_H_

