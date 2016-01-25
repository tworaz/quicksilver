// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_web_view_impl.h"

#include "content/browser/renderer_host/render_widget_host_view_aura.h"
#include "content/common/view_messages.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/renderer_preferences.h"
#include "ozone_qt/util/type_conversion.h"
#include "quicksilver/browser/qs_browser_context.h"
#include "quicksilver/browser/qs_javascript_dialog_manager.h"
#include "quicksilver/browser/qs_oom_manager.h"
#include "quicksilver/browser/qs_web_window_impl.h"
#include "quicksilver/common/qs_messages.h"
#include "quicksilver/public/browser/qs_web_context.h"
#include "quicksilver/public/browser/qs_web_view.h"
#include "quicksilver/public/browser/qs_web_view_delegate.h"
#include "quicksilver/public/browser/qs_web_window.h"
#include "ui/aura/window.h"
#include "ui/aura/window_property.h"
#include "ui/gfx/screen.h"

using namespace content;
using namespace QuickSilver;

DECLARE_WINDOW_PROPERTY_TYPE(QuickSilverImpl::QSWebViewImpl*);

namespace QuickSilverImpl {

DEFINE_WINDOW_PROPERTY_KEY(QSWebViewImpl*, kWindowQuickSilverView, nullptr);

std::map<content::WebContents*, QSWebViewImpl*> QSWebViewImpl::view_map_;

namespace {

class QSRendererPreferences : public RendererPreferences {
 public:
  QSRendererPreferences() : RendererPreferences() {
    tap_multiple_targets_strategy = TAP_MULTIPLE_TARGETS_STRATEGY_ZOOM;
    enable_do_not_track = true;
  }
};

static QSRendererPreferences g_renderer_preferences;

} // namespace


QSWebViewImpl::QSWebViewImpl(QSWebContext* context, QSWebView* wv)
    : web_view_(wv)
    , delegate_(nullptr)
    , web_window_(nullptr)
    , js_dialog_manager_(nullptr)
    , can_go_forward_(false)
    , can_go_back_(false)
    , is_loading_(false)
    , visible_(false)
    , fullscreen_(false)
    , has_render_view_(false)
    , load_progress_(0.f)
    , overscroll_y_(0.f)
    , scroll_offset_(gfx::Vector2dF(0.f, 0.f)) {
  WebContents::CreateParams params(context->impl());
  web_contents_.reset(WebContents::Create(params));
  web_contents_->SetRendererPreferences(g_renderer_preferences);
  web_contents_->SetDelegate(this);
  web_contents_->GetNativeView()->AddObserver(this);
  web_contents_->GetNativeView()->SetProperty(kWindowQuickSilverView, this);
  this->Observe(web_contents_.get());
  QSOomManager::GetInstance()->AddWebView(this);
  visible_ = web_contents_->GetNativeView()->IsVisible();

  DCHECK(view_map_.find(web_contents_.get()) == view_map_.end());
  view_map_[web_contents_.get()] = this;

  total_scale_factor_ = gfx::Screen::GetNativeScreen()->
      GetPrimaryDisplay().device_scale_factor();
}

QSWebViewImpl::QSWebViewImpl(content::WebContents* wc)
    : web_view_(nullptr)
    , delegate_(nullptr)
    , web_window_(nullptr)
    , web_contents_(wc)
    , js_dialog_manager_(nullptr)
    , can_go_forward_(false)
    , can_go_back_(false)
    , is_loading_(false)
    , visible_(wc->GetNativeView()->IsVisible())
    , fullscreen_(false)
    , has_render_view_(wc->GetRenderViewHost() != nullptr)
    , load_progress_(0.f)
    , overscroll_y_(0.f)
    , scroll_offset_(gfx::Vector2dF(0.f, 0.f)) {
  web_contents_->SetRendererPreferences(g_renderer_preferences);
  web_contents_->SetDelegate(this);
  web_contents_->GetNativeView()->AddObserver(this);
  this->Observe(web_contents_.get());
  QSOomManager::GetInstance()->AddWebView(this);

  DCHECK(view_map_.find(web_contents_.get()) == view_map_.end());
  view_map_[web_contents_.get()] = this;

  total_scale_factor_ = gfx::Screen::GetNativeScreen()->
      GetPrimaryDisplay().device_scale_factor();
}

QSWebViewImpl::~QSWebViewImpl() {
  web_contents_->GetNativeView()->RemoveObserver(this);
  DCHECK(view_map_.find(web_contents_.get()) != view_map_.end());
  view_map_.erase(web_contents_.get());
  QSOomManager::GetInstance()->RemoveWebView(this);
  web_view_ = nullptr;
  delegate_ = nullptr;
  web_window_ = nullptr;
}

//static
QSWebViewImpl* QSWebViewImpl::From(WebContents* wc) {
  if (view_map_.find(wc) == view_map_.end()) {
    return nullptr;
  }
  return view_map_[wc];
}

//static
QSWebViewImpl* QSWebViewImpl::From(aura::Window* win) {
  return win->GetProperty(kWindowQuickSilverView);
}

void QSWebViewImpl::SetDelegate(QSWebViewDelegate* delegate) {
  delegate_ = delegate;
}

void QSWebViewImpl::SetWebWindow(QSWebWindowImpl* ww) {
  web_window_ = ww;
  WindowInsetsChanged();
  if (web_content_size_.IsEmpty()) {
    web_content_size_ = gfx::RectF(ww->GetBounds()).size();
  }
}

void QSWebViewImpl::LoadURL(GURL url) {
  DCHECK(url.is_valid());
  NavigationController::LoadURLParams params(url);
  params.transition_type = ui::PageTransitionFromInt(
      ui::PAGE_TRANSITION_TYPED | ui::PAGE_TRANSITION_FROM_ADDRESS_BAR);
  web_contents_->GetController().LoadURLWithParams(params);
  web_view_->currentURLChanged();
}

void QSWebViewImpl::Reload(bool force) {
  web_contents_->GetController().Reload(force);
  Focus();
}

void QSWebViewImpl::Stop() {
  web_contents_->Stop();
  Focus();
}

void QSWebViewImpl::GoBackOrForward(int offset) {
  web_contents_->GetController().GoToOffset(offset);
  Focus();
}

void QSWebViewImpl::Focus() {
  web_contents_->Focus();
}

void QSWebViewImpl::Show() {
  web_contents_->GetNativeView()->Show();
  Focus();
}

const GURL& QSWebViewImpl::GetVisibleURL() const {
  return web_contents_->GetVisibleURL();
}

const base::string16& QSWebViewImpl::GetTitle() const {
  return web_contents_->GetTitle();
}

bool QSWebViewImpl::CanGoForward() const {
  return can_go_forward_;
}

bool QSWebViewImpl::CanGoBack() const {
  return can_go_back_;
}

bool QSWebViewImpl::IsLoading() const {
  return is_loading_;
}

bool QSWebViewImpl::IsVisible() const {
  return visible_;
}

bool QSWebViewImpl::IsFullscreen() const {
  return fullscreen_;
}

bool QSWebViewImpl::HasRenderView() const {
  return has_render_view_;
}

double QSWebViewImpl::LoadProgress() const {
  return load_progress_;
}

double QSWebViewImpl::OverscrollY() const {
  return overscroll_y_;
}

gfx::Vector2dF QSWebViewImpl::ScrollOffset() const {
  return scroll_offset_;
}

gfx::SizeF QSWebViewImpl::WebContentSize() const {
  return web_content_size_;
}

void QSWebViewImpl::ShowContextMenu(const QSContextMenuParams& params) const {
  if (delegate_) {
    delegate_->ShowContextMenu(web_view_, params);
  }
}

bool QSWebViewImpl::ShowJavaScriptDialog(QSJavaScriptDialog* dialog) const {
  if (delegate_) {
    return delegate_->ShowJavaScriptDialog(web_view_, dialog);
  }
  return false;
}

void QSWebViewImpl::CancelAllJavaScriptDialogs() const {
  if (delegate_) {
    delegate_->CancelAllJavaScriptDialogs(web_view_);
  }
}

bool QSWebViewImpl::RequestPermission(QSPermissionRequest* request) const {
  if (delegate_) {
    return delegate_->RequestPermission(web_view_, request);
  }
  return false;
}

void QSWebViewImpl::CancelPermissionRequest(int request_id) const {
  if (delegate_) {
    delegate_->CancelPermissionRequest(web_view_, request_id);
  }
}

void QSWebViewImpl::DiscardRenderer() {
  DCHECK(!has_render_view_);

  VLOG(1) << "Discarding renderer for: " << GetVisibleURL();

  WebContents* null_contents = WebContents::Create(
      WebContents::CreateParams(web_contents_->GetBrowserContext()));
  null_contents->GetController().CopyStateFrom(web_contents_->GetController());
  null_contents->SetLastActiveTime(web_contents_->GetLastActiveTime());

  null_contents->SetRendererPreferences(g_renderer_preferences);
  null_contents->SetDelegate(this);
  null_contents->GetNativeView()->AddObserver(this);

  DCHECK(view_map_.find(web_contents_.get()) != view_map_.end());
  view_map_.erase(web_contents_.get());
  web_contents_->GetNativeView()->RemoveObserver(this);

  web_contents_.reset(null_contents);
  this->Observe(web_contents_.get());
  view_map_[web_contents_.get()] = this;

  web_window_->AddView(this);
}

void QSWebViewImpl::WindowInsetsChanged() {
  if (!web_window_) {
    return;
  }
  RenderWidgetHostViewAura* rwha = static_cast<RenderWidgetHostViewAura*>(
      web_contents_->GetRenderWidgetHostView());
  if (rwha) {
    rwha->SetInsets(web_window_->GetInsets());
  }
}

WebContents* QSWebViewImpl::web_contents() const {
  return web_contents_.get();
}

void QSWebViewImpl::NavigationStateChanged(
    WebContents* wc, InvalidateTypes flags) {
  DCHECK_EQ(wc, web_contents_.get());
  if (flags & INVALIDATE_TYPE_URL)
    web_view_->currentURLChanged();
  if (flags & INVALIDATE_TYPE_TITLE)
    web_view_->titleChanged();
  if (flags & INVALIDATE_TYPE_LOAD) {
    if (is_loading_ != web_contents_->IsLoading()) {
      is_loading_ = !is_loading_;
      web_view_->loadingChanged(is_loading_);
    }
  }
}

bool QSWebViewImpl::ShouldCreateWebContents(
    content::WebContents* web_contents,
    int route_id,
    int main_frame_route_id,
    WindowContainerType window_container_type,
    const std::string& frame_name,
    const GURL& target_url,
    const std::string& partition_id,
    content::SessionStorageNamespace* session_storage_namespace) {
  if (delegate_) {
    QUrl url = ToQt(target_url);
    return delegate_->CanCreateChild(web_view_, url);
  }
  return false;
}

void QSWebViewImpl::WebContentsCreated(WebContents* source_contents,
                                       int opener_render_frame_id,
                                       const std::string& frame_name,
                                       const GURL& target_url,
                                       WebContents* new_contents) {
  QSWebViewImpl* impl = new QSWebViewImpl(new_contents);
  QSWebView* wv = new QSWebView(impl, web_view_->parent());
  impl->web_view_ = wv;
  impl->web_window_ = web_window_;
  DCHECK(delegate_);
  delegate_->WebViewCreated(web_view_, wv);
}

void QSWebViewImpl::LoadProgressChanged(content::WebContents* source,
                                      double progress) {
  load_progress_ = progress;
  web_view_->loadProgressChanged(progress);
}

JavaScriptDialogManager* QSWebViewImpl::GetJavaScriptDialogManager(
    WebContents* source) {
  if (!js_dialog_manager_) {
    js_dialog_manager_.reset(new QSJavaScriptDialogManager(*this));
  }
  return js_dialog_manager_.get();
}

bool QSWebViewImpl::CanOverscrollContent() const {
  return true;
}

void QSWebViewImpl::OverscrollUpdate(float delta_y) {
  if (overscroll_y_ != delta_y) {
    overscroll_y_ = delta_y;
    web_view_->overscrollYChanged(delta_y);
  }
}

void QSWebViewImpl::OverscrollComplete() {
  overscroll_y_ = 0.f;
  web_view_->overscrollYChanged(0.f);
  web_view_->overscrollYComplete();
}

void QSWebViewImpl::EnterFullscreenModeForTab(WebContents*, const GURL&) {
  fullscreen_ = true;
  web_view_->fullscreenChanged(true);
}

void QSWebViewImpl::ExitFullscreenModeForTab(content::WebContents*) {
  fullscreen_ = false;
  web_view_->fullscreenChanged(false);
}

bool QSWebViewImpl::IsFullscreenForTabOrPending(
    const content::WebContents*) const {
  return fullscreen_;
}

void QSWebViewImpl::CloseContents(WebContents* source) {
  if (delegate_) {
    delegate_->RequestWebViewClose(web_view_);
  }
}

void QSWebViewImpl::NavigationEntryCommitted(
    const LoadCommittedDetails& load_details) {
  const NavigationController& nc =  web_contents_->GetController();
  if (can_go_forward_ != nc.CanGoForward()) {
    can_go_forward_ = !can_go_forward_;
    web_view_->canGoForwardChanged(can_go_forward_);
  }
  if (can_go_back_ != nc.CanGoBack()) {
    can_go_back_ = !can_go_back_;
    web_view_->canGoBackChanged(can_go_back_);
  }
}

void QSWebViewImpl::RenderViewReady() {
  has_render_view_ = true;
  WindowInsetsChanged();
}

void QSWebViewImpl::RenderViewDeleted(content::RenderViewHost*) {
  has_render_view_ = false;
}

void QSWebViewImpl::RenderFrameHostChanged(RenderFrameHost* old_host,
                                           RenderFrameHost* new_host) {
  WindowInsetsChanged();
}

bool QSWebViewImpl::OnMessageReceived(const IPC::Message& message) {
  if (message.type() == ViewHostMsg_SwapCompositorFrame::ID) {
    UpdateScrollOffset();
    return false;
  }

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(QSWebViewImpl, message)
    IPC_MESSAGE_HANDLER(QuickSilverMsg_ViewScrollParamsChanged,
                        OnViewScrollParamsChanged)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}

void QSWebViewImpl::WasShown() {
  QSOomManager::GetInstance()->WebViewActivated(this);
  WindowInsetsChanged();
}

void QSWebViewImpl::OnWindowVisibilityChanged(
    aura::Window* window, bool visible) {
  if (window == web_contents_->GetNativeView() && visible_ != visible) {
    visible_ = visible;
    web_view_->visibleChanged(visible_);
  }
}

void QSWebViewImpl::OnViewScrollParamsChanged(const gfx::SizeF& size,
                                              const float& total_scale_factor) {
  if (web_content_size_ != size) {
    web_content_size_ = size;
    web_view_->contentSizeChanged();
  }
  if (total_scale_factor_ != total_scale_factor) {
    total_scale_factor_ = total_scale_factor;
    UpdateScrollOffset();
  }
}

void QSWebViewImpl::UpdateScrollOffset() {
  RenderWidgetHostViewAura* rwha = static_cast<RenderWidgetHostViewAura*>(
      web_contents_->GetRenderWidgetHostView());
  if (rwha) {
    gfx::Vector2dF offset = rwha->GetLastScrollOffset();
    offset.Scale(total_scale_factor_);

    if (offset != scroll_offset_) {
      scroll_offset_ = offset;
      web_view_->scrollOffsetChanged();
    }
  }
}

} // namespace QuickSilverImpl

