### 1.2 内存管理/引用计数

- 以`alloc`、`new`、`copy`、`mutableCopy`开头（驼峰命名法）的函数返回的对象归调用者持有（retainCount增加1），函数内不持有（所以`[[NSObject alloc] init]`，调用`alloc`时就已经增加了引用计数）

- 不以上述四种开头函数返回的对象，调用者并不持有（retainCount不增加），（MRC情况下）一般这种函数内调用`autorelease`方法放到自动释放池中（这里相当于自动释放池也持有了对象）。（ARC中没有调用`autorelease`方法，但对象也会自动注册到自动释放池，销毁的只是函数内的变量）在手动管理内存中，可以调用`retain`方法来持有返回的对象

  ```objective-c
  // MRC中，返回给调用者持有 给调用者但是调用者不持有 两种方法书写方式
  - (id)allocObject {
      id obj = [[NSObject alloc] init];
      return obj;
  }
  
  - (id)object {
      id obj = [[NSObject alloc] init];
      [obj autorelease];
      return obj;
  }
  // ARC中，两种方法书写方式相同，都是allocObject那种，区别是编译器是否自动加autorelease
  ```

- 在手动管理内存中，自己不持有的对象不要释放（即不调用`release`方法）

- `autorelease`方法实际上把对象加到了对象所在层次的自动释放池的一个数组中，自动释放池调用`drain`时这些对象全部调一遍`release`方法

### 1.3 ARC规则

- 所有权修饰符`__strong`，`__weak`，`__unsafe_unretained`，`__autoreleasing`，其中`__strong`，`__weak`，`__autoreleasing`修饰的变量可以自动初始化为`nil`

- 修饰传统静态数组（[]声明的）时数组内的各对象内容也会是`nil`（数组本身并不是`nil`）

- 修饰传统动态数组`id __strong *obj`、`NSObject * __strong *obj`不保证指针变量`obj`可以自动初始化为`nil`

- 一般不写默认是`__strong`修饰符，是持有、强引用含义，变量超出作用域时变量销毁，强引用失效，自动释放对象（相当于调一次`release`）

  ```objective-c
  id obj = [[NSObject alloc] init];
  // 等于
  id __strong obj = [[NSObject alloc] init];
  ```

- `__weak`修饰符的变量，不持有、弱引用，如果被赋给一个归调用者持有的对象会报警告，如`id __weak obj = [[NSObject alloc] init]; //报警告`。变量出作用域时不会进行释放操作，变量指向的对象被销毁时变量自动置`nil`

- `__unsafe_unretained`修饰符的变量，不持有、不强也不弱引用。ARC不管理这个修饰符修饰的变量，现在基本不用该修饰符

- `__autoreleasing`修饰的变量如果在`@autoreleasepool`块内，就是自动注册到了自动释放池

  ```objective-c
  @autoreleasepool {
      id /* __strong */ obj = [NSArray array];
      // obj持有对象，array方法内部对象也注册到了自动释放池，持有+2
      // 退出autoreleasepool块时obj销毁，持有-1，自动释放池注册的对象释放，持有-1，对象就销毁
  }
  ```

- 显示指定`__autoreleasing`修饰符的变量必须为自动变量（局部变量、函数及方法参数）

- 访问`__weak`修饰的变量引用的对象时，实际上是通过一个新的带有`__autoreleasing`修饰的临时变量访问到对象（每次访问变量都将引用到的对象注册到释放池一次，即使变量是同一个），因为需要保证在访问过程中对象一直存在，即

  ```objective-c
  id __weak obj1 = obj0; // obj0是__strong变量
  NSLog(@"class=%@", [obj1 class]);
  // 等于
  id __weak obj1 = obj0;
  id __autoreleasing tmp = obj1;
  NSLog(@"class=%@", [tmp class]);
  ```

- id的指针或对象的指针中间其实默认是`__autoreleasing`修饰符，即`id *obj;`实际上为`id __autoreleasing *obj;`，`NSObject **obj;`实际上为`NSObject * __autoreleasing *obj;`，不论是声明还是形参

  ```objective-c
  // 比如方法
  - (BOOL)performOperationWithError:(NSError **)error {
      // 发生错误
      *error = [[NSError alloc] initWithDomain:MyAppDomain code:errorCode userInfo:nil];
      return NO;
  }
  // 理解为 - (BOOL)performOperationWithError:(NSError * __autoreleasing *)error;
  // 调用
  NSError *error = nil;
  [obj performOperationWithError:&error];
  // 函数内的error对象自动注册到自动释放池，调用者不需要管
  // 此处调用不需要所有权修饰符一致的原因：编译器自动转化了，传进去的是 NSError __autoreleasing *tmpError = error;
  ```

- 所有权修饰符一致：

  ```objective-c
  NSError *error = nil;
  NSError * __strong *pError = &error;
  
  NSError __weak *error = nil;
  NSError * __weak *pError = &error;
  
  NSError __unsafe_unretained *error = nil;
  NSError * __unsafe_unretained *pError = &error;
  ```

- ARC有效时，以`init`开头（驼峰命名法）的方法也归调用者持有，不注册到自动释放池。`init`开头的方法必须为实例方法，必须返回对象，返回的对象必须是`id`、本类、本类的父类、本类的子类四种类型中的一个。比如返回`void`、方法名为`initialize`等情况不适用该规则

- ARC有效时，`dealloc`内只做废弃对象时必须的处理，如删除注册的代理、观察者对象等。方法内不能调`[super dealloc]`，且废弃对象不能手动调`[obj dealloc]`，编译器都会自己调

- ARC有效时，对象型变量不能作为C结构体成员，因为编译器不知道结构体成员的初始化和废弃时间（因为C语言规约没有规定），而ARC是要求编译器来管理对象生存周期的

  ```objective-c
  // 下面写法不能编译通过
  struct Data {
      NSMutableArray *arr;
  }
  
  // 下面写法可以编译通过，要注意内存管理
  struct Data {
      NSMutableArray __unsafe_unretained *arr;
  }
  ```

- ARC有效时，`id`类型与`void *`类型互相转换需要`__bridge`转换

  ```objective-c
  id obj = [[NSObject alloc] init];
  void *p = (__bridge void *)obj;
  id p = (__bridge id)p;
  ```

