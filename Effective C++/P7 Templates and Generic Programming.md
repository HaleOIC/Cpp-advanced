# TEMPLATES AND GENERIC PROGRAMMING

## Item 41: Understand implicit interfaces and compile-time polymorphism

面向对象设计中的类（class）考虑的是**显式接口（explicit interface）和运行时多态**， 而模板编程中的模板（template）考虑的是**隐式接口（implicit interface）和编译期多态。**

对类而言，显式接口是由函数签名表征的，运行时多态由虚函数实现；
对模板而言，隐式接口是由表达式的合法性表征的，编译期多态由模板初始化和函数重载的解析实现。

### 显式接口和运行时多态

一个类的显式接口是由public成员函数签名（包括函数名、参数类型、返回值类型等）、类型定义（typedef）、public数据成员构成的。

```cpp
class Widget{
public:
    Widget();
    virtual ~Widget();
    virtual size_t size() const;
    virtual void normalize();
    void swap(Widget& other);
};
void doProcessing(Widget& w){
    if(w.size() > 10 && w != someOne){
        Widget tmp(w);
        tmp.normalize();
        tmp.swap(w);
    }
}
```

对于doProcesing中的w，我们可以知道：

w应支持Widget的接口，包括：normalize(), swap()等。这些接口在源码中是可以找到的，称为显式接口。Widget有些成员函数是virtual的，会表现出运行时多态：具体的被调用者会根据Widget的动态类型而决定。

### 隐式接口和编译期多态

在模板和类属编程（generic programming）中这一点完全不同，在这里隐式接口和编译期多态更为重要：

```cpp
template<typename T>
void doProcessing(T& w){
    if(w.size() > 10 && w!= someOne){
        T tmp(w);
        tmp.normalize();
        tmp.swap(w);
    }
}
```

现在的doProcessing是一个函数模板，其中的w有所不同：
w应支持的接口取决于模板中w上的操作。比如：w（类型T）必须支持size, normalize, swap方法；拷贝构造函数；不等运算符。 总之，这些表达式必须合法而且通过编译构成了w应支持的接口。
其中的operator>和operator!=要调用成功可能需要实例化一些模板，而使用不同的模板参数实例化模板的过程就是编译期多态。
具体来讲，T的隐式接口应满足：

- **必须包含一个返回值为整型的成员函数**；
- **支持一个接受T类型的operator!=函数。**

但由于C++的运算符重载和隐式类型转换特性，上述两个条件都不需要满足。 首先size可能是继承来的函数而非T的成员函数，但它不需要返回一个int，甚至不需要返回一个数字类型，返回类型也不需要定义operator>。 它需要返回的类型X只需满足：operator>可以接受X和int。但operator>的第一个参数类型可以不是X，只要X能隐式转换为它的第一个参数类型即可。 类似地，operator!=接口也有极大的灵活性。

当你想到这些约束时可能真的会头大，但实践中比这些直观的多，接口只是由合法的表达式构成的。 例如下面的表达式看起来就很直观：

`if (w.size() > 10 && w != someOne) ...`
总之隐式接口和显式接口一样地真实存在，在编译时都会进行检查。正如你错误地使用显式接口会导致编译错一样， 对象不支持模板所要求的隐式接口也会导致编译错。

## Item 42: Understand the two meanings of typename

查看下面的两个定义式:

```cpp
template<class T> class Widget;
template<typename T> class Widget;
```

请问它们有何区别，答案是 **没有区别**, 有人觉得class写起来方便就用class，有人觉得typename语义更正确就用typename。 但是在这个场景之外，typename 和 class 却是完全不同的东西，下文提供了 typename 的其他用法。 比如帮助编译器识别嵌套从属类型名称。

### 声明一个类型

typename的第一个作用在于声明一个类型。为什么类型还需要声明呢？因为编译器并不是总会知道哪个名称是个类型。 下面的代码会编译错：

```cpp
template<typename C>
void print2nd(const C& container){
    if(container.size() >= 2){
        C::const_iterator it(container.begin());
        ++it;
        int value = *it;  
        cout<<value;
    }
}
```

