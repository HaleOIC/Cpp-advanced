# INHERITANCE AND OBJECT-ORIENTED DESIGN

C++自身的`OOP`习惯可能会有明显不同，包括了许多情况例如：`public, private, protected`继承?单一继承还是多重继承

## Item 32: make sure public inheritance models 'is-a'

**`public inheritance` 意味着`is-a`（是一种）的关系**，另一种方式可以理解为 **派生类的实例就是一个基类的实例，但是反之不成立**

考虑下面的代码:

```cpp
class Person{};
class Student: public Person{};

void eat(const Person &p);
void study(const Student &s);

Person p;
Student s;
eat(p);      // right!
eat(s);      // right!
study(p);    // wrong!
study(s);    // right!
```

但是，在很多时候`public inheritance`无法完成你想要功能，不如考虑`./codes/Part VI/32-1.cpp`，这样的设计表明 **并不存在一个“适用于所有软件”的完美设计，所谓的最佳设计，取决于现在和未来的选择**。

## Item 33: Avoid hiding inherited names

考虑如下代码的执行结果:

```cpp
class Base {
public:  
    virtual void mf1() = 0;
    virtual void mf1(int);
    virtual void mf2();
    void mf3();
    void mf3(double);
    ...
private:
    int x;
};
class Derived: public Base {
public: 
    virtual void mf1();
    void mf3();
    void mf4();
    ...
}

Derived d;
int x;
d.mf1();            // right!
d.mf1(x);           // wrong, hiding the function in the base 
d.mf2();            // right!
d.mf3();            // right!
d.mf3(x);           // wrong, hiding the function in the base.
```

这些行为是为了防止你在程序库或应用框架内建立新的`derived class`时附带地从疏远的`base classes`继承重载函数。

一个比较好的做法是使用`forwardding function`,它不仅可以完成作用域的扩大，又可以为那些不支持`using`声明式的老旧编译器提供一条新路，将继承而得到的名称汇入到`derived class`作用域内。完整的实现代码见`./codes/Part VI/33-1.cpp`

## Item 34: Differentiate between inheritance of interfaces and inheritance of implementations

身为`class`设计者，需要明确直到`pure virtual`和`simple virtual`以及`non-virtual`的区别的：

- 如果是纯虚函数，那么成员函数的接口总是被继承，因而可以得到纯虚函数的特性：
  - 它们必须被任何“继承了它们”的具象`class`重新声明
  - 在抽象`class`中通常没有定义
- 声明一个`pure virtual`函数的目的是为了让`derived class`自己成函数接口
- 声明一个`impure virtual(simple virtual)`函数的目的，是为了让`derived class`继承该函数的接口的缺省实现，但是我们发现可以轻易做到“提供”缺省实现给`derived class`，但除非它们明确要求并且提出申请，否则并不执行，一个详细的例子见`./codes/Part VI/34-1.cpp`
- 如果一个成员函数是个`non-virtual`函数，意味着他并不打算在`derived class`中有不同的行为，其目的是为了令`derived class`继承函数的接口和一份强制性实现

## Item 35: Consider alternatives to virtual functions

在一个继承层次中,不同的类要实现同一接口的不同实现,最先想到的可能是虚函数,假设一个存在一个继承体系,这个集成体系中的每一层都需要一个名为fun函数,那么可能会像这样实现:

```cpp
class Base{
public:
    ...
    virtual fun(int num){...}
private:
    ...
};
class Derived:public Base{
public:
    ...
    virtual fun(int num){}
private:
    ...
};
```

但除了将fun设为虚函数,还有其他选择.

### "藉由Non-Virtual Interface手法实现Template Method模式"

这一方法的基本思想是"令客户通过public non-virtual成员函数间接调用private virtual函数,成为non-virtual interface(NVI)手法",它是所谓Template Method设计模式(和template无关)的一个独特表现形式.即:

```cpp
class Base{
public:
    ...
    void wrapper(int num){
    ...   //做一些事前工作
    fun(num);
    ...   //做一些事后工作
    }
private:
    virtual void fun(int num){...}
};
class Derived:public Base{
public:
    ...
private:
    virtual void fun(int num){...}
};
```

通过wrapper函数间接调用虚函数fun的优点在于在wraper函数可以在虚函数fun被调用之前做一些准备工作,在fun被调用之后做一些事后工作(正如注释)

但以上代码将fun设为private,因此取消了Derived继承Base的fun的可能性,因此可以将fun设为protected(如果设为public将不能实施NVI手法)

### "藉由Function Pointers是吸纳Strategy模式"

NVI手法只是对virtual函数做了一下修饰,加了一层wrapper函数,另一种替代方案是为继承层次中每一个类加一个函数指针,指向所需的函数,即:

```cpp
class Base{
public:
    typedef void (*PtrF)(int);
    ...
    Base(PtrF para):ptr(para){...}
private:
    PtrF ptr;
}
class Derived:public Base{
public
    ...
    Derived(ptrF para):p(para){...}
private:
    ...
}
```

这样做提供了更大弹性:同一类型不同实体可以调用不同函数;同一实体可调用的函数可以在运行时期变更.

缺点就是p所指向的函数不再是成员函数,如果想要访问类的non-public部分就只能弱化类的封装:将函数设为friend或将使用到的non-public类成员设为public或提供public接口获取non-public成员.

### "藉由tr1::function完成Strategy模式"

中策略提供了一定弹性,但不一定要使用指针,使用`<functional>`中的function类模板可以获得更大弹性(function模板的介绍见条款54),即:

```cpp
class Base{
public:
    typedef void (*PtrF)(int);
    ...
    Base(PtrF para):ptr(para){...}
private:
    PtrF ptr;
}
class Derived:public Base{
public
    ...
    Derived(ptrF para):p(para){...}
private:
    ...
}
```