- `__bridge`相关（无缝桥接）

  - `__bridge`可以实现**Objective-C与C语言变量** 和 **Objective-C与Core Foundation对象**之间的互相转换，**不会改变对象的持有状况**，OC的对象仍然ARC管，引用计数不变，C的数据结构C管，手动释放。但是如果C对象转到OC对象，OC对象使用强引用接收，该对象的OC部分引用计数会+1
  - `__bridge_retained`用于**将OC变量转换为C语言变量** 或 **将OC对象转换为Core Foundation对象**，OC部分引用计数不变，C部分引用计数多1，ARC仍然管理该对象在OC部分的引用计数，在C部分的“引用计数”手动管
  - `__bridge_transfer`用于**将C语言变量转换为OC变量** 或 **将Core Foundation对象转换为OC对象**，OC部分引用计数多1，C部分引用计数少1，ARC仍然管理该对象在OC部分的引用计数，在C部分的“引用计数”手动管

- 传统静态数组使用：正常用，没区别

  - 声明：`id objs[10]`
  - 使用：`array[0] = [[NSObject alloc] init];...`
  - 数组内对象自动释放

- 传统动态数组使用：

  - 声明时用指针且手动初始化nil：`id __strong *array = nil;`、`NSObject * __strong *array = nil;`。
  - 分配entries个内存块且初始化为0：`array = (id __strong *)calloc(entries, sizeof(id));`或malloc函数分配+memset置0
  - 使用：`array[0] = [[NSObject alloc] init];...`
  - 手动释放所有元素与数组本身：`for(NSUInteger i = 0; i < entries; i++) { array[i] = nil; } free(array);`
  - 注意：禁止使用memcpy和realloc，可能造成对象重复废弃等问题


### 2.2 Blocks模式

- block声明：^ 返回值类型 (参数列表) {表达式}，其中返回值类型、参数列表可省略

  - 返回值类型一定可省略，省略时根据表达式块内`return`表达式返回的类型自动确定返回值类型，没有`return`时返回值类型为`void`，多个`return`时`return`返回的类型必须<font color="red">**相同**</font>。不省略时返回值类型需要与表达式块内`return`表达式返回的类型<font color="red">**相容**</font>

  - 参数列表在表达式块内不需要时可以省略

    ```objective-c
    ^void (void) { printf("Blocks\n"); }
    // 可省略为
    ^{ printf("Blocks\n"); }
    ```

- block作为函数参数与返回值

  ```objective-c
  // ①
  void func(int (^blk)(int)) {
      ...
  }
  
  // ②
  int (^func()(int)) {
  // block类型包在函数名与函数形参列表两侧，函数返回值位置写block类型返回值，
  // 函数名前写插入记号^，函数参数列表后写block类型的小括号包起的参数列表，
  // 最后用小括号从插入记号前包到block类型的参数列表后
      return ^(int count) { return count + 1; };
  }
  
  typedef int (^blk_t)(int);
  // typedef后简化为
  // ①
  void func(blk_t blk) {
      ...
  }
  
  // ②
  blk_t func() {
      return ^(int count) { return count + 1; };
  }
  ```

- block可以直接用也可以赋给指针来用

  ```objective-c
  typedef int (^blk_t)(int);
  blk_t blk = ^(int count) { return count + 1; };
  blk(10);        // 直接用block
  blk_t *blkptr = &blk;
  (*blkptr)(10);  // 指针用block
  ```

- block捕获**自动变量**（局部变量）的注意事项

  - block声明时直接捕获其内部所需变量的瞬时值

    ```objective-c
    int val = 0;
    void (^blk)(void) = ^{ printf("val = %d", val);};
    val = 1;
    blk();
    // 打印 val = 0
    ```

  - block内修改所捕获的变量时要加`__block`修饰符，或者自动变量改为静态/全局变量

    ```objective-c
    __block int val = 0;
    void (^blk)(void) = ^{ val = 1; printf("val = %d", val); };
    blk();
    // 编译通过，打印 val = 1
    ```

  - block调用OC变量的方法不会编译错误，编译错误只会发生在对没加`__block`修饰符的变量进行赋值（修改）的情况

    ```objective-c
    NSMutableArray array = [[NSMutableArray alloc] init];
    void (^blk)(void) = ^{
        id obj = [[NSObject alloc] init];
        [array addObject:obj];
    };
    // 编译通过
    
    NSMutableArray array = [[NSMutableArray alloc] init];
    void (^blk)(void) = ^{
        array = [[NSMutableArray alloc] init];
    };
    // 编译失败
    
    __block NSMutableArray array = [[NSMutableArray alloc] init];
    void (^blk)(void) = ^{
        array = [[NSMutableArray alloc] init];
    };
    // 编译通过
    ```

  - block没法对C语言数组进行捕获，可使用指针解决

    ```objective-c
    const char text[] = "hello";
    void (^blk)(void) = ^{
        printf("%c\n", text[2]);
    };
    // 编译失败
    
    const char *text = "hello";
    void (^blk)(void) = ^{
        printf("%c\n", text[2]);
    };
    // 编译通过
    ```

### 2.3 Blocks的实现

#### 2.3.1 Block的实质 2.3.2 截获自动变量值     ->（基本类型变量、静态/全局变量）

