### ----------------------------------------------------------------------------
### Meson Configuration
### ----------------------------------------------------------------------------

project('pong', 'c', 'cpp',
  version : '1.0',
  default_options : [
    'c_std=c99',
    'cpp_std=c++20',
    'warning_level=3',
    'b_ndebug=if-release',
    'buildtype=debug',
  ]
)

### ----------------------------------------------------------------------------
### Compiler Configuration
### ----------------------------------------------------------------------------

cc = meson.get_compiler('cpp')
cmath = cc.find_library('m', required : false)

### ----------------------------------------------------------------------------
### Dependencies
### ----------------------------------------------------------------------------

sdl2 = dependency('SDL2', version : '2.26.5')
sdl2_ttf = dependency('SDL2_ttf', version : '2.20.2')
spdlog = dependency('spdlog', version : '1.13.0')
cloveunit = dependency('clove-unit', version : '2.4.1')

### ----------------------------------------------------------------------------
### Primary Build Target
### ----------------------------------------------------------------------------

exe = executable('pong',
                'src/main.cpp',
                 install : false,
                 include_directories : ['src'],
                 dependencies : [ sdl2, sdl2_ttf, spdlog, cloveunit, cmath ],
                 )