此策略与3相比较的优点在于,ptr"可持有任何可调用物(callable entity,也就是函数指针,函数对象,或成员函数指针),只要其签名式兼容于需求端",因此提供了比3更大的弹性.ptr的方法像这样:

```cpp
void fun(int num){...}
ptr=&fun;
```

正如以上所言,ptr可以指向非成员函数,static成员函数,也可以指向函数对象,也可用函数指针对其赋值,但要使它指向成员函数还需要其他操作,因为成员函数有默认的this指针作为第一个参数,使用同一头文件中的bind函数模板可以解决这一问题,bind函数模板返回一个函数指针,bind的函数模板原型像这样:

```cpp
template <class Fn, class... Args>
bind (Fn&& fn,Args&&... args);
```

其中第一个参数是函数指针,接下来的参数是函数所需要的参数,如果参数是确定的值,那么bind返回的函数指针的同一位置的参数设为那个值,如果参数要保留,那么使用占位符tr1::placeholders::_1,tr1::placeholders::_2  ...  tr1::placeholders::_N,即:

```cpp
class Demo{
public:
    int fun(int){...}
    ...
private:
    ...
}
Demo tmp;
ptr=bind(&Demo::fun,tmp,tr1::placeholders::_1);
```

这样就把ptr绑定到成员函数了.

### "古典的Strategy模式"

传统的Strategy做法会将fun做成一个分离的继承体系中的虚函数,即:

```cpp
class Fun{
public:
    virtual void fun(int num){...}
    ...
private:
    ...
}
class Base{
public:
    Base(Fun*para):ptr(para){...}
    void fun(){ptr->fun();}
    ...
private:
    Fun* ptr;
    ...
}
```

这种策略的优点在于它提供了将一个既有的fun函数纳入使用的可能性,只要为Fun类集成体系新添加一个derived class即可.

## Item 36: Never redefine an inherited non-virtual function

无论什么情况下：都不该重新定义一个继承而来的`non-virtual`函数

## Item 37: Never redefine a function's inherited default parameter value

`virtual`函数系动态绑定(`dynamically bound`),而缺省值为静态绑定(`statically bound`)，从而应该避免重新定义一个继承而来的缺省函数值。具体例子详见`./codes/Part VI/37-1.cpp`

## Item 38: Model 'has-a' or 'is-implemeted-in-terms-of' through compositions.

在C++中，我们可以使用继承（inheritance）和组合（composition）来表示类之间的关系。继承表示“is-a”关系，例如，一个矩形类可以继承自一个形状类，因为矩形是一种形状。而组合则表示“has-a”或“is-implemented-in-terms-of”关系，例如，一个矩形类可以包含两个点对象表示左上角和右下角的位置，因为矩形由这两个点决定。

在这个条款中，作者指出，使用组合来模拟“has-a”或“is-implemented-in-terms-of”关系通常比继承更好。这是因为组合更加灵活，可以更好地适应需求的变化。

具体来说，使用组合可以带来以下好处：

- 可以在运行时动态地决定对象的实现方式。使用组合，可以在运行时决定一个类的实现方式，而不是在编译时就决定。这使得代码更加灵活，能够适应需求的变化。

- 可以更好地封装类的实现细节。使用组合，可以将一个类的实现细节封装在另一个类中，从而提高了代码的模块化程度和可维护性。

- 可以减少编译依赖。使用组合可以减少编译依赖，因为一个类的实现细节被封装在另一个类中，从而减少了类之间的耦合。

详细的例子见`./codes/Part VI/38-1.cpp`

## Item 39: Use private inheritance judiciously

私有继承纯粹只是一种实现手段，没有其余什么观念上的概念，私有继承有以下几个问题：

- 可能会破坏“is-a”关系。私有继承表示实现细节的隐藏，而不是“is-a”关系。如果一个类从另一个类私有继承，那么它不再是那个类的派生类，而是只是继承了它的实现细节。这可能会导致一些意想不到的结果，例如无法通过指向派生类对象的基类指针访问派生类对象的成员。
- 可能会导致多重继承问题。如果一个类同时从两个类私有继承，那么它会继承两个类的实现细节，但是这两个类之间可能存在互动的问题，导致编译错误或运行时错误。
- 可能会导致实现困难。私有继承会导致基类的接口和实现都被隐藏，从而使得派生类无法直接调用基类的接口。这可能会导致一些实现上的困难。

在使用私有继承时需要慎重考虑，并且应该尽可能使用组合来替代私有继承。使用组合可以达到相同的效果，同时避免私有继承可能会带来的问题.

一个具体的实例详见`./codes/Part VI/39-1.cpp`

## Item 40: Use multiple inheritance judiciously

在C++中，一个类可以同时从多个基类继承，这被称为多重继承。多重继承可以让类获得多个父类的接口和实现，但也可能会引入一些问题。因此，在使用多重继承时需要慎重考虑， 主要有两种问题,命名冲突和虚拟继承。

- 命名冲突：如果从多个基类中继承了同名的成员函数或成员变量，就会发生命名冲突。为了解决这个问题，可以使用作用域解析符号（::）来指定调用哪个基类的成员。
- 虚拟继承：虚拟继承是一种特殊的继承方式，用于解决多重继承中可能会出现的问题。当一个类从多个基类继承，并且这些基类之间存在共同的基类时，就可以使用虚拟继承来避免重复继承共同基类所带来的问题。

**总结：**

- 多重继承比单一继承复杂，有如上两种问题
- 虚拟继承会增加大小、速度、初始化（及赋值）复杂度等等成本，如果`virtual base classes`不带任何数据，将是最具实用价值的情况。
- 多重继承也有很多正当用途，例如`private继承某个协助实现的class`的组合.