# IMPLEMENTATIONS

## Item 26:Postpone variable definitions as long as possible

只要你定义了一个变量而其类型带有一个构造函数或者析构函数，当程序控制流到达这个变量控制式时，就得承受构造成本，那么一旦抛出异常可能造成就是变量定义并且构造了但未使用的情况，不妨看下列代码便是一个良好的实现：

```cpp
std::string encryptPassword(const std::string& password) {
    if (password.length() < MinimumPasswordLength) {
        throw logic_error("Password is too short");
    }
    std::string encrypted(password);
    encrypt(encrypted);
    return encrypted;
}
```

必须承认的是，尽可能延后的意思是指：**你不该只延后变量的定义，直到非得使用该变量的前一刻为止，甚至应该尝试延后这份定义直到能够给他实际参数为止**，

如果循环则该怎么办呢，一般有如下两种写法：

```cpp
Widget w;
for (int i = 0; i < n; ++i) {
    w = 取决于i的某个值;
    ...
}

for (int i = 0; i < n; ++i) {
    Widget w(取决于i的某个值);
    ...
}
```

二者区别在于：

- 做法A：1个构造函数 + 1个析构函数 + n个赋值操作
- 做法B：n个构造函数 + n个析构函数

因而除非有：

1. 你知道赋值操作比“构造+析构”成本低
2. 你正在处理代码中效率高度敏感的部分

否则你应该考虑做法B，因为它的作用域更小，更易维护

## Item 27:Minimize casting

除了常见的两种旧式转型`Type(value)`和`(Type)value`之外，C++引入了新的转型方式如下：

- `const_cast`：用来将对象的常量性转移，也是唯一有此能力的`C++ style`转型操作符
- `dynamic_cast`：主要用来执行安全向下转型，它是唯一无法由旧式语法执行的操作，也是唯一可能耗费重大运行成本的转型动作
- `reinterpret_cast`：执行低级转型，实际结果取决于编译器，代表着不可移植，尽量不要使用
- `static_cast`用来强迫隐式转换，可以几乎替代任何的隐式转换

尽可能避免如下代码，这是完全不对的：

```cpp
class Window {
public:
    virtual void onResize() {....}
    ...
};
class SpecialWindow: public Window {
public: 
    virtual void onResize() {
        static_cast<Window>(*this).onResize();
        .....
    }
};
```

上述代码中，你并不是在一个对象中调用了`onResize()`，而是一个副本中调用了`onResize()`,这明显不是我们所期待的，你应该将转型改成`Window::onResize()`即可，指定自己的成分完成对应的函数调用而不是转型。

很多情况下使用`dynamic_cast`是必要的，例如如下代码：

```cpp
class Window {...};
class SpecialWindow: public Window {
public: 
    void blink();
    ....
}
typedef std::vector<std::shared_ptr<Window>> VPW;
VPW winPtrs;
...
for (VPW:iterator iter = winPtrs.begin(); iter != winPtrs.end(); ++iter) {
    if (SpecialWindow *psw = dynamic_cast<SpecialWindow*>(iter->get())) 
        psw->blink();
}
```

优良的C++代码很少使用转型，但是有时候很难摆脱，我们应该尽可能隔离转型操作，把它隐藏在某个函数内，函数的接口会保护调用者不受函数内部任何动作的影响

**总结：**

- 如果可以尽可能避免转型，特别是在注重效率的代码中避免`dynamic_casts`。如果有个设计需要转型操作，试着向无需转型的替代设计
- 如果转型是必须的，那么将他隐藏在某个函数背后，不要让用户自己使用转型
- 宁可使用`c++ style`转型，不要使用旧式转型。

## Item 28: Avoid returning 'handles' to object internals

此条款只需要记住：

- 避免返回`handles`（包括`references`、指针、迭代器）指向对象内部，遵守这个条款可以增加封装性，帮助`const`成员函数的行为更像个`const`，并将发生`dangling handles`的可能性降到最低

## Item 29: Strive for exception-safe code

当异常抛出前，带有异常安全性的函数会：

- **不泄漏任何资源**
- **不允许数据败坏**

如下代码满足上述要求:

```cpp
void PrettyMenu::changeBackground(std::istream& imgSrc) {
    Lock ml(&mutex);
    delete bgImage;
    ++ImageChanges;
    bgImage = new Image(imgSrc);
}
```

而异常安全函数提供以下三个保证之一：

- 基本承诺：如果异常被抛出，程序内任何事物仍然保持在有效状态下。没有任何对象或数据结构会因此而破坏，所有对象都处在一种前后一致的状态
- 强烈保证：如果异常被抛出，程序状态不改变，调用这样的函数需要保持这样的认识：如果函数成功，就是完全成功，如果函数失败，程序会恢复到“调用函数之前的”状态
- 不抛掷保证：承诺绝不抛出异常，因为它们总是能够完成它们原先承诺的功能

