### ----------------------------------------------------------------------------
### Meson Configuration
### ----------------------------------------------------------------------------

project('pong', 'c', 'cpp',
  version : '0.1',
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

core_sources = [
    'src/core/app.cpp',
    'src/core/display.cpp',
    'src/core/renderer.cpp',
    'src/core/event_bus.cpp',
    'src/core/vector2.cpp',
    'src/core/rect.cpp',
    'src/core/texture.cpp',
    'src/core/font.cpp',
    'src/core/color.cpp',
]

core_deps = [
    sdl2, sdl2_ttf, spdlog
]

exe = executable('pong',
                'src/main.cpp',
                core_sources,
                'src/game/game.cpp',
                'src/game/entity.cpp',
                'src/game/input_bus.cpp',
                'src/game/components/input_component.cpp',
                'src/game/components/direct_render_component.cpp',
                'src/game/entities/paddle.cpp',
                'src/game/entities/ball.cpp',
                'src/game/entities/score.cpp',
                'src/game/entities/main_menu.cpp',
                 install : false,
                 include_directories : ['src'],
                 dependencies : [ sdl2, sdl2_ttf, spdlog, cloveunit, cmath ],
)

test('Core / App / Single Object Rule',
     executable('test-app-single_object',
                'src/core/tests/app.single_object.cpp',
                core_sources,
                include_directories : ['src'],
                dependencies : core_deps
     )
)
