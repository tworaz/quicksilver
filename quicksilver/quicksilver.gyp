# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'version_file': '<(DEPTH)/chrome/VERSION',
    'version_script': '<(DEPTH)/build/util/version.py',
    'version_pattern': '@MAJOR@.@MINOR@.@BUILD@.@PATCH@',
    'quicksilver_data_dir%': '',
  },

  'targets': [
    {
      'target_name': 'quicksilver',
      'type': 'shared_library',
      'dependencies': [
        '<(DEPTH)/content/content.gyp:content',
        '<(DEPTH)/content/content.gyp:content_app_both',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/qt_port/build/system.gyp:qtgui',
        '<(DEPTH)/qt_port/build/system.gyp:qtquick',
        '<(DEPTH)/third_party/WebKit/public/blink_headers.gyp:blink_headers',
        'quicksilver_pak',
      ],
      'include_dirs': [
        '<(DEPTH)/v8/include',
      ],
      'cflags!': [
        # Symbol visibility controlled by version script
        '-fvisibility=hidden',
      ],
      'defines': [
        'CONTENT_IMPLEMENTATION=1',
        'CHROMIUM_VERSION=\"<!(python <(version_script) -f <(version_file) -t "<(version_pattern)")\"',
      ],
      'link_settings': {
        'ldflags': [
          '-Wl,--no-undefined',
          '-Wl,--version-script=<(DEPTH)/qt_port/quicksilver/quicksilver.filter',
        ],
      },
      'sources': [
        'browser/public/Init.cc',
        'browser/public/Init.h',
        'browser/public/WebView.cc',
        'browser/public/WebView.h',
        '<(moc_gen_dir)/moc_WebView.cc',

        'browser/browser_context_qs.cc',
        'browser/browser_context_qs.h',
        'browser/browser_main_parts_qs.cc',
        'browser/browser_main_parts_qs.h',
        'browser/content_browser_client_qs.cc',
        'browser/content_browser_client_qs.h',
        'browser/global_data.cc',
        'browser/global_data.h',
        'browser/message_pump_qt.cc',
        'browser/message_pump_qt.h',
        'browser/network_delegate_qs.cc',
        'browser/network_delegate_qs.h',
        'browser/permission_manager_qs.cc',
        'browser/permission_manager_qs.h',
        'browser/url_request_context_getter_qs.cc',
        'browser/url_request_context_getter_qs.h',
        'browser/web_view_impl.cc',
        'browser/web_view_impl.h',
        # Reused upstream sources
        '<(DEPTH)/content/shell/browser/shell_net_log.cc',
        '<(DEPTH)/content/shell/browser/shell_net_log.h',

        'common/content_main_delegate_qs.cc',
        'common/content_main_delegate_qs.h',
        'common/content_client_qs.cc',
        'common/content_client_qs.h',

        'renderer/public/Renderer.cc',
        'renderer/public/Renderer.h',
        'renderer/content_renderer_client_qs.cc',
        'renderer/content_renderer_client_qs.h',
      ],
      'conditions': [
         ['quicksilver_data_dir!=""', {
           'defines': [
             'QUICKSILVER_DATA_DIR="<(quicksilver_data_dir)"',
           ],
         }],
       ],
    }, # quicksilver
    {
      'target_name': 'quicksilver_moc',
      'type': 'none',
      'sources': [
        'browser/public/WebView.h',
      ],
      'rules': [{
          'rule_name': 'generate_quicksilver_moc',
          'includes': [ '../build/moc_rule.gypi' ],
      }],
    }, # quicksilver_moc
    {
      'target_name': 'quicksilver_renderer',
      'type': 'executable',
      'dependencies': [
        'quicksilver',
      ],
      'sources': [
        'renderer/quicksilver_renderer.cc',
      ],
    }, # quicksilver_renderer
    {
      'target_name': 'quicksilver_pak',
      'type': 'none',
      'dependencies': [
        '<(DEPTH)/third_party/WebKit/public/blink_resources.gyp:blink_resources',
        '<(DEPTH)/third_party/WebKit/public/blink_resources.gyp:blink_image_resources',
        '<(DEPTH)/content/content.gyp:content_resources',
        '<(DEPTH)/net/net.gyp:net_resources',
        '<(DEPTH)/ui/resources/ui_resources.gyp:ui_resources',
        '<(DEPTH)/ui/strings/ui_strings.gyp:ui_strings',
      ],
      'actions': [
        {
          'action_name': 'repack_quicksilver_pack',
          'variables': {
            'pak_inputs': [
              '<(SHARED_INTERMEDIATE_DIR)/blink/public/resources/blink_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/blink/public/resources/blink_image_resources_100_percent.pak',
              '<(SHARED_INTERMEDIATE_DIR)/content/app/resources/content_resources_100_percent.pak',
              '<(SHARED_INTERMEDIATE_DIR)/content/content_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/net/net_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/ui/resources/ui_resources_100_percent.pak',

              # TODO: Store strings in the localization paks
              '<(SHARED_INTERMEDIATE_DIR)/content/app/strings/content_strings_en-US.pak',
            ],
            'pak_output': '<(PRODUCT_DIR)/quicksilver.pak',
          },
          'includes': [ '../../build/repack_action.gypi' ],
        },
      ],
    }, # quicksilver_pak
  ],

  'includes': [
    'shell/quicksilver_shell.gypi',
  ],
}