- block实例的转换与解释

  ```objective-c
  // 源代码 test.m，捕获自动变量、捕获全局变量
  int global_val = 1;
  static int static_global_val = 2;
  int main() {
      static int static_val = 3;
      int dmy = 256;
      int val = 10;
      const char *fmt = "val = %d\n";
  
      int (^blk)(int) = ^(int count) {
          global_val *= 1;
          static_global_val *= 2;
          static_val *= 3;
          printf(fmt, val);
          return count;
      };
      blk(42);
  
      return 0;
  }
  ```


  ```cpp
  //--------------转换后的源代码 test.cpp----------------
  int global_val = 1;
  static int static_global_val = 2;
  
  // 通用block信息结构体
  struct __block_impl {
      void *isa;
      int Flags;     //标志
      int Reserved;  //今后版本升级所需区域
      void *FuncPtr; //函数指针
  };
  
  // 具体对应block的实现
  struct __main_block_impl_0 {
      //含有通用信息结构体指针、对应block描述信息结构体指针、捕获的自动变量
      struct __block_impl impl;
      struct __main_block_desc_0 *Desc;
      int *static_val;
      const char *fmt;
      int val;
  
      //构造函数
      __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc,
          int *_static_val, const char *_fmt, int _val, int flags = 0)
          : static_val(_static_val), fmt(_fmt), val(_val){
          impl.isa = &_NSConcreteStackBlock; //所以是一个OC的_NSConcreteStackBlock类的实例
          impl.Flags = flags;
          impl.FuncPtr = fp; //对应block内容转换成的C函数指针赋值
          //没赋值所以impl.Reserved = 0
          Desc = desc;
      }
  };
  
  // 对应block内容转换成C函数
  static int __main_block_func_0(struct __main_block_impl_0 *__cself, int count) {
  // __cself相当于C++的this，或者OC的self，指向转换后的对应block实现
      // （局部）静态变量使用指针承接
      int *static_val = __cself->static_val;
      // 局部变量按传进来的值使用
      const char *fmt = __cself->fmt;
      int val = __cself->val;
  
      // 全局变量访问方式不变
      global_val *= 1;
      static_global_val *= 2;
      (*static_val) *= 3; //（局部）静态变量使用指针访问
  
      printf(fmt, val);
      return count;
  }
  
  // 对应block描述信息结构体，声明结构体+定义初始化一个实例
  static struct __main_block_desc_0 {
      unsigned long reserved;   //今后版本升级所需区域
      unsigned long Block_size; //转换后对应block实现的大小
  } __main_block_desc_0_DATA = {
      0,
      sizeof(struct __main_block_impl_0)
  };
  
  int main() {
      static int static_val = 3;
      int dmy = 256;
      int val = 10;
      const char *fmt = "val = %d\n";
  
      //使用外面__main_block_func_0（block转换出来的函数）、__main_block_desc_0_DATA（对应block描述信息）、自动变量，
      //调用构造函数生成一个__main_block_impl_0结构体实例，赋值到int (*)(int)类型的blk
      //就是结构体的地址转成了函数指针
      int (*blk)(int) = ((int (*)(int))&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, &static_val, fmt, val, 0x22000000));
  
      //blk转为__block_impl类型找到FuncPtr，将FuncPtr指针转为int (*)(__block_impl *, int)类型的函数指针，
      //再将blk转为__block_impl类型作为实参传进去，进行函数调用。对应源代码blk(42);
      //FuncPtr使用(void *)__main_block_func_0赋值，__main_block_func_0第一形参应为__main_block_impl_0类型，
      //但是传进一个(__block_impl *)blk实参，由于C里结构体指针间可以随意转换，__main_block_impl_0的第一个成员就是__block_impl结构体，
      //所以(__block_impl *)blk是可以转为(__main_block_impl_0 *)并找到里面存的自动变量比如__cself->fmt的
      //个人推测转成__block_impl类型是因为这个是个基本结构体，每个block第一个成员就是，方便转换统一的代码
      ((int (*)(__block_impl *, int))((__block_impl *)blk)->FuncPtr)((__block_impl *)blk, 42);
  
      return 0;
  }
  ```

- 根据上述代码，block不支持修改所捕获的变量的原因为：不符合程序员所想要的实现（block内改了实际上没反映到外部变量上）

- 根据上述代码，block不能捕获数组的原因为

  ```objective-c
  char a[10] = {2};
  void (^blk)(void) = ^{ a[0] = 0; };
  
  // 假如编译通过，生成的代码为
  static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
      int a[10] = __cself->a; //C语言不允许[]数组相互赋值，因为a是数组名，代表数组地址，是个常量
      a[0] = 0;
  }
  // 所以编译不通过
  ```

#### 2.3.3 `__block`说明符    ->（`__block`基本类型变量）

```objective-c
// 源代码 test.m，捕获带__block修饰符的自动变量
int main() {
    __block int mutval = 10;
    __block int mutval2 = 2;
    void (^blk)(void) = ^{ 
        mutval = 1;
        mutval2 = 42;
    };
    blk();

    return 0;
}
```


```cpp
//--------------(带__block修饰符的自动变量)转换后的源代码 test.cpp----------------
// 通用block信息结构体
struct __block_impl {
    void *isa;
    int Flags;     //标志
    int Reserved;  //今后版本升级所需区域
    void *FuncPtr; //函数指针
};

// 具体对应__block变量信息结构体，在__main_block_impl_0外面是为了可以在不同block中使用__block变量
struct __Block_byref_mutval_0 {
    void *__isa;
    __Block_byref_mutval_0 *__forwarding; //为什么有这个在下面解释
    int __flags;
    int __size;
    int mutval;    //承载原变量值的成员
}
struct __Block_byref_mutval2_1 {
    void *__isa;
    __Block_byref_mutval2_1 *__forwarding;
    int __flags;
    int __size;
    int mutval2;
};

// 具体对应block的实现
struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    __Block_byref_mutval_0 *mutval;
    __Block_byref_mutval2_1 *mutval2;

    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, 
        __Block_byref_mutval_0 *_mutval, __Block_byref_mutval2_1 *_mutval2, int flags=0) 
        : mutval(_mutval->__forwarding), mutval2(_mutval2->__forwarding) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

// 对应block内容转换成C函数
static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
    __Block_byref_mutval_0 *mutval = __cself->mutval;
    __Block_byref_mutval2_1 *mutval2 = __cself->mutval2;

    (mutval->__forwarding->mutval) = 1;    //为什么使用__forwarding在下面解释
    (mutval2->__forwarding->mutval2) = 42;
}

static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src) {
    // BLOCK_FIELD_IS_BYREF表示对__block变量进行内存管理
    _Block_object_assign((void*)&dst->mutval, (void*)src->mutval, 8/*BLOCK_FIELD_IS_BYREF*/);
    _Block_object_assign((void*)&dst->mutval2, (void*)src->mutval2, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static void __main_block_dispose_0(struct __main_block_impl_0*src) {
    _Block_object_dispose((void*)src->mutval, 8/*BLOCK_FIELD_IS_BYREF*/);
    _Block_object_dispose((void*)src->mutval2, 8/*BLOCK_FIELD_IS_BYREF*/);
}

// 对应block描述信息结构体，声明结构体+定义初始化一个实例
static struct __main_block_desc_0 {
    unsigned long reserved;
    unsigned long Block_size;
    void (*copy)(struct __main_block_impl_0*, struct __main_block_impl_0*);
    void (*dispose)(struct __main_block_impl_0*);
} __main_block_desc_0_DATA = {
    0,
    sizeof(struct __main_block_impl_0),
    __main_block_copy_0, 
    __main_block_dispose_0
};

int main() {
    __attribute__((__blocks__(byref))) __Block_byref_mutval_0 mutval = {
        (void*)0,
        (__Block_byref_mutval_0 *)&mutval,
        0, 
        sizeof(__Block_byref_mutval_0), 
        10
    };
    __attribute__((__blocks__(byref))) __Block_byref_mutval2_1 mutval2 = {
        (void*)0,
        (__Block_byref_mutval2_1 *)&mutval2, 
        0, 
        sizeof(__Block_byref_mutval2_1), 
        2
    };

    void (*blk)(void) = ((void (*)())&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, (__Block_byref_mutval_0 *)&mutval, (__Block_byref_mutval2_1 *)&mutval2, 0x22000000));

    ((void (*)(__block_impl *))((__block_impl *)blk)->FuncPtr)((__block_impl *)blk);

    return 0;
}
```

