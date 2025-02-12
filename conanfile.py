from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake, CMakeDeps, CMakeToolchain


class RtOneWeekend(ConanFile):
    name = "RaytracingInAWeekend"
    version = "1.0"
    package_type = "application"

    license = "MIT"
    author = "arnor-nolen"
    url = "https://github.com/arnor-nolen/rt-one-weekend"
    description = "Raytracing in one weekend implementation."
    topics = ("C++", "Conan", "Raytracing", "Raytracing in one weekend")

    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "src/*"

    default_options = {
        "cimg/*:enable_fftw": False,
        "cimg/*:enable_jpeg": False,
        "cimg/*:enable_openexr": False,
        "cimg/*:enable_png": True,
        "cimg/*:enable_tiff": False,
        "cimg/*:enable_ffmpeg": False,
        "cimg/*:enable_opencv": False,
        "cimg/*:enable_magick": False,
        "cimg/*:enable_xrandr": False,
        "cimg/*:enable_xshm": False,
    }

    def requirements(self):
        self.requires("cimg/3.3.0")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.user_presets_path = "ConanPresets.json"
        vars = {}
        vars["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        if tc.generator == "Ninja":
            vars["CMAKE_COLOR_DIAGNOSTICS"] = "ON"
        tc.cache_variables = vars
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
