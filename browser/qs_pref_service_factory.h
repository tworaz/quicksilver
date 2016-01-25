// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_PREFS_QS_PREF_SERVICE_FACTORY_H_
#define QUICKSILVER_BROWSER_PREFS_QS_PREF_SERVICE_FACTORY_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

class PrefService;
class PrefRegistry;

namespace base {
class FilePath;
class SequencedTaskRunner;
}

namespace QuickSilverImpl {

scoped_ptr<PrefService> CreateContextPrefs(
    const base::FilePath& prefs_file,
    base::SequencedTaskRunner* task_runner,
    const scoped_refptr<PrefRegistry>& pref_registry,
    bool async);

scoped_ptr<PrefService> CreateOffTheRecordContextPrefs(
    const base::FilePath& prefs_file,
    base::SequencedTaskRunner* task_runner,
    const scoped_refptr<PrefRegistry>& pref_registry,
    bool async);

} // namespace QuickSilverImpl

#endif // QUICKSILVER_BROWSER_PREFS_QS_PREF_SERVICE_FACTORY_H_