#### 2.3.4 Block存储域

- block种类与存储位置：block在全局变量位置声明的、block中不使用（捕获）自动变量这两种情况下，block是`_NSConcreteGlobalBlock`类型；对block发送`copy`消息创建的block是`_NSConcreteMallocBlock`类型

  | block所属类              | 存储位置   | 调用`copy`方法的效果 |
  | ------------------------ | ---------- | -------------------- |
  | `_NSConcreteStackBlock`  | 栈         | 从栈复制到堆         |
  | `_NSConcreteGlobalBlock` | 程序数据区 | 无效果               |
  | `_NSConcreteMallocBlock` | 堆         | 增加引用计数         |

- ARC有效时，函数返回一个block时，（函数内在栈上创建block，且没有手动拷贝到堆上）编译器会自动返回一个拷贝到堆上的block

- ARC有效时，传一个block给函数使用时，需要看函数的实现有没有适当复制该block。Cocoa框架内带usingBlock的方法、GCD的API这两种情况不需要手动复制。其他看情况要复制到堆上给函数或者海曙内自己复制到堆上

- 注：私下测试中，block中不使用（捕获）自动变量时，还是`_NSConcreteStackBlock`（Apple clang version 13.1.6 (clang-1316.0.21.2.5)，API Objective-C 2.0）

![测试结果](pics/测试结果.png)

#### 2.3.5 `__block`变量存储域

- `__block`修饰的变量，对block发送copy消息复制到堆上时，block本身与用到的`__block`变量一块被复制到堆上。在block从栈复制到堆上时，

- 原来`__block`修饰的变量在栈上，则现在复制到堆上并被复制到堆上的block持有

- 原来`__block`修饰的变量在堆上，则现在直接被复制到堆上的block持有

- 如果`__block`修饰的变量被多个block引用，第一次随block被复制到堆上时`__block`修饰的变量发生复制并被block持有，其他block也复制到堆上时`__block`修饰的变量只改引用计数

- 堆上使用的`__block`修饰的变量声明周期与正常相同，block不使用它了，就被释放（引用计数-1），没有block使用它（引用计数=0）就被废弃

- `__block`修饰的变量转化成的结构体中

- `__forwarding`初始化时，是用自己的地址赋值（指向栈上结构体自己）

- 复制到堆上后，栈上和堆上的`__forwarding`都更新为堆上结构体地址（指向堆上的结构体）

- 所以在转化后对这种变量使用时都使用`__forwarding`来找到正确的同一个变量（大概是有堆上都能找到堆上，没堆上就找到栈上？），在非block内对`__block`修饰的变量的修改就对应修改了堆上那个结构体。

  ```objective-c
  struct __main_block_impl_0 {
      struct __block_impl impl;
      struct __main_block_desc_0* Desc;
      __Block_byref_mutval_0 *mutval;
      __Block_byref_mutval2_1 *mutval2;
  
      __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, 
          __Block_byref_mutval_0 *_mutval, __Block_byref_mutval2_1 *_mutval2, int flags=0) 
          : mutval(_mutval->__forwarding), mutval2(_mutval2->__forwarding) {
          impl.isa = &_NSConcreteStackBlock;
          impl.Flags = flags;
          impl.FuncPtr = fp;
          Desc = desc;
      }
  };
  
  static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
      __Block_byref_mutval_0 *mutval = __cself->mutval;
      __Block_byref_mutval2_1 *mutval2 = __cself->mutval2;
  
      (mutval->__forwarding->mutval) = 1;
      (mutval2->__forwarding->mutval2) = 42;
  }
  
  int main() {
      __attribute__((__blocks__(byref))) __Block_byref_mutval_0 mutval = {
          //...
          (__Block_byref_mutval_0 *)&mutval,
          //...
      };
      __attribute__((__blocks__(byref))) __Block_byref_mutval2_1 mutval2 = {
          //...
          (__Block_byref_mutval2_1 *)&mutval2, 
          //...
      };
  
      void (*blk)(void) = ((void (*)())&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, (__Block_byref_mutval_0 *)&mutval, (__Block_byref_mutval2_1 *)&mutval2, 0x22000000));
  
      ((void (*)(__block_impl *))((__block_impl *)blk)->FuncPtr)((__block_impl *)blk);
      // FuncPtr即__main_block_func_0，传入上面初始化的blk为实参。blk里用(__Block_byref_mutval_0 *)&mutval初始化__main_block_impl_0构造函数
      // 的 __Block_byref_mutval_0 *_mutval 形参，取其中的__forwarding给__Block_byref_mutval_0 *mutval成员赋值，
      // 所以是栈上的结构体的地址。所以__main_block_func_0里变量mutval = __cself->mutval指向栈的结构体。
      // block拷贝到堆之后，栈上结构体里面__forwarding会变为堆上__forwarding地址，mutval是栈上结构体变量，
      // mutval->__forwarding找到堆上结构体，mutval->__forwarding->mutval找到堆上结构体内承载原变量值的成员
  
      return 0;
  }
  ```

#### 2.3.6 截获对象 2.3.7 `__block`变量和对象    ->（对象、`__block`对象）

