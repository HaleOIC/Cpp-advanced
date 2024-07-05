# DESIGN AND DECLARATION

## Item 18: Make interfaces easy to use correctly and hard to use incorrectly

理想上来讲：

- 如果客户企图使用某个接口而这个接口却没有获得它所预期的行为，这个代码不该通过编译
- 如果代码通过了编译，它的作为就应该是客户所想要的

在实际生产实践中来看，上述情况很难到达，例如考虑如下情况

```cpp
class Date {
public:
    Date (int month, int day, int year);
    ...
}
```

上述定义了一个`Date`类，但是用户很容易犯下错误，例如参数传递顺序不对，或者传递一个无效的参数，而许多客户端的错误都可以通过引入新类型而获得预防，修改如下：

```cpp
struct Day {
    explicit Day(int d) : val(d) {}
    int val;
};
struct Month {
    explicit Month(int m) : val(m) {}
    int val;
};
struct Year {
    explicit Year(int y) : val(y) {}
    int val;
};

class Date {
public: 
    Date(const Month& m, const Day& d, const Year& y);
    ....
}

```

这是一个简单的例子，完整的封装可见`./codes/Part IV/18-1.cpp`

而一个有效的实现可以考虑下面的方式完成：

```cpp
class Month {
public:
    static Month Jan() {return Month(1);}
    static Month Feb() {return Month(2);}
    ...
    static Month Dec() {return Month(12);}
private:
    explicit Month(int m) : val(m);
    int val;
};
```

- **一致性** 可以造成 **接口容易且被正确的使用**
- **非一致性** 将造成 **接口明显的恶化**
- 任何接口如果要求客户必须记得做某些事情，就是有着“不正确使用”的倾向，通常假设客户一定不会做，例如：返回一个`new`对象，并提醒用户`delete`
- 而一个较好的接口，设计原则为先发制人，例如使用`shared_ptr`作为函数返回对象，更甚可以考虑传入删除器，因为`shared_ptr`可以保证自动使用它“每个指针专属的删除器”
- 任何时候，都应该将内存泄露的风险降低到接口内部实现中，不要指望用户会完成内存泄露的预防

具体代码详见 `./codes/Part Iv/18-2.cpp`

**总结:**

- 好的接口很容易被正确使用，不容易被误用。你应该在你所有接口中努力达成这些性质
- “促进正确使用”的办法包括接口的一致性，以及与内置类型的行为兼容
- “阻止误用”的办法包括建立新类型、限制类型上的操作，束缚对象值，以及消除客户的资源管理责任

## Item 19：Treat class design as type design

任何时候想要设计完成一个好的类的话都需要大量的心力和劳累，在设计过程中，请你考虑以下问题，这样有助于你更好的帮助完成类的设计：

- **新type的对象如何被创建和销毁？** 主要是指你的析构函数和构造函数如何产生
- **对象的初始化和对象的赋值该有什么样的差别**： 主要是指构造函数和赋值操作符的行为设计
- **新type对象如果被(Passed by value)**，意味着什么
- **什么是新type的合法值**：对于`class`的成员变量而言，通常只有某些数值集是有效的，这些数值集将要决定你如何维护你的`class`的约束条件，也决定了你的成员函数必须进行的错误检查工作。
- **你的新type需要配合某个继承图系(`inheritance graph`)吗**:如果需要继承某些`classes`，那么就要受到对应`class`接口设计的约束。
- **你的新type需要怎么样的转换？**：如果你只允许`explicit`构造函数存在，就得专门写出负责执行的函数，且不得为类型转换操作符`type conversion operators`和`non-explicit-one-argument`构造函数
- **什么样操作符和函数对此新type而言是合理的**
- **什么样的标准函数应该被驳回**
- **谁该取用新type的成员**
- **什么是新type的“未声明接口(`undeclared interface`)”**
- **你的新type有多么一般化**？或许你应该定义一个`class template`
- **你真的需要一个新type吗** 或许如果只是定义新的`derived class`来为原来的添加功能，那么考虑有没有可能只需要定义几个`non-member`函数即可呢

## Item 20: prefer pass-by-reference-to-const to pass-by-value

除非你另外指定，否则函数参数都是以实际参数的复本为初值，而调用端所获得的也是函数返回值的一个复本，这些复本是对象调用构造函数完成的，可能调用多次的构造函数和析构函数，效率较低，考虑使用一种新的办法`pass-by-reference-to-const`，这样的话没有任何构造函数和析构函数被调用，同时`const`是必要的，为保证调用端不会对其值进行修改。

