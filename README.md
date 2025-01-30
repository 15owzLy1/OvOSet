# README

XDU 2025 届毕设代码存档

## Build

构建 benchmarck 库

```bash
git clone https://github.com/google/googletest.git benchmark/googletest
cd benchmark
cmake -E make_directory "build"
cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
cmake --build "build" --config Release
```

构建项目
```bash
cd build
cmake ..
make
```