发生编译错误是因为编译器不知道`C::const_iterator`是个类型。万一它是个变量呢？ `C::const_iterator`的解析有着逻辑上的矛盾，直到确定了C是什么东西，编译器才会知道`C::const_iterator`是不是一个类型； 然而当模板被解析时，C还是不确定的。这时我们声明它为一个类型才能通过编译：

`typename C::const_iterator it(container.begin());`

### 嵌套从属名称

事实上类型`C::const_iterator`依赖于模板参数C， 模板中依赖于模板参数的名称称为从属名称（`dependent name`）， 当一个从属名称嵌套在一个类里面时，称为嵌套从属名称（`nested dependent name`）。 其实`C::const_iterator`还是一个嵌套从属类型名称（`nested dependent type name`）。

嵌套从属名称是需要用typename声明的，其他的名称是不可以用typename声明的。比如下面是一个合法的声明：

```cpp
template<typename C>
void f(const C& container, typename C::iterator iter);
```

如果你把`const C&`也声明了`typename`也是要编译错的哦：

```cpp
template<typename C>
void f(typename const C& container, typename C::iterator iter);
```

错误输出：

```text
error: expected a qualified name after 'typename'
```

### 一个例外

模板中的嵌套从属名称是需要typename声明的，然而有一个例外情况： 在派生子类的基类列表中，以及构造函数的基类初始化列表中，不允许typename声明。 例如`Derived<T>`继承自`Base<T>::Nested`：

```cpp
template<typename T>
class Derived: public Base<T>::Nested{  // 继承基类列表中不允许声明`typename`
public:
    explicit Derived(int x): Base<T>::Nested(x){    // 基类初始化列表不允许声明`typename`
        typename Base<T>::Nested tmp;   // 这里是要声明的
    }
};
```

### traits

C++提供了一系列的traits模板，用来提供类型信息。比如：

```cpp
template<typename IterT>
void workWithIterator(IterT it){
    typename std::iterator_traits<IterT>::value_type tmp(*it);
}
```

其实上述模板方法也可以不同traits来实现，比如：

```cpp
template<typename container>
void workWithIterator(typename container::iterator it){
    typename container::value_type tmp(*it);
}
```

但traits提供了更加一致的使用方式以及容器实现的灵活性，模板代码也简洁了不少。 尽管如此，程序员还是懒惰的。我们倾向于用typedef来给这些嵌套从属名称起一些别名：

```cpp
template<typename IterT>
void workWithIterator(IterT it){
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    value_type tmp(*it);
}
```

虽然typedef typename看起来也很怪异，但你想敲很多遍`typename std::iterator_traits<Iter>::value_type`么？

## Item 43: Know how to access names in templatized base classes

从面相对象C++转移到模板C++时，你会发现类继承在某些场合不在好使了。 比如父类模板中的名称对子类模板不是直接可见的，需要通过this->前缀、using或显式地特化模板父类来访问父类中的名称。

因为父类模板在实例化之前其中的名称是否存在确实是不确定的，而C++偏向于早期发现问题（`early diagnose`），所以它会假设自己对父类完全无知。

### 编译错的一个例子

一个MsgSender需要给多个Company发送消息，我们希望在编译期进行类型约束，于是选择了模板类来实现MsgSender。

```cpp
template<typename Company>
class MsgSender{
public:
    void sendClear(const MsgInfo& info){...}    // 发送明文消息
    void sendSecret(const MsgInfo& info){...}   // 发送密文消息
};
```

由于某种需求我们需要继承MsgSender，比如需要在发送前纪录日志：

```cpp
template<typename Company>
class LoggingMsgSender: public MsgSender<Company>{
public:
    void sendClearMsg(const MsgInfo& info){
        // 存储一些日志
        sendClear(info);    // 编译错！
    }
};
```

首先要说明这里我们创建了新的方法`sendClearMsg`而不是直接重写`sendClear`是一个好的设计， 避免了隐藏父类中的名称，见Item 33；也避免了重写父类的非虚函数，见Item 36。

