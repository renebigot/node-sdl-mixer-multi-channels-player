{
  'targets': [{
    'target_name': 'sdl-mixer-player',
    'cflags!': ['-fno-exceptions'],
    'cflags_cc!': ['-fno-exceptions'],
    'xcode_settings': {
      'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
      'CLANG_CXX_LIBRARY': 'libc++',
      'MACOSX_DEPLOYMENT_TARGET': '10.7'
    },
    'msvs_settings': {'VCCLCompilerTool': {'ExceptionHandling': 1}},
    'sources': [
      'src/SDLMixerPlayer.cc',
    ],
    'cflags': ['-Wall', '-std=c++11'],
    'include_dirs': [
      '<!(node -e "require(\'nan\')")',
      '<(module_root_dir)/dependencies/include/',
    ],
    'conditions': [[
      'OS==\'mac\'', {
        'libraries': [
          '-framework', 'Cocoa',
          '<(module_root_dir)/dependencies/macOS/libSDL2.a',
          '<(module_root_dir)/dependencies/macOS/libSDL2_mixer.a',
        ]
      },
      'OS==\'win\'', {
        'libraries': [
          '<(module_root_dir)/dependencies/win/<(target_arch)/SDL2.lib',
          '<(module_root_dir)/dependencies/win/<(target_arch)/SDL2_mixer.lib'
        ]
      }
    ]]
  }]
}
