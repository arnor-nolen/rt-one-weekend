# rt-one-weekend
Raytracing in one weekend. Based on the [book with the same name](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

This project tries to use modern C++20 features to improve on the original code.

## Dependencies
Dependencies are listed in [conanfile.py](./conanfile.py).

## How to build
* In one command:
```sh
conan build . -s build_type=Debug --build=missing
```

* Using `conan` and `cmake`:
```sh
conan install . -s build_type=Debug --build=missing
cmake --workflow --preset debug # Use multi-debug for multi-configs.
```
Check contents of [CMakePresets.json](./CMakePresets.json) for more info.

## How to run
```sh
./build/Debug/bin/rt-one-weekend > output/image.ppm
```

## Result
Here's the output produced by the program:
![Image](./output/image.ppm "Image")