编译错误发生的原因是编译器不知道父类`MsgSender<Company>`中是否有一个sendClear，因为只有当`Company`确定后父类才可以实例化。 而在解析子类`LoggingMsgSender`时父类`MsgSender`还没有实例化，于是这时根本不知道`sendClear`是否存在。

为了让这个逻辑更加明显，假设我们需要一个公司CompanyZ，由于该公司的业务只能发送密文消息。所以我们特化了`MsgSender`模板类：

```cpp
template<>
class MsgSender<CompanyZ>{
public:
    void sendSecret(const MsgInfo& info){...}   // 没有定义sendClear()
};
```

`template<>`意味着这不是一个模板类的定义，是一个模板类的全特化（total template specialization）。 我们叫它全特化是因为MsgSender没有其它模板参数，只要`CompanyZ`确定了`MsgSender`就可以被实例化了。

现在前面的编译错误就更加明显了：如果`MsgSender`的模板参数`Company == CompanyZ`， 那么`sendClear()`方法是不存在的。这里我们看到在模板C++中继承是不起作用的。

### 访问模板父类中的名称

既然模板父类中的名称在子类中不是直接可见的，我们来看如何访问这些名称。这里介绍三种办法：

#### this指针

父类方法的调用语句前加`this->`：

```cpp
template<typename Company>
class LoggingMsgSender: public MsgSender<Company>{
public:
    void sendClearMsg(const MsgInfo& info){
        ...
        this->sendClear(info);
    }
};
```

这样编译器会假设sendClear是继承来的。

#### using 声明

把父类中的名称使用using声明在子类中。该手法我们在Item 33中用过，那里是为了在子类中访问被隐藏的父类名称， 而这里是因为编译器不会主动去搜索父类的作用域。

```cpp
template<typename Company>
class LoggingMsgSender: public MsgSender<Company>{
public:
    using MsgSender<Company>::sendClear;  
    void sendClearMsg(const MsgInfo& info){
        ...
        sendClear(info);
    }
};
```

using语句告诉编译器这个名称来自于父类`MsgSender<Company>`。

#### 调用时声明

最后一个办法是在调用时显式指定该函数所在的作用域（父类）：

```cpp
template<typename Company>
class LoggingMsgSender: public MsgSender<Company>{
public:
    void sendClearMsg(const MsgInfo& info){
        ...
        MsgSender<Company>::sendClear(info);
    }
};
```

这个做法不是很好，因为显式地指定函数所在的作用域会禁用虚函数特性。万一sendClear是个虚函数呢？

子类模板无法访问父类模板中的名称是**因为编译器不会搜索父类作用域**，上述三个办法都是显式地让编译器去搜索父类作用域。 但如果父类中真的没有`sendClear`函数（比如模板参数是CompanyZ），在后续的编译中还是会抛出编译错误。

## Item 44: Factor parameter-independent code out of templates

模板是个好东西，你可以在保证类型安全的同时用一份代码同时兼容不同类型。 但模板提供的是编译期的多态， 即使你的代码看起来非常简洁短小，生成的二进制文件也可能包含大量的冗余代码。 因为模板每次实例化都会生成一个完整的副本，所以其中与模板参数无关的部分会造成代码膨胀（code bloat）。

把模板中参数无关的代码重构到模板外便可以有效地控制模板产生的代码膨胀。 另外代码膨胀也可以由类型模板参数产生：

对于非类型模板参数产生的代码膨胀，用函数参数或数据成员来代替模板参数即可消除冗余；
对于类型模板参数产生的代码膨胀，可以让不同实例化的模板类共用同样的二进制表示。
抽取公共代码
在避免代码冗余的问题上，抽取公共代码（commonality and variability analysis）是我们每天都在用的方法。 当你写几个函数时，会把其中的公共部分抽取到另一个函数；当你声明类时，也会把它们的公共部分抽取到父类中。

于是你希望在模板编程中也用该办法来避免代码重复，但模板和非模板代码在这一点上是不同的：

