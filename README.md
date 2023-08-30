# opengl-soft-render

### 介绍

在 linux 平台下自己实现了 opengl 软件渲染管线，使用 gles2.0 标准，简单实现了 gles2.0 库，以及整个 opengl 渲染管线。

所有的例子都在 example 目录下，可以修改 main.cpp 里面的内容，使其运行不同的测试用例，目前总共包含了 30+ 测试用例，覆盖了点、线、三角形、纹理、纹理压缩、深度、模板、混合、scissor 测试，fbo 等场景

### 编译方法（Linux）

```bash
git clone https://github.com/yangpan4485/opengl-soft-render.git
cd opengl-soft-render
mkdir build
cd build
cmake ..
make -j
./render_demo
```

