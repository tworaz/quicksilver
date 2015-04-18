// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/web_contents/web_contents_impl_qt.h"

#include "content/public/common/content_client.h"
#include "content/common/view_messages.h"
#include "content/browser/browser_plugin/browser_plugin_guest.h"
#include "content/browser/dom_storage/dom_storage_context_wrapper.h"
#include "content/browser/dom_storage/session_storage_namespace_impl.h"
#include "content/browser/renderer_host/render_view_host_impl.h"
#include "content/browser/web_contents/web_contents_view.h"
#include "content/public/browser/browser_plugin_guest_manager.h"
#include "content/public/browser/content_browser_client.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/user_metrics.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/result_codes.h"

namespace content {

// static
WebContentsImpl* WebContentsImpl::CreateWithOpener(
    const WebContents::CreateParams& params,
    WebContentsImpl* opener) {
  TRACE_EVENT0("browser", "WebContentsImpl::CreateWithOpener");

  WebContentsImpl* new_contents = new WebContentsImplQt(
      params.browser_context, opener, params.platform_data);

  new_contents->Init(params);
  return new_contents;
}

WebContents* WebContents::CreateWithSessionStorage(
    const WebContents::CreateParams& params,
    const SessionStorageNamespaceMap& session_storage_namespace_map) {
  WebContentsImpl* new_contents = new WebContentsImplQt(
      params.browser_context, NULL, params.platform_data);

  for (SessionStorageNamespaceMap::const_iterator it =
           session_storage_namespace_map.begin();
       it != session_storage_namespace_map.end();
       ++it) {
    new_contents->GetController().
        SetSessionStorageNamespace(it->first, it->second.get());
  }

  new_contents->Init(params);
  return new_contents;
}

WebContentsImplQt::WebContentsImplQt(BrowserContext* browser_context,
                                       WebContentsImpl* opener,
                                       void* platform_data)
    : WebContentsImpl(browser_context, opener)
    , platform_data_(platform_data) {
}

void WebContentsImplQt::CreateNewWindow(
    int render_process_id,
    int route_id,
    int main_frame_route_id,
    const ViewHostMsg_CreateWindow_Params& params,
    SessionStorageNamespace* session_storage_namespace) {
  // We usually create the new window in the same BrowsingInstance (group of
  // script-related windows), by passing in the current SiteInstance.  However,
  // if the opener is being suppressed (in a non-guest), we create a new
  // SiteInstance in its own BrowsingInstance.
  bool is_guest = BrowserPluginGuest::IsGuest(this);

  // If the opener is to be suppressed, the new window can be in any process.
  // Since routing ids are process specific, we must not have one passed in
  // as argument here.
  DCHECK(!params.opener_suppressed || route_id == MSG_ROUTING_NONE);

  scoped_refptr<SiteInstance> site_instance =
      params.opener_suppressed && !is_guest ?
      SiteInstance::CreateForURL(GetBrowserContext(), params.target_url) :
      GetSiteInstance();

  // A message to create a new window can only come from the active process for
  // this WebContentsImpl instance. If any other process sends the request,
  // it is invalid and the process must be terminated.
  if (GetRenderProcessHost()->GetID() != render_process_id) {
    RenderProcessHost* rph = RenderProcessHost::FromID(render_process_id);
    base::ProcessHandle process_handle = rph->GetHandle();
    if (process_handle != base::kNullProcessHandle) {
      RecordAction(
          base::UserMetricsAction("Terminate_ProcessMismatch_CreateNewWindow"));
      rph->Shutdown(RESULT_CODE_KILLED, false);
    }
    return;
  }

  // We must assign the SessionStorageNamespace before calling Init().
  //
  // http://crbug.com/142685
  const std::string& partition_id =
      GetContentClient()->browser()->
          GetStoragePartitionIdForSite(GetBrowserContext(),
                                       site_instance->GetSiteURL());
  StoragePartition* partition = BrowserContext::GetStoragePartition(
      GetBrowserContext(), site_instance.get());
  DOMStorageContextWrapper* dom_storage_context =
      static_cast<DOMStorageContextWrapper*>(partition->GetDOMStorageContext());
  SessionStorageNamespaceImpl* session_storage_namespace_impl =
      static_cast<SessionStorageNamespaceImpl*>(session_storage_namespace);
  CHECK(session_storage_namespace_impl->IsFromContext(dom_storage_context));

  content::WebContents* thiz = this;
  if (delegate_ &&
      !delegate_->ShouldCreateWebContents(thiz,
                                          route_id,
                                          main_frame_route_id,
                                          params.window_container_type,
                                          params.frame_name,
                                          params.target_url,
                                          partition_id,
                                          session_storage_namespace)) {
    if (route_id != MSG_ROUTING_NONE &&
        !RenderViewHost::FromID(render_process_id, route_id)) {
      // If the embedder didn't create a WebContents for this route, we need to
      // delete the RenderView that had already been created.
      Send(new ViewMsg_Close(route_id));
    }
    GetRenderViewHost()->GetProcess()->ResumeRequestsForView(route_id);
    GetRenderViewHost()->GetProcess()->ResumeRequestsForView(
        main_frame_route_id);
    return;
  }

  scoped_refptr<SessionStorageNamespace> ssn = session_storage_namespace;
  WebContentsDelegate::WebContentsCreateCallback callback = base::Bind(
      &WebContentsImplQt::HandleNewWebContentsCreate,
      base::Unretained(this), render_process_id, route_id,
      main_frame_route_id, params, ssn);

  if (delegate_ && delegate_->WebContentsCreateAsync(callback))
    return;

  callback.Run(NULL);
}

WebContents* WebContentsImplQt::HandleNewWebContentsCreate(
    int render_process_id,
    int route_id,
    int main_frame_route_id,
    const ViewHostMsg_CreateWindow_Params& params,
    SessionStorageNamespace* session_storage_namespace,
    void* platform_data) {

  bool is_guest = BrowserPluginGuest::IsGuest(this);
  scoped_refptr<SiteInstance> site_instance =
      params.opener_suppressed && !is_guest ?
      SiteInstance::CreateForURL(GetBrowserContext(), params.target_url) :
      GetSiteInstance();
  const std::string& partition_id =
      GetContentClient()->browser()->
          GetStoragePartitionIdForSite(GetBrowserContext(),
                                       site_instance->GetSiteURL());

  // Create the new web contents. This will automatically create the new
  // WebContentsView. In the future, we may want to create the view separately.
  CreateParams create_params(GetBrowserContext(), site_instance.get());
  create_params.routing_id = route_id;
  create_params.main_frame_routing_id = main_frame_route_id;
  create_params.opener = this;
  create_params.opener_suppressed = params.opener_suppressed;
  create_params.platform_data = platform_data;
  if (params.disposition == NEW_BACKGROUND_TAB)
    create_params.initially_hidden = true;
  create_params.renderer_initiated_creation = true;

  WebContentsImplQt* new_contents = NULL;
  if (!is_guest) {
    create_params.context = view_->GetNativeView();
    create_params.initial_size = GetContainerBounds().size();
    new_contents = static_cast<WebContentsImplQt*>(
          WebContents::Create(create_params));
  }  else {
    new_contents = static_cast<WebContentsImplQt*>(
        GetBrowserPluginGuest()->CreateNewGuestWindow(create_params));
  }
  new_contents->GetController().SetSessionStorageNamespace(
        partition_id,
        session_storage_namespace);

  new_contents->SetPlatformData(platform_data);

  // Save the window for later if we're not suppressing the opener (since it
  // will be shown immediately).
  if (!params.opener_suppressed) {
    if (!is_guest) {
      WebContentsView* new_view = new_contents->view_.get();

      // TODO(brettw): It seems bogus that we have to call this function on the
      // newly created object and give it one of its own member variables.
      new_view->CreateViewForWidget(new_contents->GetRenderViewHost(), false);
    }
    // Save the created window associated with the route so we can show it
    // later.
    DCHECK_NE(MSG_ROUTING_NONE, route_id);
    pending_contents_[route_id] = new_contents;
    AddDestructionObserver(new_contents);
  }

  if (delegate_) {
    delegate_->WebContentsCreated(
        this, params.opener_render_frame_id, params.frame_name,
        params.target_url, new_contents);
  }

  if (params.opener_suppressed) {
    // When the opener is suppressed, the original renderer cannot access the
    // new window.  As a result, we need to show and navigate the window here.
    bool was_blocked = false;
    if (delegate_) {
      gfx::Rect initial_rect;
      delegate_->AddNewContents(
          this, new_contents, params.disposition, initial_rect,
          params.user_gesture, &was_blocked);
    }
    if (!was_blocked) {
      OpenURLParams open_params(params.target_url,
                                Referrer(),
                                CURRENT_TAB,
                                ui::PAGE_TRANSITION_LINK,
                                true /* is_renderer_initiated */);
      open_params.user_gesture = params.user_gesture;
      new_contents->OpenURL(open_params);
    }
  }

  return new_contents;
}

}
