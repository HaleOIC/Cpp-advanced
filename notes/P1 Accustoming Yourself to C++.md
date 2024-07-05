# ACCUSTOMING YOURSELF TO C++

## Item 1:view C+++ as a federation of languages

C++ 已经是个多重泛型编程语言(multiparadigm programming language),一个同时支持过程形式，面向对象形式，函数形式，泛型形式，元编程形式的语言。
最简单的方法就是将C++视为一个由相关语言组成的联邦而非单一语言，具体的次语言类型有四种：

- `C`: 说到底C++还是以C为基础,`blocks, statements, preprocessor, built-in data types, arrays, pointer`等均来自C。
- `Object-oriented C++`:通俗来讲叫`C with classes`,典型的面向对象程序设计的思想例如`classes, encapsulation, inheritance, polymorhism, dynamic binding`
- `Template C++`:最复杂，且主流程序员经验最少的部分
- `STL`:本质上是个`Template`库，但是对于`containers, iterators, algorithms, function objects`的规约有着极佳的紧密配合与协调

C++高效编程守则试情况而变化，取决于你使用C++的哪一个部分

## Item 2:Prefer consts, enums, and inlines to #defines

- `#define ASPECT_RATIO 1.653` --> `const double AspectRatio = 1.653`
因为预编译将宏展开，`ASPECT_RATIO`将不会被编译器看到，而`AspectRatio`作为一个语言常量，将会被编辑器记录，并放置到记号表内。但是用常量替代`#defines`时，仍需要注意两种情况：

  - 定义常量指针时，有必要将指针声明称`const`例如：`const char *const authorname = "Shine hale";`
  - 当考虑`class`专属常量时，为了将常量的作用域限制于`class`内，注意保证使用`static`来完成限定如果这个常量要求取地址，则需要额外的定义完成，详见`./codes/Part I/02-1.cpp`,需要明确的是类内的静态变量只是声明式，此时没有地址指向，只有外部再次定义才是定义式，此时将会产生变量对应的地址。

- 同时需要说明的是无法利用`#define`创建一个`class`专属常量，因为`#define`并不注重定义域的问题，这意味着同样无法保证封装性。
- 但是上述写法仍有例外的情况，详见`.codes/Part I/02-2.cpp`,从这个例子可以看到如果你不想他人通过一个指针或者引用实现对你某个整数常量的访问，考虑使用`enum`和`#define`这样不会造成额外的内存分配。
- 尽量不要使用`#define`来实现宏，或许可以参考`Template inline`中的写法完成想要实现的功能，如果一定要使用`#define`，那么尽可能对实参带上括号(尽管还是由很多麻烦在其中)。详见`./codes/Part I/02-3.cpp`

**总结:**

- 对于单纯变量，最好以`const`对象或`enums`替换`#define`
- 对于形似函数的宏`(macros)`，最好改用`inline`函数替换`#define`

## Item 3:Use const whenever possible

- `const`修饰指针由多种情况，具体可解释为，如果出现在星号左边，表示被指物是常量，如果出现在星号右边，则表示指针本身是常量，如果出现在两边，表示被指物和指针同样是常量。

    ```cpp
    const greeating[] = 'Hello';
    char* p = greeting; 
    const char *p = greeting;   //non-const pointer, const data
    char *const p = greeting;   //const pointer, non-const data.
    const char *const p = greeting; // cosnt pointer, const data. 
    ```

- `STL`迭代器就是模拟指针而产生的有两种`std::vector<T>::interator`和`std::vector<T>::const_iterator`
- `const`最有用的声明便是可以在一个函数声明式中将函数的返回值、各参数、函数本身（如果是成员函数）产生关联.
  - const成员函数：
    - 使得`class`结构更容易理解，可以立刻得知哪些函数可以改动对象内容而哪些不能这么操作
    - 为将来提高C++效率的`pass-by-reference-to-const`方式传递对象提供理论基础
  - 如果两个成员函数只有常量性不同，可以被重载，详见`./codes/Part I/03-1.cpp`

- 关于`constness`的讨论:
  - `bitwise constness`认为成员函数只有在不改变对象之任何成员变量时才可以说是`const`,这对编译器来讲很容易实现，但是同样有反直觉的例外情况.
  - 从`./codes/Part I/03-2.cpp`这个例子中，我们看到尽管你只用了`const`类型的接口，但是依然改变了内部的值，但是编辑器需要遵守`bitwise constness`,这种例外的情况致使`logical constness`派登上舞台,他们认为：你可以更改对象的某些值，只要对外不会被检测出即可。因此我们考虑使用`mutable`关键字，例子见`./codes/Part I/03-3.cpp`, 但是`mutable`也无法解决所有问题.
  - 在`const`和`non-const`成员函数中应该避免重复情况的出现,降低代码重复率,提高工作效率.考虑使用`const_static`完成对代码的简化和复用,也成为 **常量性移除**.见`./codes/Part I/03-4.cpp`,此例中说明我们理应重视 **在`non-const`成员函数中调用`const`函数是安全的,推崇如此实现,反之是绝对危险的**

**总结:**

- 将某些东西声明称`const`可以帮助编译器侦测出错误用法.`const`可被施加于任何作用域内的对象,函数参数,函数返回类型,成员函数本体
- 编译器强制实施`bitwise constness`,但你编写程序时应该使用"概念上的常量性`(conceptual constness)`"
- 当`const`和`non-const`成员函数有着实质等价的实现时,令`non-const`版本调用`const`版本可以避免代码重复.

## Item 4:Make sure that objects are initialized before they're used

任何资源获取在使用前都需要确保完成了初始化,而初始化责任落在了构造函数中,**即确保每一个构造函数都将对象中每一个成员进行初始化**,而不同于先完成初始化再进行赋值(即调用一次`default`构造函数,再调用`copy assignment`函数),使用成员初始值列表构造是更高效的做法(调用一次`copy`构造函数):

```cpp
class Boo{
public:
    Boo(const int &ival, const double &dval, vector<int> &ivec) : 
        intval_(ival), dbval_(dval), vec_(ivec);
private:
    int intval_;
    double dbval_;
    vector<int> vec_;
};
```

函数初始化过程是顺序执行的,这样的话就要求保证后面初始化的值,如果需要参数则必须在此之前已经完成了初始化.

**前置概念:**

- `static`对象:寿命从被构造函数出来直到程序结束为止.具体包括:`global`对象,定义于`namespace`作用域中内的对象,在`classes`内,在函数中声明为`static`的对象.
- `local static`对象:特指函数中的`static`对象,在`main()`函数结束时将会调用析构函数销毁.

如果在不同编译单元中,如果本编译单元中的成员所依赖的对象没有完成初始化,将造成灾难性的后果,但是无法计算出来多个编译单元内的`non-local static`对象的构造函数次序,所以考虑下面做法:

```cpp
class FileSystem{/*....*/};
FileSystem &tfs(){
    static FileSystem fs;
    return fs;
}
class Directory {/*....*/};
Directory::Directory(params) {
    ...
    std::size_t disks = tfs().numDisks();
    ...
}
Directory &tempdir(){
    static Directory td;
    return td;
}
```

尽管如此,在多线程中也是带有不确定性的,建议在程序的单线程启动阶段,手工调用所有的`reference-returning`函数,以消除跟初始化相关的`race conditions`(竞速竞争问题)

**总结:**

- 为内置型对象进行手工初始化,因为C++不保证初始化他们
- 构造函数最好使用成员初值列,初值列列出的成员变量,其排列次序应该和他们在class中的声明次序相同
- 为免除"跨编译单元之初始化次序"问题,用`local static`对象替换`non-local static`对象
