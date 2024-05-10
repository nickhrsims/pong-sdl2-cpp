from conan import ConanFile
from conan.tools.meson import Meson, MesonToolchain
from conan.tools.gnu import PkgConfigDeps


class PongSdl2Cpp(ConanFile):
    name = "Pong SDL2 C++"
    version = "1.0"
    description = "Pong Implementation in C++ using SDL2."
    author = "Nicholas H.R. Sims (nickhrsims@gmail.com)"

    settings = "os", "arch", "compiler", "build_type"
    requires = (
        "sdl/2.26.5",
        "sdl_ttf/2.20.2",
        "spdlog/1.13.0",
        "clove-unit/2.4.1",
    )

    options = {"shared": [True, False]}
    default_options = {"shared": False}

    def generate(self):
        pkgconfig = PkgConfigDeps(self)
        toolchain = MesonToolchain(self)

        pkgconfig.generate()
        toolchain.generate()

    def build(self):
        meson = Meson(self)
        meson.configure()
        meson.build()

    def package(self):
        meson = Meson(self)
        meson.install()
