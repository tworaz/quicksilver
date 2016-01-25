// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_QS_JAVASCRIPT_DIALOG_MANAGER_H_
#define QUICKSILVER_BROWSER_QS_JAVASCRIPT_DIALOG_MANAGER_H_

#include "content/public/browser/javascript_dialog_manager.h"

namespace QuickSilver {
class QSJavaScriptDialog;
}

namespace QuickSilverImpl {

class QSWebViewImpl;

class QSJavaScriptDialogManager : public content::JavaScriptDialogManager {
 public:
  QSJavaScriptDialogManager(QSWebViewImpl&);
  ~QSJavaScriptDialogManager() override;

  // content::JavaScriptDialogManager:
  void RunJavaScriptDialog(
      content::WebContents* web_contents,
      const GURL& origin_url,
      const std::string& accept_lang,
      content::JavaScriptMessageType javascript_message_type,
      const base::string16& message_text,
      const base::string16& default_prompt_text,
      const DialogClosedCallback& callback,
      bool* did_suppress_message) override;
  void RunBeforeUnloadDialog(content::WebContents* web_contents,
                             const base::string16& message_text,
                             bool is_reload,
                             const DialogClosedCallback& callback) override;
  bool HandleJavaScriptDialog(content::WebContents* web_contents,
                              bool accept,
                              const base::string16* prompt_override) override;
  void CancelActiveAndPendingDialogs(content::WebContents*) override;
  void ResetDialogState(content::WebContents*) override;

  void CloseDialog(QuickSilver::QSJavaScriptDialog*);

 private:
  QSWebViewImpl& web_view_;
  QuickSilver::QSJavaScriptDialog* active_dialog_;

  DISALLOW_COPY_AND_ASSIGN(QSJavaScriptDialogManager);
};

} // namespace QuickSilverImpl

#endif // QUICKSILVER_BROWSER_QS_JAVASCRIPT_DIALOG_MANAGER_H_