非模板的代码中，冗余的显式的（explicit）。只要有重复代码你都会看到它；
模板代码中，冗余是隐式的（implicit）。模板代码只有一份，模板被实例化时产生的冗余需要你的直觉才能感受到。
模板产生的代码膨胀
现在来看一个模板是怎样引发代码膨胀的。比如要实现一个固定大小的矩阵，它支持转置运算。

```cpp
template<typename T, int n>
class Square{
public:
    void invert();
};
```

其中的int n是一个非类型参数，它也是一种合法的模板参数~

然后可能会这样使用该模板：

```cpp
Square<double, 5> s1;
Square<double, 10> s2;
s1.invert();    
s2.invert();
```

Square模板会实例化两个类：`Square<double, 5>和Square<double, 10>`，它们拥有相同的invert方法。 这是模板产生代码膨胀的典型场景。

### 抽取父类模板

结局模板产生的代码膨胀，仍然是用抽取公共代码的办法。如果你真的看到了二进制代码中两个相同的invert函数， 你的直觉肯定是把它抽取到另一个类中：

```cpp
template<typename T>
class SquareBase{
protected:
    void invert(int size);
};

template<typename T, int n>
class Square:private SquareBase<T>{
private:
    using SquareBase<T>::invert;
public:
    void invert(){ this->invert(n); }
}
```

因为invert函数定义在基类中，所以它只会在二进制代码中出现一次，即`SquareBase<double>::invert`。该函数由两个子类共享。 上述代码中有些细节还值得一提：

`SquareBase::invert`是供子类用的，所以声明为`private`而不是`public`；
调用父类invert的代价为零.

### 数据存储问题

既然我们决定由父类来做invert操作，那么父类怎么访问数据呢？因为数据本来是在子类中的。 当然我们可以在调用SquareBase::invert时把内存地址也一起告知父类， 但如果矩阵类中有很多函数都需要这些信息呢？我们可能需要调用每个函数时都把这些信息传递给父类函数。 既然这样，何不把数据地址直接放在父类中？既然父类存放了数据，那么就把矩阵大小也一并存放了吧！

```cpp
template<typename T>
class SquareBase{
protected:
    SquareBase(int _n, T *_data): n(_n), data(_data){}
    void setData(T *_data){
        data = _data;
    }
private:
    int n;
    T* data;
};
```

父类中存储了矩阵数据的位置（data）以及大小（n），子类仍然可以决定如何分配地址空间。 可以存放在子类中作为成员属性，也可以动态申请内存。

### 权衡

不管数据是怎样分配和访问的，我们消除代码重复的方案是确定的：将公共部分抽取到父模板类中。 这样做的好处便是避免了代码膨胀，减小了二进制文件和"working set"的大小，有利于提高指令缓存的命中率， 从而达到更高的代码执行效率。但提取公共部分到新的模板类也造成了一些问题：

如果int n硬编码在模板参数中的话，编译器能做更多的优化，比如常量传播等。但int n作为函数参数，这些优化就没有了。
添加类的层级会导致对象大小的增加。至少多存储了一个T* data指针。
实践中到底是否应该抽取公共代码出来取决于你的应用场景，在上述的优劣中进行权衡。

本问讨论的是非类型模板参数，对于类型模板参数，代码膨胀的问题也是存在的，比如

int和long在多数平台都是一样的底层实现，然而模板却会实例化为两份，因为它们类型不同。
`List<int *>, List<const int *>, List<double *>`的底层实现也是一样的。但因为指针类型不同，也会实例化为多份模板类。

## Item 45: Use member function templates to accept "all compatible types"

Item 13提到智能指针可用来自动释放堆中的内存，STL中的迭代器也是一种智能指针，它甚至支持链表元素指针的++操作。 这些高级特性是普通指针所没有的。本文以智能指针为例，介绍成员函数模板的使用：

成员函数模板可以使得函数可以接受所有兼容的类型。
如果你用成员函数模板声明了拷贝构造函数和赋值运算符，仍然需要手动编写普通拷贝构造函数和拷贝运算符。
隐式类型转换
智能指针虽然比普通指针提供了更多有用的特性，但也存在一些问题，比如我们有一个类的层级：

