# MISCELLANY

## Item 53: Pay attention to compiler warnings

编译警告在C++中很重要，因为它可能是个错误啊！ 不要随便忽略那些警告，因为编译器的作者比你更清楚那些代码在干什么。 所以, **请严肃对待所有warning，要追求最高warning级别的warning-free代码；**但不要依赖于warning，可能换个编译器有些warning就不在了。
其实在多数项目实践中，不仅要消除所有编译警告，消除所有代码风格检查警告也是常见惯例。

还是看一个常见的错误吧，编译器会帮你发现它。比如我们想在D中重写B中的虚函数f()：

```cpp
class B{
public:
    virtual void f() const;
};
class D:public B{
public:
    virtual void f();
};
```

我们忘记写const了！这已经不是重写虚函数了，而是定义同名函数而彻底隐藏父类中的`void f() const`。 所以编译器会给警告: `warning: D::f() hides virtual B::f()`
编译器的意思是B中没有声明过这样一个f。但很多无知的程序员会想：当然`D::f`隐藏了`B::f`，这就是我要的结果啊！ 却没有想到是自己忘写了const。这里犯的错误可能会导致长时间的debug，就因为你忽略了编译器早就发现的一个问题。

当你有很多经验时便能识别那些warning到底在说什么，但最好的习惯还是消除多有warning。因为当warning井喷时很容易忽略其中的严重问题。 至少当你忽略一个warning时，要确保你已经完全理解了它在说什么。

## Item 54: Familiarize yourself with the standard library, including TR1

C++这个名字是在1983年由Rick Mascitti提出的，C++的曾用名还有"new C", "C with Classes"。 1998年ISO/IEC 14882发布了第一个C++标准，就是我们常讲的C++98。后续的标准还包括C++03，C++TR1，C++11，C++14。 值得一提的是C++11标准，它已经被主流编译器支持。包含了核心语言的新机能，而且扩展C++标准程序库，并入了大部分的C++ TR1程序库。 Effective C++ Edition 3中大部分std::tr1都可以在std下直接访问了。

我们来回顾一下C++98中的内容：

- STL（standard template library），包括容器迭代器和算法。
- IOstream，支持标准IO，用户定义IO，以及预定义对象：`cin, cout, cerr, clog`。
- 国际化支持。如wchar_t为16位Unicode字符。
- 数字处理。加入了complex, valarray等。
- 异常层级体系。包括基类`exception`, 子类`logic_error, runtime_error`等。
- C89的标准库。1989年的C标准库都并入了C++98。
Effective C++ Edition 3中提到了很多TR1(technical report 1)的概念：

- 智能指针：`TR1::shared_ptr`, `TR1::weak_ptr`, `TR1::auto_ptr`等，见Item 13。
- `TR1::function`：可以代表一个可调用的实体，可以是函数、函数对象等，见Item 35。
- `TR1::bind`：将某个函数绑定到某个对象上（即替换this），见Item35。
- 哈希表：`TR1::unordered_set, TR1::unordered_multiset, TR1::unordered_map, TR1::unordered_multimap。`
- 正则表达式
- 元组：在STL中本已有一个pair模板来包含两个元素，在TR1中提出了不限元素数量的`TR1::tuple`。
- `TR1::array`：STL风格的数组。
- `TR1::mem_fn`：提供了统一的方式来适配成员函数指针。
- `TR1::reference_wrapper`：使得引用更像一个对象，原本在容器中只能存储指针和对象的。
- 随机数：C++的rand来自于C89标准中，TR1给出了更好的随机数算法。
- 特殊数学函数：拉格朗日多项式、贝塞尔函数、椭圆积分等。
- C99兼容的扩展：引入了很多C99的特性。
- 类型特征（traits）：一个类模板，用来在编译期标识类型信息，见Item 47。
- `TR1::result_of`：一个模板，用来推导函数调用的返回类型。

## Item 55: Familiarize yourself with Boost

Boost是一个C++开发者的社区，作为C++标准的试验场， 收容了很多高质量、开源的、跨平台、独立于编译器的C++库， 包括许多TR1组件的实现。

Boost是其他C++组织和社区所不能比拟的：

与C++标准委员会的亲近关系。Boost社区成员和C++标准委员会成员有很大的交集。 多数C++下一代标准都来自于Boost社区。
特殊的项目接收流程。首先在邮件列表中提出它，然后开启整个流程： 讨论、优化、重新提交，直到满意。
Boost社区中的库千差万别，较小的库（比如Convention）除了错误处理和跨平台外只有几行代码； 较大的库（比如Boost Graph Library、Boost MPL Library） 却需要整本的书来写它们。这些库可以分为以下几个大的类别：

- 字符串和文本处理。类型安全的printf、正则表达式、 分词（tokenizing）、转换（parsing）。
- 容器。STL风格的固定大小数组、可变大小的Bitset、多维数组。
- 函数对象和高阶编程。比如有趣的Lambda库：

```cpp
  using namespace boost::lambda;
  std::vector<int> v;
  std::for_each(v.begin(), v.end(), std::cout<< _1 * 2 + 10 << "\n");
```

> _1 是 Lambda 库的占位符，表示当前元素

- 泛型编程。包括一套可扩展的traits类，见Item 47。
- 模板元编程（TMP，见Item 48）。
- 数值计算。包括有理数、八元数、最大公约数、最小公倍数等。
- 正确性和测试。隐式模板接口的形式化和测试驱动编程。
- 数据结构。包括类型安全的联合体、元组。
- 跨语言支持。比如C++和Python提供无缝互操作性的库。
- 内存。如固定大小内存池的分配器。
- 杂项。CRC检查、时间日期操作、文件系统遍历等。
也有一些程序设计领域的东西并不在Boost里面， 比如GUI开发、数据库连接等
