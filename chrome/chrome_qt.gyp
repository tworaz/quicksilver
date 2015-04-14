# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'target_defaults': {
    'include_dirs': [
      '<(DEPTH)',
      '<(DEPTH)/skia/config',
    ],
  },

  'targets': [
    {
      'target_name': 'chrome_error_pages',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/chrome/chrome_resources.gyp:chrome_resources',
        '<(DEPTH)/components/components.gyp:error_page_renderer',
        '<(DEPTH)/third_party/WebKit/public/blink_headers.gyp:blink_headers',
        'chrome_common_reuse',
      ],
      'sources': [
        '<(DEPTH)/chrome/renderer/net/net_error_helper.cc',
        '<(DEPTH)/chrome/renderer/net/net_error_helper.h',
        '<(DEPTH)/chrome/renderer/net/net_error_page_controller.cc',
        '<(DEPTH)/chrome/renderer/net/net_error_page_controller.h',
      ],
    }, # chrome_error_pages
    {
      'target_name': 'chrome_common_reuse',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/chrome/chrome_resources.gyp:chrome_strings',
      ],
      'sources': [
        '<(DEPTH)/chrome/common/chrome_switches.cc',
        '<(DEPTH)/chrome/common/chrome_switches.h',
        '<(DEPTH)/chrome/common/localized_error.cc',
        '<(DEPTH)/chrome/common/localized_error.h',
        'common/common_message_generator_qt.cc',
        'common/common_message_generator_qt.h',
        'common/render_messages_qt.h',
      ],
    }, # chrome_common_reuse
  ],
}