```cpp
class Top{};
class Middle: public Top{};
class Bottom: public Middle{};
```

普通指针可以做到派生类指针隐式转换为基类指针：

```cpp
Top *p1 = new Bottom;
const Top *p2 = p1;
```

但如果是智能指针，比如我们实现了SmartPtr，我们则需要让下面代码经过编译：

```cpp
SmartPtr<Top> p1 = SmartPtr<Bottom>(new Bottom);
SmartPtr<const Top> p2 = p1;
```

同一模板的不同实例之间是没有继承关系的，在编译器看来`AutoPtr<Top>`和`AutoPtr<Bottom>`是完全不同的两个类。 所以上述代码直接编译是有问题的。

### 重载构造函数

为了支持用`SmartPtr<Bottom>`初始化`SmartPtr<Top>`，我们需要重载SmartPtr的构造函数。 原则上讲，有多少类的层级我们就需要写多少个重载函数。因为类的层级是会扩展的，因此需要重载的函数数目是无穷的。 这时便可以引入成员函数模板了：

```cpp
template<typename T>
class SmartPtr{
public:
    template<typename U>
    SmartPtr(const SmartPtr<U>& other);
};
```

注意该构造函数没有声明为explicit，是为了与普通指针拥有同样的使用风格。子类的普通指针可以通过隐式类型转换变成基类指针。

接受同一模板的其他实例的构造函数被称为通用构造函数（`generalized copy constructor`）。

### 兼容类型检查

事实上，通用构造函数提供了更多的功能。他可以把一个`SmartPtr<Top>`隐式转换为`SmartPtr<Bottom>`，把一个`SmartPtr<int>`转换为`SmartPtr<double>`。 但普通指针是不允许这些隐式转换的。因此我们需要把它们禁用掉。注意一下通用构造函数的实现方式便可以解决这个问题：

```cpp
template<typename T>
class SmartPtr{
public:
    template<typename U>
    SmartPtr(const SmartPtr<U>& other): ptr(other.get()){};
    T* get() const{ return ptr; }
private:
    T *ptr;
};
```

在`ptr(other.get())`时编译器会进行类型的兼容性检查，只有当U可以隐式转换为T时，`SmartPtr<U>`才可以隐式转换为`SmartPtr<T>`。 这样就避免了不兼容指针的隐式转换。

### 其他使用方式

除了隐式类型转换，成员函数模板还有别的用途，例如赋值运算符。。下面是shared_ptr的部分源码：

```cpp
template<class T> 
class shared_ptr{
public:
    template<class Y>
        explicit shared_ptr(Y *p);
    template<class Y>
        shared_ptr<shared_ptr<Y> const& r>;
    template<class Y>
        shared_ptr& operator=(shared_ptr<Y> const& r);
};
```

可以看到普通指针Y*到`shared_ptr<Y>`声明了explicit，需要显式的类型转换；而`shared_ptr<Y>`之间只需要隐式转换。 使用拷贝构造函数模板存在一个问题：编译器是会生成默认的拷贝构造函数？还是会从你的模板实例化一个拷贝构造函数？ 即Y == T场景下的编译器行为。

事实上，成员函数模板不会改变C++的规则。C++规则讲：如果你没有声明拷贝构造函数，那么编译器应该生成一个。 所以Y == T时拷贝构造函数不会从成员函数模板实例化，而是会自己生成一个。

所以shared_ptr模板中还是手动声明了拷贝构造函数：

```cpp
template<class T>
class shared_ptr{
public:
    shared_ptr(shared_ptr const& r);
    template<class Y>
        shared_ptr(shared_ptr<Y> const& r);

    shared_ptr& operator=(shared_ptr const& r);
    template<class Y>
        shared_ptr& operator=(shared_ptr<Y> const& r);
};
```

## Item 46: Define non-member functions inside templates when type conversions are desired

Item 24中提到，如果所有参数都需要隐式类型转换，该函数应当声明为非成员函数。 Item 24是以Rational和operator*为例子展开的，本文把这个观点推广到类模板和函数模板。 但是在类模板中，需要所有参数隐式转换的函数应当声明为友元并定义在类模板中。

