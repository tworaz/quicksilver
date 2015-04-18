// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/web_view_impl.h"

#include "base/strings/utf_string_conversions.h"
#include "content/browser/web_contents/web_contents_impl_qt.h"
#include "content/browser/web_contents/web_contents_view_qt.h"
#include "quicksilver/browser/browser_context_qs.h"
#include "quicksilver/browser/content_browser_client_qs.h"
#include "quicksilver/browser/public/NewWindowPermissionRequest.h"
#include "quicksilver/browser/public/WebView.h"
#include "ui/qt/type_conversion.h"

using namespace content;
using namespace QuickSilver;

namespace QuickSilverImpl {

namespace {

static WebContents* NullCreateWebContents(void*) {
  return NULL;
}

WebContentsDelegate::WebContentsCreateCallback g_new_web_contents_create_cb =
    base::Bind(&NullCreateWebContents);

} // namespace

// static
QQuickItem* WebViewImpl::CreateNativeView(const WebContents* wc) {
  const WebContentsImplQt* wci = static_cast<const WebContentsImplQt*>(wc);
  const WebViewImpl* thiz = static_cast<WebViewImpl*>(wci->GetPlatformData());
  DCHECK(thiz);
  return thiz->web_view_;
}

WebViewImpl::WebViewImpl(QuickSilver::WebView* web_view)
    : WebContentsDelegate(),
      WebContentsObserver(),
      web_view_(web_view),
      can_go_forward_(false),
      can_go_back_(false),
      is_loading_(false),
      load_progress_(0.f) {
  static bool nv_factory_set = false;
  if (!nv_factory_set) {
    WebContentsViewQt::SetNativeViewFactoryDelegate(
        &WebViewImpl::CreateNativeView);
    nv_factory_set = true;
  }

  WebContents* web_contents = g_new_web_contents_create_cb.Run(this);
  if (!web_contents) {
    WebContents::CreateParams params(
        ContentBrowserClientQS::Get()->browser_context());
    params.platform_data = this;
    web_contents = WebContents::Create(params);
  }
  web_contents_.reset(web_contents);
  web_contents_->SetDelegate(this);
  this->Observe(web_contents_.get());
}

WebViewImpl::~WebViewImpl() {
  web_view_ = NULL;
}

void WebViewImpl::LoadURL(GURL url) {
  DCHECK(url.is_valid());
  NavigationController::LoadURLParams params(url);
  params.transition_type = ui::PageTransitionFromInt(
      ui::PAGE_TRANSITION_TYPED | ui::PAGE_TRANSITION_FROM_ADDRESS_BAR);
  web_contents_->GetController().LoadURLWithParams(params);
  web_view_->currentURLChanged();
}

void WebViewImpl::Reload(bool force) {
  web_contents_->GetController().Reload(force);
  web_contents_->Focus();
}

void WebViewImpl::Stop() {
  web_contents_->Stop();
  web_contents_->Focus();
}

void WebViewImpl::GoBackOrForward(int offset) {
  web_contents_->GetController().GoToOffset(offset);
  web_contents_->Focus();
}

const GURL& WebViewImpl::GetVisibleURL() const {
  return web_contents_->GetVisibleURL();
}

const base::string16& WebViewImpl::GetTitle() const {
  return web_contents_->GetTitle();
}

bool WebViewImpl::CanGoForward() const {
  return can_go_forward_;
}

bool WebViewImpl::CanGoBack() const {
  return can_go_back_;
}

bool WebViewImpl::IsLoading() const {
  return is_loading_;
}

double WebViewImpl::LoadProgress() const {
  return load_progress_;
}

void WebViewImpl::NavigationStateChanged(
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

void WebViewImpl::LoadProgressChanged(content::WebContents* source,
                                      double progress) {
  load_progress_ = progress;
  web_view_->loadProgressChanged(progress);
}

bool WebViewImpl::ShouldCreateWebContents(
    content::WebContents* web_contents,
    int route_id,
    int main_frame_route_id,
    WindowContainerType window_container_type,
    const base::string16& frame_name,
    const GURL& target_url,
    const std::string& partition_id,
    content::SessionStorageNamespace* session_storage_namespace) {
  NewWindowPermissionRequest request(toQt(target_url.spec()));
  web_view_->newWindowPermissionRequest(&request);
  return request.isAccepted();
}

bool WebViewImpl::WebContentsCreateAsync(
    WebContentsDelegate::WebContentsCreateCallback cb) {
  g_new_web_contents_create_cb = cb;
  web_view_->createNewWindow();
  g_new_web_contents_create_cb = base::Bind(&NullCreateWebContents);
  return true;
}

void WebViewImpl::NavigationEntryCommitted(
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

} // namespace QuickSilverImpl
