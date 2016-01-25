// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_pref_service_factory.h"

#include "base/bind.h"
#include "base/files/file_path.h"
#include "base/prefs/json_pref_store.h"
#include "base/prefs/overlay_user_pref_store.h"
#include "base/prefs/pref_filter.h"
#include "base/prefs/pref_service.h"
#include "base/prefs/pref_service_factory.h"

namespace QuickSilverImpl {

namespace {

void HandleReadError(PersistentPrefStore::PrefReadError error) {
  if (error == PersistentPrefStore::PREF_READ_ERROR_NO_FILE) {
    return;
  }
  LOG(ERROR) << "An error happened during prefs loading: " << error;
}

} // namespace

scoped_ptr<PrefService> CreateContextPrefs(
    const base::FilePath& prefs_file,
    base::SequencedTaskRunner* task_runner,
    const scoped_refptr<PrefRegistry>& pref_registry,
    bool async) {

  DVLOG(1) << "Create prefs from file: " << prefs_file.value();

  base::PrefServiceFactory factory;
  factory.set_async(async);
  factory.set_read_error_callback(base::Bind(&HandleReadError));
  factory.SetUserPrefsFile(prefs_file, task_runner);

  return factory.Create(pref_registry.get()).Pass();
}

scoped_ptr<PrefService> CreateOffTheRecordContextPrefs(
    const base::FilePath& prefs_file,
    base::SequencedTaskRunner* task_runner,
    const scoped_refptr<PrefRegistry>& pref_registry,
    bool async) {

  DVLOG(1) << "Create off the record prefs from file: " << prefs_file.value();

  scoped_refptr<PersistentPrefStore> user_prefs =
      new JsonPrefStore(prefs_file, task_runner, scoped_ptr<PrefFilter>());
  scoped_refptr<OverlayUserPrefStore> incognito_pref_store =
      new OverlayUserPrefStore(user_prefs.get());

  base::PrefServiceFactory factory;
  factory.set_async(async);
  factory.set_read_error_callback(base::Bind(&HandleReadError));
  factory.set_user_prefs(incognito_pref_store);

  return factory.Create(pref_registry.get()).Pass();
}

} // namespace QuickSilverImpl