### 模板化的Rational

既然是Item 24的推广，那么我们先把Item24中的Rational和operator*模板化。得到如下代码：

```cpp
template<typename T>
class Rational {
public:
  Rational(const T& numerator = 0, const T& denominator = 1);
  const T numerator() const;           
  const T denominator() const;        
};

template<typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs){}
```

### 模板参数推导出错

上述代码是Item24直接模板化的结果，看起来很完美但它是有问题的。比如我们有如下的调用：

```cpp
Rational<int> oneHalf(1, 2);            // OK
Rational<int> result = oneHalf * 2;     // Error!
```

为什么第二条会出错呢？因为编译器无法推导出合适的模板参数来实例化`Rational<T>`。 模板参数的推导包括两部分：

根据onHalf，它的类型是`Rational<int>`，很容易知道接受oneHalf的`operator*<T>`中模板参数T应该是int；
根据2的模板参数推导却不那么顺利，编译器不知道如何将实例化`operator*<T>`才能使得它接受一个int类型的2。
可能你会希望编译器将2的类型推导为`Rational<int>`，再进行隐式转换。但在编译器中模板推导和函数调用是两个过程： 隐式类型转换发生在函数调用时，而在函数调用之前编译器需要实例化一个函数。而在模板实例化的过程中，编译器无从推导T的类型。

### 声明为友元函数

为了让编译器知道T是什么，我们可以在类模板中通过friend声明来引用一个外部函数。

```cpp
template<typename T>
class Rational {
public:
    friend const Rational operator*(const Rational& lhs, const Rational& rhs);
};

template<typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs){}
```

在`Rational<T>`中声明的friend没有添加模板参数T，这是一个简便写法，它完全等价于：`friend const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs);`。

因为类模板实例化后，T总是已知的，因而那个friend函数的签名会被Rational模板类声明。 这样，`result = oneHalf * 2`便可以编译通过了，但链接会出错。 虽然在类中声明了friend operator*，然而编译器却不会实例化该声明对应的函数。 因为函数是我们自己声明的，那么编译器认为我们有义务自己去定义那个函数。

### 在类中给出定义

那我们就在声明operator*时直接给出定义：

```cpp
template<typename T>
class Rational {
public:
    friend const Rational operator*(const Rational& lhs, const Rational& rhs){
        return Rational(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
    }
};
```

这样混合模式的调用result = oneHalf * 2终于可以编译、链接并且运行了。到这里想必问题已经很清楚了：

为了对所有参数支持隐式类型转换，operator*需要声明为非成员函数；
为了让编译器推导出模板参数，operator*需要在类中声明；
在类中声明非成员函数的唯一办法便是声明为friend；
声明的函数的同时我们有义务给出函数定义，所以在函数定义也应当放在friend声明中。
调用辅助函数
虽然`operator*`可以成功运行了，但定义在类定义中的函数是inline函数，见Item 30。 如果`operator*`函数体变得很大，那么inline函数就不再合适了，这时我们可以让operator*调用外部的一个辅助函数：

```cpp
template<typename T> class Rational;
template<typename T>
const Rational<T> doMultiply(const Rational<T>& lhs, const Rational<T>& rhs);

template<typename T>
class Rational{
public:
    friend Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs){
        return doMultiply(lhs, rhs);
    }
};
```

doMultiply仍然是不支持混合模式调用的，然而doMultiply只会被operator*调用。 operator*将会完成混合模式的兼容，然后用统一的`Rational<T>`类型参数来调用doMultiply。

## Item 47: Use traits classes for information about types

C++中的 Traits 类可以在编译期提供类型信息，它是用Traits模板及其特化来实现的。 通过方法的重载，可以在编译期对类型进行"if…else"判断。我们通过STL中的一个例子来介绍Traits的实现和使用

本文以 `iterator_traits` 为例介绍了如何实现traits类，以及如何使用traits类 （在Item 42中提到过iterator_traits）。 其实 C++ 标准库中还提供了很多其他的 traits，比如 char_traits, numeric_limits 等。

