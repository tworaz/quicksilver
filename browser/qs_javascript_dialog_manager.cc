// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_javascript_dialog_manager.h"

#include "ozone_qt/util/type_conversion.h"
#include "quicksilver/browser/qs_web_view_impl.h"
#include "quicksilver/public/browser/qs_javascript_dialog.h"

using namespace content;
using namespace QuickSilver;

namespace QuickSilverImpl {

namespace {

class QSJavaScriptDialogImpl : public QSJavaScriptDialog {
 public:
  QSJavaScriptDialogImpl(Type type,
                         QUrl origin_url,
                         QString message_text,
                         QString prompt_text,
                         const JavaScriptDialogManager::DialogClosedCallback& cb,
                         QSJavaScriptDialogManager& manager)
      : QSJavaScriptDialog(type, origin_url, message_text, prompt_text)
      , handled_(false)
      , callback_(cb)
      , manager_(manager) {
  }

  ~QSJavaScriptDialogImpl() override {
    DCHECK(handled_);
  }

  void accept(QString user_input = QString()) override {
    decide(true, user_input);
  }

  void reject() override {
    decide(false, QString());
  }

 private:
  void decide(bool value, QString user_input) {
    DCHECK(!handled_);
    if (!handled_) {
      base::string16 input = toString16(user_input);
      callback_.Run(value, input);
      manager_.CloseDialog(this);
      handled_ = true;
    }
  }

  bool handled_;
  JavaScriptDialogManager::DialogClosedCallback callback_;
  QSJavaScriptDialogManager& manager_;
};

} // namespace

QSJavaScriptDialogManager::QSJavaScriptDialogManager(QSWebViewImpl& wv)
    : web_view_(wv)
    , active_dialog_(nullptr) {
}

QSJavaScriptDialogManager::~QSJavaScriptDialogManager() {
  if (active_dialog_) {
    active_dialog_->reject();
    active_dialog_ = nullptr;
  }
}

void QSJavaScriptDialogManager::RunJavaScriptDialog(
    WebContents* web_contents,
    const GURL& origin_url,
    const std::string& accept_lang,
    JavaScriptMessageType javascript_message_type,
    const base::string16& message_text,
    const base::string16& default_prompt_text,
    const DialogClosedCallback& callback,
    bool* did_suppress_message) {

  if (active_dialog_) {
    // One dialog at a time.
    *did_suppress_message = true;
    return;
  }

  QSJavaScriptDialog::Type type;
  switch (javascript_message_type) {
    case JAVASCRIPT_MESSAGE_TYPE_ALERT:
      type = QSJavaScriptDialog::Alert;
      break;
    case JAVASCRIPT_MESSAGE_TYPE_CONFIRM:
      type = QSJavaScriptDialog::Confirm;
      break;
    case JAVASCRIPT_MESSAGE_TYPE_PROMPT:
      type = QSJavaScriptDialog::Prompt;
      break;
  }

  QSJavaScriptDialog* dialog = new QSJavaScriptDialogImpl(type,
      ToQt(origin_url), ToQt(message_text), ToQt(default_prompt_text),
      callback, *this);

  active_dialog_ = dialog;

  if (!web_view_.ShowJavaScriptDialog(dialog)) {
    delete dialog;
    active_dialog_ = nullptr;
    *did_suppress_message = true;
    return;
  }

  *did_suppress_message = false;
}

void QSJavaScriptDialogManager::RunBeforeUnloadDialog(
    WebContents* web_contents,
    const base::string16& message_text,
    bool is_reload,
    const DialogClosedCallback& callback) {
  NOTIMPLEMENTED();
}

bool QSJavaScriptDialogManager::HandleJavaScriptDialog(
    WebContents* web_contents,
    bool accept,
    const base::string16* prompt_override) {
  DCHECK(active_dialog_);
  if (accept) {
    QString reply = prompt_override ? ToQt(*prompt_override) : QString();
    active_dialog_->accept(reply);
  } else {
    active_dialog_->reject();
  }
  return true;
}

void QSJavaScriptDialogManager::CancelActiveAndPendingDialogs(WebContents*) {
  web_view_.CancelAllJavaScriptDialogs();
}

void QSJavaScriptDialogManager::ResetDialogState(WebContents*) {
  if (active_dialog_) {
    active_dialog_->reject();
  }
}

void QSJavaScriptDialogManager::CloseDialog(QSJavaScriptDialog* dialog) {
  DCHECK(dialog == active_dialog_);
  active_dialog_->deleteLater();
  active_dialog_ = nullptr;
}

} // namespace QuickSilverImpl

