# libpng.gyp - for libpng 1.6.12
{
  'targets': [
    {
      'target_name': 'libpng_static',
      'product_name': 'libpng',
      'type': 'static_library',
      'dependencies': [
        '../zlib/zlib.gyp:zlib_static',
      ],
      'include_dirs': [
        '.',
        '../zlib',
      ],
      'sources': [
        'png.c',
        'png.h',
        'pngconf.h',
        'pngdebug.h',
        'pngerror.c',
        'pngget.c',
        'pnginfo.h',
        'pnglibconf.h',
        'pngmem.c',
        'pngpread.c',
        'pngpriv.h',
        'pngread.c',
        'pngrio.c',
        'pngrtran.c',
        'pngrutil.c',
        'pngset.c',
        'pngstruct.h',
        'pngtrans.c',
        'pngwio.c',
        'pngwrite.c',
        'pngwtran.c',
        'pngwutil.c',
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
          'msbuild_settings': {
            'ClCompile': {
              'RuntimeLibrary': 'MultiThreaded', # /MT
            },
            'Link': {
              'EnableCOMDATFolding': 'true',
              'OptimizeReferences': 'true',
            },
          },
        },
      },
      'msbuild_configuration_attributes': {
        'CharacterSet': 'MultiByte',
      },
      'msbuild_settings': {
        'ClCompile': {
          'SuppressStartupBanner': 'true',    # /nologo
          'WarningLevel': 'Level4',           # /W4
          'Optimization': 'Full',             # /Ox
          'IntrinsicFunctions': 'true',       # /Oi
          #'WholeProgramOptimization': 'true', # /GL
          'PreprocessorDefinitions': [
            'WIN32',
            '_WINDOWS',
          ],
          'MinimalRebuild': 'false',          # /Gm-
          'FunctionLevelLinking': 'true',     # /Gy
          'FloatingPointExceptions': 'false', # /fp:except-
          'CompileAs': 'CompileAsC',          # /TC
          'StringPooling': 'true',            # /GF
          'DisableSpecificWarnings': [
            '4996', '4127'
          ], # /wd"4996"
          'TreatWChar_tAsBuiltInType': 'false', # /Zc:wchar_t-
        },
        'Lib': {
          #'LinkTimeCodeGeneration': 'true', # /LTCG
        },
      },
      'xcode_settings': {
        'SKIP_INSTALL': 'YES',
      },
    },
  ],
}