STL提供了很多的容器、迭代器和算法，其中的 advance 便是一个通用的算法，可以让一个迭代器移动n步：

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d);     // 如果d小于0，就逆向移动
```

### STL迭代器回顾

- 最简单的迭代器是输入迭代器（input iterator）和输出迭代器（output iterator）， 它们只能向前移动，可以读取/写入它的当前位置，但只能读写一次。比如 ostream_iterator 就是一个输出迭代器。

- 比它们稍强的是前向迭代器（forward iterator），可以多次读写它的当前位置。 单向链表（slist，STL并未提供）和TR1哈希容器的迭代器就属于前向迭代器。

- 双向迭代器（bidirectional iterator）支持前后移动，支持它的容器包括 set, multiset, map, multimap。

- 随机访问迭代器（random access iterator）是最强的一类迭代器，可以支持 +=, -= 等移动操作，支持它的容器包括 vector, deque, string 等。

### Tag 结构体

对于上述五种迭代器，C++ 提供了五种 Tag 来标识迭代器的类型，它们之间是“is-a”的关系：

```cpp
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iterator_tag: public forward_iterator_tag {};
struct random_access_iterator_tag: public bidirectional_iterator_tag {};
```

现在回到 advance 的问题，它的实现方式显然取决于 Iter 的类型：

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d){
  if (iter is a random access iterator) {
    iter += d;                                      // use iterator arithmetic
  }                                                  // for random access iters
  else {
    if (d >= 0) { while (d--) ++iter; }              // use iterative calls to
    else { while (d++) --iter; }                     // ++ or -- for other
  }                                                  // iterator categories
}
```

怎么得到 Iter 的类型呢？这正是 traits 的作用。

### Traits

`Traits` 不是关键字，也不是 std 类型或模板，它只是 C++ 中的一种编程惯例，允许我们在编译期得到类型的信息。 用 Bjarne 的话讲，Traits 是一个用来携带信息的很小的对象（或结构）， 在其他对象或算法中用这一信息来确定策略或实现细节。

traits的另一个需求在于 advance 对与基本数据类型也能正常工作，比如 char*。所以traits不能借助类来实现， 于是我们把traits放到模板中。比如：

```cpp
template<typename IterT>          // template for information about
struct iterator_traits;           // iterator types
```

`iterator_traits<IterT>` 将会标识 IterT 的迭代器类别。iterator_traits 的实现包括两部分：

1. 用户定义类型的迭代器
2. 基本数据类型的指针

### 用户类型的迭代器

在用户定义的类型中，typedef 该类型支持迭代器的 Tag，例如 deque 支持随机迭代器：

```cpp
template < ... >                    // template params elided
class deque {
public:
  class iterator {
  public:
    typedef random_access_iterator_tag iterator_category;
  }:
};
```

然后在全局的 `iterator_traits` 模板中 typedef 那个用户类型中的 Tag，以提供全局和统一的类型识别。

```cpp
template<typename IterT>
struct iterator_traits {
  typedef typename IterT::iterator_category iterator_category;
};
```

### 基本数据类型的指针

上述办法对基本数据类型的指针是不起作用的，我们总不能在指针里面 typedef 一个 Tag 吧？ 其实这时只需要偏特化 iterator_traits，因为内置类型指针都是可以随机访问的：

```cpp
template<typename IterT>               // partial template specialization
struct iterator_traits<IterT*>{
  typedef random_access_iterator_tag iterator_category;
};
```

你已经看到了实现一个traits类的整个过程：