而`pass-by-reference`方式传递参数也可以避免`slicing`对象分割的问题，具体代码和示例详见`./codes/Part IV/20-1.cpp`

如果你窥探C++编译器的底层，那么`reference`往往以指针的形式实现出来，因此`pass by reference`通常意味着真正传递的是指针。

**上述情况成立的条件是，你处理的对象不是内置类型或者`STL`的迭代器和函数对象，不然将会不成立，这两种更推荐使用`pass-by-value`实现**

## Item 21：Don't try to return a reference when you must return an object

以下是常见的三种错误代码：

- 返回对象是一个`reference`：这样显然不对，引用只是一个实例的别名，如果`local variable`已经在函数结束时销毁，那么这个`reference`将会自动无效
- 返回对象是一个在函数体内`newed`对象，这样将会不可避免的造成内存泄露
- 返回对象是一个函数体内产生的`static`对象的常量引用，则无论如何调用可能都会造成成立的情况，因为`static`只会将变量构造一次

**总结**：

绝不要返回`pointer`和`reference`指向一个`local stack`对象，或者返回`reference`指向一个`heap-allocated`对象，或返回`pointer`和`reference`指向一个`local static`对象而有时时需要多个这样的对象。

## Item 22:Declare data members private

- 从一致性角度考虑，`public`接口中应该尽可能使得每样东西都是函数
- 将成员变量隐藏在函数接口的背后，可以为“所有可能的实现”提供弹性
- 对用户隐藏成员变量，可以确保`class`的约束条件总是被维护
- 假设有一个`public`成员变量，如果我们要取消它，将会造成所有使用它的代码完全破坏，而`protected`变量同理，`derived class`中所有这样的变量都将无法再使用，由此可知`protected`不比`private`更有封装性
- 其实只有两种访问权限：`private`（提供封装）和其他（不提供封装）

**切记将成员变量声明为`private`**，这可赋予用户访问数据的一致性，可细微划分访问控制，允许约束条件得到保证，并提供`class`作者以充分实现的弹性

## Item 23: Prefer non-member non-friend functions to member functions

面向对象守则要求数据应该尽可能被封装，而封装性的本质是指数据能被多少访问，从这个方面中可以看到`non-member non-friend`函数可以提供更好的效果，而`friend`函数对`class private`的成员的访权力和`member`函数相同，一般情况下实现一个工具类来提供，或者考虑如果必要的话，考虑在不同的`namespace`中实现。更好的写法和代码实现见`./codes/Part IV/23-1.cpp`

## Item 24:Declare non-member functions when type conversions should apply to all parameters

考虑如下代码：

```cpp
class Rational {
public:
    ....
    const Rational operator* (const Rational& rhs) const;
};

Rational result = Rational(1, 8) * Rational(1, 2);      // okay, that is good
result = Rational(1, 8) * 2;                // fine
result = 2 * Rational(1, 9);                // wrong !!!
```

上述说明，只有当参数被列于参数列中(`parameter list`)内，这个参数才是隐式类型转换的合格参与者。
所以应该将其声明为`non-member`函数，并且在`class`中不再进行声明即可，并且请记住 **不能够只因该函数不该成为`member`，就自动让它成为`friend`**，所以他也不该是一个`friend`函数

如果你想要为某个函数的所有参数（包括被this指针所指的那个隐喻参数）进行类型转换，那么这个函数必须是`non-member`.完整Rational实现见`./codes/Part IV/24-1.cpp`

## Item 25:Consider support for a non-throwing swap

如果`swap`的缺省实现码对你的`class`和`class template`提供可接受的效率，你不需要额外做任何事情，任何尝试`swap`对象都会取得缺省版本，如果效率不足，考虑如下事情：

- 提供一个`public swap`成员函数，让它高效的置换你的类型的两个对象值，这个函数绝不该抛出异常
- 在你的`class`和`class template`所在的命名空间内提供一个`non-member swap`，并令它调用上述`swap`成员函数
- 如果你正在编写一个`class`，为你的`class`特化`std::swap`。并令他调用你的`swap`成员函数

**成员版本的`swap`**绝对不可抛出异常，各式各样的`swap`详见`./codes/Part IV/25-1.cpp`
