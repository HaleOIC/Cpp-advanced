# CON/DE STRUCTORS, ASSIGNMENT OPERATORS

## Item 5: Know what functions C++ silently writes and calls

`class Empty {};` 将会自动被调用成如下代码:

```cpp
class Empty{
public:
    inline Empty() {...}
    inline Empty(const Empty &rhs) {...}
    ~Empty() {...}

    Empty & operator= (const Empty &rhs) {...}
};
```

如果类中声明了一个构造函数,则编译器将不再为它创建`default`构造函数.`copy`构造函数和`copy assignment`同理,但是默认的`copy assignment`如果没有定义,需要满足如下的两个条件才能自动生成默认:

- 成员变量不为引用或常量
- 基类中赋值构造函数不为`private`, 此时讨论的是派生类中的赋值构造函数

因为如果成员变量为引用和常量时,编译器无法明白两个引用之间如何赋值,而常量则无法进行默认的赋值操作,这时编译器将会报错;而如果基类中的赋值构造函数存在但是为`private`类型,则子类无法调用,编译器无法完成工作.

## Item 6:Explicitly disallow the use of complier-generated functions you do not want

编译器自动生成的构造函数都是`public`,所以如果你不想使用编译器自动生成的,可以考虑将拷贝构造函数,赋值构造函数声明成`private`,而不去定义, 如下所示

```cpp
class A{
public: 
    ...
private:
    A (const A&);
    A& operator= (const A&);
};
```

上述代码,当友元函数或者成员函数调用时,将会产生错误在`linker`中,如果想要将其移到编译器中,则考虑实现一个基类中,如下所示:

```cpp
class Uncopyable{
protected:
    Uncopyable() {}
    ~Uncopyable() {}
private:    
    Uncopyable (const Uncopyable&);
    Uncopyable & operator=(const Uncopyable&);
};
```

此时只考虑继承这个类就可以完成实现,在派生类中不再声明拷贝构造函数和赋值构造函数即可.

## Item 7:Declare destructors virtual in polymorphic base classes

- 如果一个基类将要被其他类继承,则需要声明自己的析构函数为虚函数,不然会造成内存泄露的情况
- 如果`class`不含`virtual`函数,通常表示自己并不意图被用作一个`base class`,这时不要将其声明为`virtual`,如果强制设置成为`virtual`,那么即便程序实现细节到位,也将不再具有可移植性
- 所有`STL`容器如`vector, list, set, map`等等,都不可被继承,因为他们都有`non-virtual`析构函数.
- `pure virtual`纯虚函数可以导致类成为`abstract classes`,即不可被实例化无法被创建为对象具体写法为`virtual ~ClassName () = 0;`这是一个定义式.
- **如果类的设计不是作为`base class`使用,则不要将其析构函数设置成`virtual`类型**

具体更详细的例子见`./codes/Part II/07-1.cpp`

## Item8: Prevent exceptions from leaving destructors

- 析构函数绝对不要吐出异常.如果一个被析构函数调用的函数可能抛出异常,析构函数应该捕捉任何异常,然后吞下他们(不传播)或结束程序
- 如果客户需要对某个操作函数运行器件抛出的异常做出反应,那么`class`应该提供一个普通函数(而非析构函数)执行此操作.

具体详见`./codes/Part II/08-1.cpp`

## Item 9:Never call virtual functions during construction or destruction

在`base class`构造期间,`virtual`函数不是`virtual`函数,即由于构造函数调用顺序的问题,基类构造函数执行时,派生类的函数还没有完成`overload`,同样的道理适用于析构函数.

如果确实想要在构造函数中调用某些功能,考虑在派生类中构造函数执行中传递参数给基类,来弥补无法使用`virtual`函数从`base classes`向下调用的情况.具体例子详见`./codes/Part II/09-1.cpp`, 注意其中派生类中`private static`用法

## Item 10:Have assignment operators return a reference to *this

考虑如下的连续赋值的情况

```cpp
int x , y, z;
x = y = z = 100;
```

如果想要实现这样的效果,需要在`operator=`中将`*this`返回即可,`STL`库中均遵守了这样的协议,具体实现如下:

```cpp
class Foo{
    Foo& operator+=(const Foo& rhs){
        ....
        return *this;
    }
    Foo& operator=(const Foo& rhs){
        ....
        return *this;
    }
};
```

## Item11 在`operator=`中处理"自我赋值"

自我赋值其实很常见,例如`a[i] = a[j]`,如果`i = j`时,将会发生自我赋值的情况,再比如`*px = *py`,指针或者引用指向同一个地址时,也会有隐式的自我赋值的情况,在函数中表现为`operator=`函数中`*this`和`rhs`有可能是同一个对象,此时如果冒失的`delete`资源则造成访问无效地址问题,具体解决办法如下:

1. 使用正同测试`identity test`来检验是否为同一个对象
2. 如果`new`新对象时发生异常,则将造成内存泄露

一个比较好的实现方法如下:

```cpp
Foo& Foo::operator=(const Foo& rhs){
    TypeObj* tmp = pobj;
    obj = new TypeObj(*rhs.obj);
    delete tmp;
    return *this;
}
```

而更好的实现办法考虑使用`copy and swap`技术,详见`Item 29`

## Item 12 Copy all parts of an object

如果你自己声明`copying`函数(指`copy assignment`赋值和`copy constructor`),那么编译器将会在你代码近乎必然出错的情况下,依然不提醒你,这要求自己在编写`copying`族函数的时候需要万般小心,具体要求两点:

- `copying`函数应该确保复制"对象内的所有成员变量"及"所有`base class`成分"
- 不要尝试以某个`copying`函数实现另一个`copying`函数,如果需要代码复用的话,考虑将相同的功能放置到一个`private void`成员函数中,由两个`copying`函数共同调用即可.

具体例子可见`./codes/Part II/12-1.cpp`,其尽可能包含了`Part II`的全部细节.