```objective-c
// 源代码 test.m，捕获自动变量array
typedef void (^blk_t)(id);

int main(void) {
    blk_t blk;
    __block NSObject *nsobj = [[NSObject alloc] init];
    {
        id array = [[NSMutableArray alloc] init];
        blk = [^(id obj) {
            [array addObject:obj];
            NSLog(@"array count = %ld", [array count]);
            nsobj = [[NSObject alloc] init];
        } copy];
    }

    blk([[NSObject alloc] init]);
    blk([[NSObject alloc] init]);
    blk([[NSObject alloc] init]);
    return 0;
}
//--------------转换后的源代码 test.cpp----------------
// 部分简化为OC用法
// 通用block信息结构体
struct __block_impl {
    void *isa;
    int Flags;     //标志
    int Reserved;  //今后版本升级所需区域
    void *FuncPtr; //函数指针
};

// 通用 block捕获的__block对象 的拷贝与释放函数
static void __Block_byref_id_object_copy_131(void *dst, void *src) {
    _Block_object_assign((char*)dst + 40, *(void * *) ((char*)src + 40), 131);
}

static void __Block_byref_id_object_dispose_131(void *src) {
    _Block_object_dispose(*(void * *) ((char*)src + 40), 131);
}

typedef void (*blk_t)(id);

struct __Block_byref_nsobj_0 {
    void *__isa;
    __Block_byref_nsobj_0 *__forwarding;
    int __flags;
    int __size;
    // 对象型__block变量专有拷贝与释放函数
    void (*__Block_byref_id_object_copy)(void*, void*);
    void (*__Block_byref_id_object_dispose)(void*);
    NSObject *__strong nsobj;
};

struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    // 前面说ARC下C结构体内不能有__strong的OC对象，而这里运行时库明确了复制到堆上和废弃的时机，
    // 所以这个对象可以存在。但是需要__main_block_desc_0增加copy和dispose函数来管理
    __strong id array;
    __Block_byref_nsobj_0 *nsobj; // by ref

    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, __strong id _array, __Block_byref_nsobj_0 *_nsobj, int flags=0) 
    : array(_array), nsobj(_nsobj->__forwarding) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself, __strong id obj) {
    __Block_byref_nsobj_0 *nsobj = __cself->nsobj; // bound by ref
    __strong id array = __cself->array; // bound by copy

    // 简化为OC
    [array addObject:obj];
    NSLog(@"array count = %ld", [array count]);
    (nsobj->__forwarding->nsobj) = [[NSObject alloc] init];
}

static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src) {
    // 对象赋值，相当于retain
    // BLOCK_FIELD_IS_OBJECT表示对OC对象进行内存管理
    _Block_object_assign((void*)&dst->array, (void*)src->array, 3/*BLOCK_FIELD_IS_OBJECT*/);
    // BLOCK_FIELD_IS_BYREF表示对__block变量进行内存管理
    _Block_object_assign((void*)&dst->nsobj, (void*)src->nsobj, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static void __main_block_dispose_0(struct __main_block_impl_0*src) {
    // 对象释放，相当于release
    _Block_object_dispose((void*)src->array, 3/*BLOCK_FIELD_IS_OBJECT*/);
    _Block_object_dispose((void*)src->nsobj, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static struct __main_block_desc_0 {
    size_t reserved;
    size_t Block_size;
    void (*copy)(struct __main_block_impl_0*, struct __main_block_impl_0*);
    void (*dispose)(struct __main_block_impl_0*);
} __main_block_desc_0_DATA = {
    0, 
    sizeof(struct __main_block_impl_0), 
    __main_block_copy_0, 
    __main_block_dispose_0
};

// 部分简化为OC
int main(void) {
    blk_t blk;
    __attribute__((__blocks__(byref))) __Block_byref_obj_0 obj = {
        (void*)0,
        (__Block_byref_obj_0 *)&obj,
        0x2000000, 
        sizeof(__Block_byref_obj_0), 
        __Block_byref_id_object_copy_131, 
        __Block_byref_id_object_dispose_131, 
        [[NSObject alloc] init];
    };

    {
        id array = [[NSMutableArray alloc] init];
        blk = ((void (*)(__strong id))&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, array, (__Block_byref_nsobj_0 *)&nsobj, 0x22000000));
        blk = [blk copy];
    }

    ((void (*)(__block_impl *, __strong id))((__block_impl *)blk)->FuncPtr)((__block_impl *)blk, [[NSObject alloc] init]);
    ((void (*)(__block_impl *, __strong id))((__block_impl *)blk)->FuncPtr)((__block_impl *)blk, [[NSObject alloc] init]);
    ((void (*)(__block_impl *, __strong id))((__block_impl *)blk)->FuncPtr)((__block_impl *)blk, [[NSObject alloc] init]);

    return 0;
}
```

- `__main_block_copy_0/__main_block_dispose_0`或者`__main_block_desc_0.copy/dispose`这些在block复制到堆上以及堆上block被废弃时自动调用，复制时机：
- block调`copy`方法时
- block作为函数返回值被返回时
- block赋值给某类的`__strong id`成员变量或者对应类型的成员变量时
- 作为实参传给Cocoa框架内带usingBlock的方法、GCD的API时
- 以上的后三种情况之外，在block中使用自动变量时，推荐调用`copy`方法将block从栈复制到堆上，避免使用的自动变量超作用域销毁，而废弃掉自动变量所指的对象
- 如果捕获的是`__weak`变量，结构体仍然生成，但是不管带不带`__block`，只要`__weak`引用的对象被销毁了，`__weak`变量都变成`nil`
- 如果捕获的是`__unsafe_unretained`变量，结构体仍然生成，但是不管带不带`__block`，编译器不会做多余处理（像是`__weak`变量的赋`nil`）
- `__autoreleasing`修饰符与block连用没有规定，不要用，甚至`__autoreleasing`修饰符与`__block`连用会编译失败

#### 2.3.8 block循环引用

```objective-c
typedef void (^blk_t)(void);

@interface MyObj : NSObject
{
    blk_t _blk;
}
@end

@implementation MyObj
- (id)init {
    self = [super init];
    if (self) {
        _blk = ^{ NSLog(@"self = %@", self); };
    }
    return self;
}
- (void)dealloc {
    NSLog(@"dealloc");
}
@end

int main () {
    id o = [[MyObj alloc] init];
    NSLog(@"%@", o);
    return 0;
}
```

