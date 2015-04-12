# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'target_defaults': {
      'include_dirs': [
        '<(DEPTH)',
      ],
  },

  'targets': [
    {
      'target_name': 'shell_lib_qt',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/third_party/mojo/mojo_public.gyp:mojo_cpp_bindings',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/qt_port/build/system.gyp:qtcore',
        '<(DEPTH)/qt_port/build/system.gyp:qtgui',
        '<(DEPTH)/qt_port/build/system.gyp:qtqml',
        '<(DEPTH)/qt_port/build/system.gyp:qtquick',
        'shell_lib_qt_moc',
      ],
      'sources': [
        'shell/browser/shell_qt.cc',
        'shell/browser/shell_native_view_qt.h',
        'shell/browser/shell_native_view_qt.cc',
        'shell/browser/shell_window_qt.h',
        'shell/browser/shell_window_qt.cc',
        '<(moc_gen_dir)/moc_shell_window_qt.cc',
        'shell/browser/shell_web_contents_view_delegate_qt.cc',
        '<(rcc_gen_dir)/rcc_shell.cc',
      ],
      'actions': [{
        'action_name': 'generate_shell_rcc',
        'variables': {
          'rcc_file': 'shell/browser/shell.qrc',
          'resources': [
            'shell/browser/shell.qml',
          ],
          'rcc_output': 'rcc_shell.cc',
          'rcc_name': 'shell',
        },
        'includes': [ '../build/rcc_action.gypi' ],
      }],
    }, # shell_lib_qt
    {
      'target_name': 'content_browser_qt',
      'type': 'static_library',
      'link_settings': {
        'libraries': [
          # XXX: Workaround for
          # qopenglfunctions.h:570: error: undefined reference to 'glBindTexture'
          # qopenglfunctions.h:1070: error: undefined reference to 'glActiveTexture'
          '-lGLESv2',
        ],
      },
      'include_dirs!': [
        '<(DEPTH)/third_party/khronos',
      ],
      'dependencies': [
        '<(DEPTH)/qt_port/build/system.gyp:qtcore',
        '<(DEPTH)/qt_port/build/system.gyp:qtgui',
        '<(DEPTH)/qt_port/build/system.gyp:qtquick',
        '<(DEPTH)/qt_port/build/system.gyp:qtopengl',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/third_party/mojo/mojo_public.gyp:mojo_cpp_bindings',
        'qt_gpu_integration',
      ],
      'sources': [
        'browser/renderer_host/native_web_keyboard_event_qt.cpp',
        'browser/renderer_host/render_widget_host_view_qt.cc',
        'browser/renderer_host/render_widget_host_view_qt.h',
        'browser/renderer_host/delegated_frame_node.h',
        'browser/renderer_host/delegated_frame_node.cpp',
        'browser/renderer_host/yuv_video_node.h',
        'browser/renderer_host/yuv_video_node.cpp',
        'browser/renderer_host/stream_video_node.h',
        'browser/renderer_host/stream_video_node.cpp',
        'browser/renderer_host/web_event_factory.h',
        'browser/renderer_host/web_event_factory.cpp',
        'browser/web_contents/web_contents_view_qt.cc',
        'browser/web_contents/web_contents_view_qt.h',
      ],
    }, # content_browser_qt
    {
      'target_name': 'shell_lib_qt_moc',
      'type': 'none',
      'dependencies': [
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/qt_port/build/system.gyp:qtgui',
        '<(DEPTH)/qt_port/build/system.gyp:qtcore',
        '<(DEPTH)/qt_port/build/system.gyp:qtquick',
      ],
      'sources': [
        'shell/browser/shell_window_qt.h',
      ],
      'rules': [{
          'rule_name': 'generate_shell_moc',
          'includes': [ '../build/moc_rule.gypi' ],
      }],
    }, # ui_base_qt_moc
    {
      'target_name': 'qt_gpu_integration',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/qt_port/build/system.gyp:qtcore',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/third_party/mesa/mesa.gyp:mesa_headers',
        '<(DEPTH)/third_party/mojo/mojo_public.gyp:mojo_cpp_bindings',
        '<(DEPTH)/third_party/khronos/khronos.gyp:khronos_headers',
      ],
      'sources': [
        'browser/renderer_host/chromium_gpu_helper.h',
        'browser/renderer_host/chromium_gpu_helper.cpp',
      ],
    }
  ],
}

