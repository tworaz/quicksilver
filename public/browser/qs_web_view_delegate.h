// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_PUBLIC_BROWSER_QS_WEB_VIEW_DELEGATE_H_
#define _QUICKSILVER_PUBLIC_BROWSER_QS_WEB_VIEW_DELEGATE_H_

namespace QuickSilver {

class QSContextMenuParams;
class QSJavaScriptDialog;
class QSPermissionRequest;
class QSWebView;

class QSWebViewDelegate {
 public:
  // Ask the embedder if the view can create child for a given url.
  virtual bool CanCreateChild(const QSWebView*, QUrl& url) = 0;

  // Inform the embedder that a new child view has been created.
  // The embedder is responsible for actually adding the view to
  // some window and showing it.
  virtual void WebViewCreated(const QSWebView*, QSWebView* new_view) = 0;
  virtual void RequestWebViewClose(QSWebView*) = 0;

  virtual void ShowContextMenu(const QSWebView*, const QSContextMenuParams&) = 0;

  virtual bool ShowJavaScriptDialog(const QSWebView*, QSJavaScriptDialog*) = 0;
  virtual void CancelAllJavaScriptDialogs(const QSWebView*) = 0;

  virtual bool RequestPermission(const QSWebView*, QSPermissionRequest*) = 0;
  virtual void CancelPermissionRequest(const QSWebView*, int request_id) = 0;
};

} // namespace QuickSilver

#endif // !_QUICKSILVER_PUBLIC_BROWSER_QS_WEB_VIEW_DELEGATE_H_