- 上述代码块中，o持有_blk，由于块出括号就要销毁，所以块复制到堆上并且赋给_blk，同时块持有self，于是出现循环引用

- 下面也同样是循环引用，因为成员变量用的时候也是通过对象来找成员变量，也是捕获了self

  ```objective-c
  @interface MyObj : NSObject
  {
      blk_t _blk;
      id _obj;
  }
  @end
  
  @implementation MyObj
  - (id)init {
      self = [super init];
      if (self) {
          // 实际上等于 NSLog(@"_obj = %@", self->_obj);
          _blk = ^{ NSLog(@"_obj = %@", _obj); };
      }
      return self;
  }
  - (void)dealloc {
      NSLog(@"dealloc");
  }
  @end
  ```

- 解决方法如下，

- 使用`__weak`变量（此时应为对象持有的块，块中的tmp必定指向对象，所以不用判断`nil`）

  ```objective-c
  if (self) {
      // 以前没有__weak的时候，此处也可以用__unsafe_unretained
      id __weak tmp = self;
      _blk = ^{ NSLog(@"self = %@", tmp); }
  }
  ```

- 使用`__block`变量，但是需要额外调用执行

  ```objective-c
  @implementation MyObj
  - (id)init {
      self = [super init];
      if (self) {
          __block id tmp = self;
          _blk = ^{ 
              NSLog(@"self = %@", tmp);
              // 置空操作，减少tmp带来的引用计数
              tmp = nil;
          };
      }
      return self;
  }
  - (void)execBlock {
      _blk();
  }
  - (void)dealloc {
      NSLog(@"dealloc");
  }
  @end
  
  int main () {
      id o = [[MyObj alloc] init];
      NSLog(@"%@", o);
      // 必须调用execBlock方法执行一下块的代码，否则会循环引用内存泄漏（tmp没有取消对self的引用）
      [o execBlock];
      return 0;
  }
  ```

- 上述`__block`变量方法优点为：可自由控制对象持有时间，不必在不能使用`__weak`时使用`__unsafe_unretained`修饰符。缺点为：必须执行block内的代码，如果任何情况下都不执行则仍然会造成内存泄漏

#### 2.3.9 `copy`/`release`

- ARC无效时
- 对栈上block调`copy`方法来复制并持有（`retain`无用），对堆上block调用`retain`/`copy`方法来持有，对堆上block调用`release`方法来释放
- 上述`copy`可换为`Block_copy()`的C函数，`release`可换为`Block_release()`的C函数
- <font color="red">**注意**</font>：使用__block来避免循环引用：`__block`修饰的对象类型自动变量<font color="red">**不会**</font>被块retain，不被`__block`修饰的对象类型自动变量却<font color="red">**会**</font>被块retain（此时`__block`相当于ARC下的`__unsafe_unretained`）

### 3.2 GCD的API

#### 3.2.1 Dispatch Queue