下面是基于上述修改的代码：

```cpp
class PrettyMenu {
    ...
    std::shared_ptr<Image> bgImage;
    ...
}
void PrettyMenu::changeBackground(std::istream& imgSrc) {
    Lock ml(&mutex);
    bgImage.reset(new Image(imgSrc));
    ++ImageChanges;
}
```

智能指针内部将完成`delete`操作，几乎完成了强烈保证的承诺

有一个一般化的策略很典型的导致强烈保证，就是`copy and swap`。
原则很简单：为你打算修改的对象做出一份副本，然后在副本本身完成一切必要修改。如果任何修改过程抛出异常，原对象仍保持未改变状态。待所有状态都成功后，再将修改过的副本和原对象在一个不抛出异常的操作中置换`swap`

具体实现如下：

```cpp
struct PMImpl {
    std::shared_ptr<Image> bgImage;
    int imageChanges;
};
class PrettyMenu {
    ...
private:
    Mutex mutex;
    std::shared_ptr<PMImpl> pImpl;
};
void PrettyMenu::changeBackground(std::istream& imgSrc) {
    using std::swap;
    Lock ml(&mutex);
    std::shared_ptr<PMImpl> pNew(new PMImpl(*pImpl));

    pNew->bgImage.reset(new Image(imgSrc));
    ++pNew->imageChanges;

    swap(pImpl, pNew);
}
```

尽管策略如此优雅，但是无法完成对整个函数的保证，并且不幸的说，许多老旧的C++代码并不具备异常安全性，所以今天很多系统仍然不能说是异常安全的，因为它们引入了些并非“异常安全”的代码。

## Item 30:Understand the ins and outs of inlining

1. inlining 在C++程序中是编译期行为
2. 过度热衷inlining会造成程序体积体大，即使有虚拟内存，inline 造成的代码膨胀亦会导致额外的换页行为，降低指令高速缓存装置的击中率，降低效率。
3. virtual 的调用会使得 inlining 落空（运行期确定的多态行为，当然会使得inlining落空）
4. 大部分调试器面对 inline 函数都束手无策，
5. 编译器通常不对“通过函数指针而进行调用”实施`inling`，这意味着对`inline`函数的调用有可能被`inlined`，也可能不被`inlined`，取决于该调用的实施方法

**总结：**

- 一开始不要先将任何函数声明为`inline`，或者至少将`inling`施行范围局限在"一定成为`inline`"或者十分平淡无奇的函数身上
- 不要只因为`function templates`出现在头文件，就将他们声明为`inline`

## Item 31:Minimize compilation dependencies between files

现在开始进入你的C++程序，你对你的类实现做了一个很小的改动。注意，不是接口，只是实现，而且是private部分。然后你需要rebuild你的程序，计算着这个build应该几秒钟就足够了。毕竟，只修改了一个类。你点击了build 或者输入了make( 或者其他方式)，你被惊到了，然后羞愧难当，因为你意识到整个世界都被重新编译和重新链接了！当这些发生时你不觉的感到愤恨么？

问题出在C++并不擅长将接口从实现中分离出来。类定义不仅指定了类的接口也同时指定了许多类的细节。举个例子：

```cpp
class Person {
public:
Person(const std::string& name, const Date& birthday,
const Address& addr);
std::string name() const;
std::string birthDate() const;
std::string address() const;
...
private:
std::string theName; // implementation detail
Date theBirthDate; // implementation detail

Address theAddress;              // implementation detail
};
```

这里，类Person的实现需要使用一些类的定义，也就是string,Date,和Address,如果类Person对这些类的定义没有访问权，那么Person不会被编译通过。这些定义通过使用#include指令来提供，所以在定义Person类的文件中，你可能会发现像下面这样的代码：

```cpp
#include <string>
#include <person.h>
#include <address.h>
```

不幸的是，定义Person类的文件和上面列出的头文件之间建立了编译依赖。任何一个头文件被修改，或者这些头文件依赖的文件被修改，包含Person类的文件就必须要重新编译，使用Person的任何文件也必须要重新编译。这样的级联编译依赖会对一个工程造成无尽的伤痛。

分离的关键在于把对定义的依赖替换为对声明的依赖。这是最小化编译依赖的本质：**在切实可行的情况下让你的头文件能够自给自足，如果达不到这个要求，依赖其他文件中的声明而不是定义。**

一个比较好的实现方式就是： **为声明和定义分别编写头文件**