确定你希望提供的类型信息。比如你希望提供 deque 的 iterator 类型；
为那个信息起一个名字。比如 iterator_catetory；
提供一个模板以及必要的特化，来包含你希望提供的类型信息。比如 iterator_traits。
advance 的实现
我们已经用 iterator_traits 提供了迭代器的类型信息，是时候给出 advance 的实现了。

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d) {
  if (typeid(typename std::iterator_traits<IterT>::iterator_category) ==
    typeid(std::random_access_iterator_tag))
  ...
}
```

上述实现其实并不完美，至少 if 语句中的条件在编译时就已经决定，它的判断却推迟到了运行时（显然是低效的）。 在编译时作此判断，需要为不同的 iterator 提供不同的方法，然后在 advance 里调用它们。

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d) {
  doAdvance(                                              // call the version
    iter, d,                                              // of doAdvance
    typename std::iterator_traits<IterT>::iterator_category()
  );                                                     
}                                                       

// 随机访问迭代器
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::random_access_iterator_tag) {
  iter += d;
}

// 双向迭代器
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag) {
  if (d >= 0) { while (d--) ++iter; }
  else { while (d++) --iter; }
}

// 输入迭代器
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::input_iterator_tag) {
  if (d < 0 ) {
     throw std::out_of_range("Negative distance");    // see below
  }
  while (d--) ++iter;
}
```

总结一下上面代码是如何使用traits类的：

1. 创建一系列的“worker”函数，拥有不同的 traits 参数。根据 traits 参数来提供相应的实现；
2. 创建一个“master”函数来调用这些“worker”，并将traits类提供的信息传递给“worker”。

## Item 48: Be aware of template metaprogramming

模板元编程（Template Metaprogramming，TMP）就是利用模板来编写那些在编译时运行的C++程序。 模板元程序（Template Metaprogram）是由C++写成的，运行在编译器中的程序。当程序运行结束后，它的输出仍然会正常地编译。

C++并不是为模板元编程设计的，但自90年代以来，模板元编程的用处逐渐地被世人所发现。

- 模板编程提供的很多便利在面向对象编程中很难实现；
- 程序的工作时间从运行期转移到编译期，可以更早发现错误，运行时更加高效。
- 在设计模式上，可以基于不同的策略，自动组合而生成具体的设计模式实现。

### 静态类型检查

在Item 47中提到了这样一个std::advance的实现：

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d) {
  if (typeid(typename std::iterator_traits<IterT>::iterator_category) ==
    typeid(std::random_access_iterator_tag)){
      iter += d;
  }
  ...
}

list<int>::iterator it;
advance(it, 10);
```

其实上述代码是不能编译的，设想以下`advance<list<int>::iterator, int>`中的这条语句：

```cpp
iter += d;
```

`list<int>::iterator`是双向迭代器，不支持`+=`运算。虽然上述语句不会执行，但编译器不知道这一点。 编译时这条语句仍然会抛出类型错误。

### 模板元编程

TMP后来被证明是图灵完全的，这意味着TMP可以用来计算任何可计算的问题。你可以声明变量、执行循环、编写和调用函数等等。 但它的使用风格和普通C++完全不同。

我们来看看TMP中如何执行一个循环：

```cpp
#include <iostream>

using namespace std;

template<unsigned n>
struct Factorial{
    enum{ value = n * Factorial<n-1>::value };
};
template<>
struct Factorial<0>{
    enum{ value = 1 };
};

int main(){
    cout << Factorial<5>::value << endl;
    cout << Factorial<10>::value << endl;
    return 0;
}

```

这是一个典型的TMP例子，其低位就像是普通编程语言中的"hello world"一样。

### TMP的用途

为了更好地理解TMP的重要性，我们来看看TMP能干什么：

1. 确保量纲正确。在科学计算中，量纲的结合要始终保持正确。比如一定要单位为"m"的变量和单位为"s"的变量相除才能得到一个速度变量（其单位为"m/s"）。 使用TMP时，编译器可以保证这一点。因为不同的量纲在TMP中会被映射为不同的类型。
2. 优化矩阵运算。比如矩阵连乘问题，TMP中有一项表达式模板（expression template）的技术，可以在编译期去除临时变量和合并循环。 可以做到更好的运行时效率。
3. 自定义设计模式的实现。设计模式往往有多种实现方式，而一项叫基于策略设计（policy-based design）的TMP技术可以帮你创建独立的设计策略（design choices），而这些设计策略可以以任意方式组合。生成无数的设计模式实现方式。