- Dispatch Queue（派发队列）有两种：Serial Dispatch Queue（串行派发队列），FIFO，使用一个线程，执行完一个任务再派发下一个任务进去执行；Concurrent Dispatch Queue（并发派发队列），FIFO，使用多个线程，不用等前一个任务执行完就可以派发下一个任务去执行，但是同时执行的任务数量取决于当前系统状态
- 派发队列获取方式有两种，[3.2.2](#322-dispatch_queue_create)直接创建和[3.2.3](#323-main-dispatch-queueglobal-dispatch-queue)获取系统提供的

#### 3.2.2 `dispatch_queue_create`

- `dispatch_queue_create`生成的派发队列个数是无上限的

- 生成串行派发队列时注意，一个队列对应使用一个线程（所以不同任务指派到不同串行派发队列上实际会形成并行效果），但如果生成太多队列会消耗大量内存，引起大量上下文切换，降低系统响应

- 所以应在解决多线程更新同一个资源导致数据竞争时使用串行派发队列（不同线程对同一个资源的更新操作放在同一个串行派发队列中）

- 生成并发派发队列时，虽然也可以生成无限多个，但是系统内核只使用有效管理的线程，不会发生串行派发队列的问题

- `dispatch_queue_create`创建队列，参数一为队列名，可以为NULL，但是建议按照逆序域名方式命名（如com.example.gcd.mySerialQueue），因为该名称会出现在调试器中以及崩溃日志中表示为相应队列名称；参数二为队列特征，一般只有下面三种填法

  ```objective-c
  // 创建串行派发队列
  dispatch_queue_t serialQueue = dispatch_queue_create("mySerialQueue", NULL);
  dispatch_queue_t serialQueue = dispatch_queue_create("mySerialQueue", DISPATCH_QUEUE_SERIAL);
  
  // 创建并发派发队列
  dispatch_queue_t concurrentQueue = dispatch_queue_create("myConcurrentQueue", DISPATCH_QUEUE_CONCURRENT);
  ```

- 非ARC下，需要通过`dispatch_release()`释放，通过`dispatch_retain()`持有

- 即使生成的队列在 block通过`dispatch_async()`派发到这个队列上 后立刻`dispatch_release()`释放，也不会有问题，因为block会`dispatch_retain()`来持有这个队列，block执行结束后`dispatch_release()`释放队列，队列才被废弃

#### 3.2.3 Main Dispatch Queue/Global Dispatch Queue

- Main Dispatch Queue（主队列）将任务派发到主线程，属于串行派发队列

- Global Dispatch Queue（全局并发队列）将任务派发到系统创建的多个其他线程，属于并发派发队列，但是并不保证执行任务的实时性

- 全局派发队列优先级有四种

  ```objective-c
  // 获取主队列
  dispatch_queue_t mainDispatchQueue = dispatch_get_main_queue();
  
  // 获取全局并发队列：高优先级、默认优先级、低优先级、后台优先级
  dispatch_queue_t globalDispatchQueueHigh = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
  dispatch_queue_t globalDispatchQueueDefault = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
  dispatch_queue_t globalDispatchQueueLow = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0);
  dispatch_queue_t globalDispatchQueueBackground = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0);
  ```

- 非ARC中，对主队列和全局并发队列执行`dispatch_release()`和`dispatch_retain()`无效，也不会产生影响

#### 3.2.4 `dispatch_set_target_queue`

- 用处1：变更所创建队列的优先级

  ```objective-c
  // 创建的串行或并发派发队列优先级与全局并发队列的默认优先级相同
  dispatch_queue_t serialQueue = dispatch_queue_create("mySerialQueue", NULL);
  dispatch_queue_t globalDispatchQueueBackground = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0);
  
  // 将参数一的派发队列优先级设定为参数二的全局并发队列的优先级（参数一不可传入主队列或者系统提供的全局并发队列）
  dispatch_set_target_queue(serialQueue, globalDispatchQueueBackground);
  ```

- 用处2：作成执行阶层

  ```objective-c
  dispatch_queue_t serialQueue = dispatch_queue_create("mySerialQueue", NULL);
  dispatch_queue_t myQueue1 = dispatch_queue_create("myQueue1", NULL);
  dispatch_queue_t myQueue2 = dispatch_queue_create("myQueue2", NULL);
  dispatch_queue_t myQueue3 = dispatch_queue_create("myQueue3", NULL);
  
  dispatch_set_target_queue(myQueue1, serialQueue);
  dispatch_set_target_queue(myQueue2, serialQueue);
  dispatch_set_target_queue(myQueue3, serialQueue);
  
  dispatch_async(myQueue1, ^{ NSLog(@"Queue1"); });
  dispatch_async(myQueue2, ^{ NSLog(@"Queue2"); });
  dispatch_async(myQueue3, ^{ NSLog(@"Queue3"); });
  ```

- 那么上述本来并行执行的myQueue1、myQueue2、myQueue3，在目标队列serialQueue（串行队列）上只能同步执行（如果serialQueue是个并发队列则并不会产生同步执行的效果），执行顺序按照哪个myQueue先被加入任务块，哪个myQueue先执行，执行完了才执行下一个queue内安排的任务（即使任务、`dispatch_set_target_queue`分布在不同的方法中）

- 如果都是并发queue加入目标串行队列，那么所有任务块严格按照加入先后串行执行，就像是任务块直接按顺序添加入目标串行队列

- 如果都串行和并发两种queue加入目标串行队列，那么第一个任务块加入哪个queue，哪个queue先执行，但是在目标串行队列上，并发queue的任务块执行顺序可以被串行queue打断，串行queue的任务块执行顺序无法被并发queue打断（某个串行queue所有任务整个插入并发queue的执行顺序中，插队）

- 但是目标队列毕竟也是队列，也可以往里添加任务，如果是myQueue（串行或并发queue）和目标serialQueue都安排了任务块，那么在任务块执行顺序表现上，第一个任务块加入哪个queue，哪个queue先执行，serialQueue表现与上述并发queue的表现相同

#### 3.2.5 `dispatch_after`

- 在指定时间将任务块加入队列

  ```objective-c
  // 生成参数一时间的参数二纳秒数后的时间
  dispatch_time_t time = dispatch_time(DISPATCH_TIME_NOW, 3ull * NSEC_PER_SEC);
  // 参数一时间时用dispatch_async加入任务块
  dispatch_after(time, dispatch_get_main_queue(), ^{ NSLog(@"3 seconds."); });
  ```

- `dispatch_time()`常用于生成相对时间，`dispatch_time_t dispatch_walltime(const struct timespec *_Nullable when, int64_t delta);`常用于生成绝对时间（参数一是`timespec`时间格式结构体）

#### 3.2.6 Dispatch Group

- 等待派到不同队列（或者并发队列）的一组block执行完之后做些事情时使用

  ```objective-c
  dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
  // 创建dispatch group
  dispatch_group_t group = dispatch_group_create();
  
  // 添加到dispatch group及dispatch queue
  dispatch_group_async(group, queue, ^{ NSLog(@"blk0"); });
  dispatch_group_async(group, queue, ^{ NSLog(@"blk1"); });
  dispatch_group_async(group, queue, ^{ NSLog(@"blk2"); });
  
  // group内所有任务做完后执行
  dispatch_group_notify(group, dispatch_get_main_queue(), ^{ NSLog(@"Done."); });
  ```

- 如果仅仅想等待一组做完而不想再执行什么，`dispatch_group_notify`可换为

  ```objective-c
  // 仅等待，但调用这个函数的线程会停止，一直在该函数中不会返回，直到到达等待的时间点。
  // 参数二为等待到什么时候，DISPATCH_TIME_FOREVER表示永久等待且中途无法取消，DISPATCH_TIME_NOW表示立即返回，
  // 或者类似3.2.5中生成一个时间点赋值在这
  // 可根据返回值进行分支判断，时间内一组任务成功全部做完则返回0，否则返回非0
  dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
  ```

#### 3.2.7 `dispatch_barrier_async`

- **并发队列**中加一个与其他操作互斥的操作（比如写入）时使用

  ```objective-c
  dispatch_queue_t queue = dispatch_queue_create("com.example.gcd", DISPATCH_QUEUE_CONCURRENT);
  dispatch_async(queue, blk0_reading);
  dispatch_async(queue, blk1_reading);
  dispatch_async(queue, blk2_reading);
  dispatch_barrier_async(queue, blk_writing);// 此操作会前面操作并发后单独执行，执行完了后面操作再并发
  dispatch_async(queue, blk3_reading);
  dispatch_async(queue, blk4_reading);
  dispatch_async(queue, blk5_reading);
  dispatch_async(queue, blk6_reading);
  ```

#### 3.2.8 `dispatch_sync`

- 用法简单，但是容易产生死锁

  ```objective-c
  // 主线程中死锁：
  dispatch_queue_t queue = dispatch_get_main_queue();
  dispatch_sync(queue, ^{ NSLog(@"Hello"); });
  // 不仅是主线程，只要是dispatch_sync回调用该函数的线程都死锁，因为函数要等待块执行结束才能退出，
  // 而函数要执行完了才能给队列添加块（块要等待函数执行结束才能添加），所以死锁
  ```

#### 3.2.9 `dispatch_apply`

- `dispatch_sync`执行完了才退出的特点和dispatch group执行一组任务特点相结合的API

  ```objective-c
  dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
  // 参数一执行块的次数，参数二追加到的队列，参数三执行的block，block是带参数的，从0开始每次进来加一
  // 比如可以用于对数组中各个对象执行某个块
  dispatch_apply(10, queue, ^(size_t index) {
      NSLog(@"%zu", index);
  });
  // 上面函数添加的块都执行完了才退出，才能执行下面的代码
  NSLog(@"done");
  ```

#### 3.2.10 `dispatch_suspend`/`dispatch_resume`

- 对队列进行挂起、继续

  ```objective-c
  // 挂起队列，已经执行完的就执行完了，没执行的就暂停执行
  dispatch_suspend(queue);
  // 挂起的队列恢复执行
  dispatch_resume(queue);
  ```

#### 3.2.11 Dispatch Semaphore

- 信号量，用于比同步派发队列和`dispatch_barrier_async`更细粒度进行排他性处理的场景

- 等待的方式使用，等待信号量大于等于1时可安全操作

  ```objective-c
  // 形参表示给信号量初始化的值
  dispatch_semaphore_t semaphore = dispatch_semaphore_create(1);
  // 等待信号量变成大于等于1，并且给信号量减1，然后返回。此时进入排他性处理：我（们）开始处理了，下一个等等再来
  // 如果是forever，则等待到大于等于1时返回，返回值0
  // 如果是某特定时间点，则等待到大于等于1时返回值0，没等待到大于等于1时返回值非0
  dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
  
  // 上述函数返回值0时安全处理（并不一定信号量为0）
  ...
  
  // 处理完了给信号量用下列函数加一，表示已经处理完了，下一个可以不用等了
  dispatch_semaphore_signal(semaphore);
  ```

#### 3.2.12 `dispatch_once`

  - 绝对只执行一次的代码，多线程可保证百分百安全，单例模式

  ```objective-c
static dispatch_once_t pred;
dispatch_once(&pred, ^{
    // ...只执行一次的代码
});
  ```

#### 3.2.13 Dispatch I/O

- 大文件分割成多个块使用多个线程并列读取，提升文件读取速度

  ```objective-c
  // 创建一个dispatch io通道类型变量，参数一是io通道属性，实参二选一：DISPATCH_IO_STREAM流式读写，
  // DISPATCH_IO_RANDOM随机读写；参数二是文件描述符；参数三创建的dispatch queue；参数四是错误处理block
  dispatch_io_t pipe_channel = dispatch_io_create(DISPATCH_IO_STREAM, fd, queue, ^(int error) {
      // 错误处理
  })
  
  // 设置一次读取的大小
  dispatch_io_set_low_water(pipe_channel, SIZE_MAX);
  
  // 读数据
  dispatch_io_read(pipe_channel, 0, SIZE_MAX, queue, ^(bool done, dispatch_data_t  _Nullable data, int error) {
       // 读完的每一部分数据流在这做处理
  });
  ```

### 3.3 GCD实现

- dispatch queue通过结构体和链表，实现为FIFO队列，FIFO队列管理的是追加到queue的block。但是block不是直接加到FIFO队列里，而是加入一个dispatch_continuation_t类型结构体（里面也存储着相应的执行上下文），结构体再加入FIFO队列

- 加入到dispatch queue的block执行时

- libdispatch（提供dispatch queue的组件）取出block对应的dispatch_continuation_t结构体，调用pthread_workqueue_additem_up函数（由更底层的libc组件提供pthread API）并传递进去相关信息

- pthread_workqueue_additem_up通知workqueue（由最底层XNU系统提供）增加要执行的项目

- workqueue根据传进来的信息判断是否生成一个新的线程来处理项目，处理项目时执行pthread_workqueue函数，函数调用libdispatch的回调函数，回调函数执行block

- block执行完之后进行通知dispatch group结束、释放dispatch_continuation_t结构体等操作，然后准备执行下一个block

- Dispatch Source是XNU系统内核发生各种事件时的处理方法，可根据需要选择使用

  | Dispatch Source种类名称（目前）     | 事件内容                                   |
  | ----------------------------------- | ------------------------------------------ |
  | DISPATCH_SOURCE_TYPE_DATA_ADD       | 自定义事件，变量增加                       |
  | DISPATCH_SOURCE_TYPE_DATA_OR        | 自定义事件，变量OR                         |
  | DISPATCH_SOURCE_TYPE_DATA_REPLACE   | 自定义事件，变量Replace                    |
  | DISPATCH_SOURCE_TYPE_MACH_SEND      | MACH端口发送                               |
  | DISPATCH_SOURCE_TYPE_MACH_RECV      | MACH端口接收                               |
  | DISPATCH_SOURCE_TYPE_MEMORYPRESSURE | 内存报警                                   |
  | DISPATCH_SOURCE_TYPE_PROC           | 进程相关事件监听，如进程退出、创建子线程等 |
  | DISPATCH_SOURCE_TYPE_READ           | 可读取文件映像时响应                       |
  | DISPATCH_SOURCE_TYPE_SIGNAL         | 接收到UNIX信号时响应                       |
  | DISPATCH_SOURCE_TYPE_TIMER          | 定时器                                     |
  | DISPATCH_SOURCE_TYPE_VNODE          | 文件状态监听，如文件删除、移动、重命名等   |
  | DISPATCH_SOURCE_TYPE_WRITE          | 可写入文件映像时响应                       |

  ```objective-c
  // 创建一个dispatch source，此处以定时器为例，参数一为上列表格内种类常量，参数二为要监听的系统句柄，传入什么看source常量的说明
  // 参数三是描述细节信息，传入什么看source常量的说明，参数四是处理该事件的block执行在哪个线程上
  dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
  
  // 定时器特有，设置定时器触发时刻、下次开始定时的间隔时间、可允许的事件误差
  dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 15ull * NSEC_PER_SEC),
      DISPATCH_TIME_FOREVER, 1ull * NSEC_PER_SEC);
  
  // dispatch source发生时的处理block
  dispatch_source_set_event_handler(timer, ^{
      NSLog(@"wakeup");
      // 用完后必须取消并释放dispatch source，否则该dispatch source会一直活动
      dispatch_source_cancel(timer);
      timer = NULL;
  });
  
  // 如果有需要在取消dispatch source时有自定义行为，则定义这个函数，比如进行内存、资源释放等
  dispatch_source_set_cancel_handler(timer, ^{
      NSLog(@"canceled");
  });
  
  // 开始执行dispatch source，如果是开始执行还可以使用dispatch_active()；
  dispatch_resume(timer);
  
  // 暂停dispatch source时也是使用dispatch_suspend()，但是次数要与dispatch_resume匹配
  ```
