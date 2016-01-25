# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'version_file': '<(DEPTH)/chrome/VERSION',
    'version_script': '<(DEPTH)/build/util/version.py',
    'version_pattern': '@MAJOR@.@MINOR@.@BUILD@.@PATCH@',
    'version_cmd': 'python <(version_script) -f <(version_file)',
    'quicksilver_target': 'quicksilver',
    'quicksilver_soname': '<!(<(version_cmd) -t "@MAJOR@")',
  },

  'target_defaults': {
    'defines': [
      'COMPONENT_BUILD=1',
      'SKIA_DLL=1',
      'BUILDING_V8_SHARED=1',
      'V8_SHARED=1',
      'QUICKSILVER=1',
    ],
  },
}
