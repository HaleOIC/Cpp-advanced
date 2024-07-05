# RESOURCE MANAGEMENT

C++中最常用的资源就是动态分配内存，但是内存只是你必须管理的众多资源的一种， 其他常见的资源还包括

- 文件描述符`file descriptors`
- 互斥锁`mutex locks`
- 图形界面中的字形和笔刷
- 数据库的连接
- 网络的`socket`

无论哪种资源，当不再使用时，必须将它还给系统

## Item13 Use objects to manage resources

把资源放进对象内，我们便可依赖`c++`的析构函数'自动调用机制'确保资源的释放，即许多资源被动态分配与`heap`内而后被用于单一区块或函数内，它们应该在控制流离开那个区块或函数时被释放，其包含两个关键思想:

- 获得资源后立刻放进管理对象`managing object`中,这样的观念称为`Resource Acquisition is Initialization; RAII`.
- 管理对象运用析构函数确保资源被释放,即一旦对象被销毁，其析构函数自然会被调用，如果析构函数调用发生了异常，参见`Item 8`
  
两个常用的`RAII classes`分别是`shared_ptr`和`auto_ptr`,但是可知的是`auto_ptr`已经被弃用，取而代之的是`unique_ptr`和`weak_ptr`，而`shared_ptr`更符合直觉和使用习惯，推荐使用其进行资源管理

## Item 14 Think carefully about copying behavior in resource-managing classes

如果资源只有一份的话，像`auto_ptr`和`shared_ptr`不适合作为资源的掌管者`(resource handlers)`，有时候需要自己创建资源管理类。那么将引出这样的问题：如果资源被复制了，你将作何选择，大概有两种解决方案：

- **禁止复制**：如果复制动作对`RAII class`并不合理，便应该禁止之，将`copying`操作设置为`private`或者`delete`
- **对底层资源使用`reference-count`**：有时候我们希望保有现有资源，直到它的最后一个资源使用者被销毁。

并且`shared_ptr`和`unique_ptr`提供了自定义的销毁器结构，利于对于资源自定义释放。当然除了上述两种常见的解决方案，还有两种情况可以考虑：

- **复制底层资源**：如果在这样的情况下，复制资源管理对象，同时也应该复制其所包覆的资源，即考虑进行资源的完全复制（尤其是指针指向的地址内容的复制）深拷贝`(deep copying)`
- **转移底层资源的所有权**:如果想要资源对象只有一个，需要将资源对象的所有权进行转换，建议使用右值引用`std::move()`配合着`unique_ptr`使用

## Item 15 Provide access to raw resouces in resoucre-managing classes

大多数情况下，需要对资源管理类的原始对象进行访问和处理，这时有两种方案可以考虑完成

- 显式转换：提供一个`get()`接口，将管理的对象的指针进行返回
- 隐式转换：重载`operator*`或者`operator Obj()`

但是一般情况下，支持显式转换，因为隐式转换太容易引发如下错误了：

```cpp
class Font {
public: 
    ...
    operator FontHandle() const{
        return f
    }
    ...
private:
    FontHandle f;
}

Font f1(getFont());
...
FontHandle f2 = f1;         // 原本为考虑拷贝一个Font对象，结果自动拷贝了一个FontHandle对象
```

## Item 16 Use the same form in corresponding uses of new and delete

当使用`new`动态生成一个对象时，有两件事发生：

- 内存被`malloc`分配过来
- 针对该内存会有一个或者多个构造函数进行调用

同理当`delete`一个对象时，也有两件事情发生：

- 针对该内存有一个或者多个析构函数进行调用
- 内存会被`free`清除掉

而如果你想要完成这样的内存分配`std::string* stringPtr2 = new std::string[100]`，那么释放资源的时候仅用一个`delete`将会造成明显的内存泄露，需要使用`delete[]`，即释放一组内存，这样的话有如下的规则：

**如果调用`new`时使用`[]`，则必须在对应调用`delete`时同样用`[]`**, 反之亦然

并且尽量不要使用`typedef`来给数组形式取别名，这样在`new`过程中可能造成混乱，尽可能使用`C++`标准库中的含有`vector`和`string`等`templates`,对数组的需求可以近乎降至零。

## Item 17 Store newed objects in smart pointers in standalone statements

考虑这样的一个语句 `processWidget(std::shared_ptr<Widget>(new Widget), priority())`，编译器执行顺序可能是随机优化的，如果完成了`new`的内存分配，此时`priority()`中抛出了异常，那么这片内存将永远无法回收，造成了内存泄漏，更好的写法如下：

```cpp
std::shared_ptr<Widget> pw(new Widget);
processWidget(pw, priority());
```

即使用独立语句将`newed`对象存储到智能指针中，这样可以极大程度保证如果异常发生，则会自动将资源释放。

相关代码有

- `./codes/Part III/Sharedptr.cpp`：实现了一个简易版的`shared_ptr`，具体用到了`Item 14/ 15`
- `./codes/Part III/sum.cpp`:完成了一个资源管理和释放，使用了`Shareprt`类，具体展示了`Item 13/ 16/ 17`
