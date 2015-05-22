# zlib.gyp - for zlib 1.2.8
{
  'targets': [
    {
      'target_name': 'zlib_static',
      'product_name': 'zlib',
      'type': 'static_library',
      'include_dirs': [
        '.',
      ],
      'sources': [
        'adler32.c',
        'compress.c',
        'crc32.c',
        'crc32.h',
        'deflate.c',
        'deflate.h',
        'gzclose.c',
        'gzguts.h',
        'gzlib.c',
        'gzread.c',
        'gzwrite.c',
        'infback.c',
        'inffast.c',
        'inffast.h',
        'inffixed.h',
        'inflate.c',
        'inflate.h',
        'inftrees.c',
        'inftrees.h',
        'trees.c',
        'trees.h',
        'uncompr.c',
        'zconf.h',
        'zlib.h',
        'zutil.c',
        'zutil.h',
      ],
      'default_configuration': 'Release',
      'configurations': {
        'Debug': {
          'defines': ['_DEBUG'],
          'msbuild_settings':{
            'ClCompile': {
              'RuntimeLibrary': 'MultiThreadedDebug', # /MTd
            },
          },
        },
        'Release': {
          'defines': ['NDEBUG'],
          'msbuild_settings':{
            'ClCompile': {
              'PreprocessorDefinitions': [
                'ASMV',
                'ASMINF',
              ],
              'RuntimeLibrary': 'MultiThreaded', # /MT
            },
          },
        },
      },
      'msbuild_configuration_attributes': {
        'CharacterSet': 'Unicode',
      },
      'msbuild_settings':{
        'ClCompile': {
          'PreprocessorDefinitions': [
            'WIN32',
            '_WINDOWS',
            '_CRT_NONSTDC_NO_DEPRECATE',
            '_CRT_SECURE_NO_DEPRECATE',
            '_CRT_NONSTDC_NO_WARNINGS',
          ],
          'SuppressStartupBanner': 'true',                 # /nologo
          'WarningLevel': 'Level3',                        # /W3
          'Optimization': 'MaxSpeed',                      # /O2
          'InlineFunctionExpansion': 'OnlyExplicitInline', # /Ob1
          'StringPooling': 'true',                         # /GF
          'FunctionLevelLinking': 'true',                  # /Gy
          'BufferSecurityCheck': 'false',                  # /GS-
        },
      },
      'xcode_settings': {
        'SKIP_INSTALL': 'YES',
        'COMBINE_HIDPI_IMAGES': 'YES',
        'GCC_C_LANGUAGE_STANDARD': 'c89',
      },
    },
  ],
}
