## Accustoming Yourself to C++
### Item 1:view C+++ as a federation of languages
C++ 已经是个多重泛型编程语言(multiparadigm programming language),一个同时支持过程形式，面向对象形式，函数形式，泛型形式，元编程形式的语言。
最简单的方法就是将C++视为一个由相关语言组成的联邦而非单一语言，具体的次语言类型有四种：

- `C`: 说到底C++还是以C为基础,`blocks, statements, preprocessor, built-in data types, arrays, pointer`等均来自C。
- `Object-oriented C++`:通俗来讲叫`C with classes`,典型的面向对象程序设计的思想例如`classes, encapsulation, inheritance, polymorhism, dynamic binding`
- `Template C++`:最复杂，且主流程序员经验最少的部分
- `STL`:本质上是个`Template`库，但是对于`containers, iterators, algorithms, function objects`的规约有着极佳的紧密配合与协调

C++高效编程守则试情况而变化，取决于你使用C++的哪一个部分

### Item 2:Prefer consts, enums, and inlines to #defines.
`#define ASPECT_RATIO 1.653` --> `const double AspectRatio = 1.653`
因为预编译将宏展开，`ASPECT_RATIO`将不会被编译器看到，而`AspectRatio`作为一个语言常量，将会被编辑器记录，并放置到记号表内。但是用常量替代`#defines`时，仍需要注意两种情况：
- 定义常量指针时，有必要将指针声明称`const`例如：`const char *const authorname = "Shine hale";`
- 当考虑`class`专属常量时，为了将常量的作用域限制于`class`内，注意保证