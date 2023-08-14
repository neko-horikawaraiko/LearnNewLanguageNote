public class IntroductionToJavaClassInterface {
    public static void main(String[] args) {

    }

    void fun1() {
        ExampleClass1 eClass1 = new ExampleClass1();
        System.out.println(eClass1.defaultInt);         // 使用成员变量
        System.out.println(eClass1.classInt);           // 使用静态变量，不推荐
        System.out.println(ExampleClass1.classInt);     // 使用静态变量
        System.out.println(eClass1.getDoubleNum());     // 调用方法
        System.out.println(eClass1.getClassInt());      // 调用静态方法，不推荐
        System.out.println(ExampleClass1.getClassInt());// 调用静态方法

        // 内部类对象创建方式
        OuterClass outerObj = new OuterClass();
        OuterClass.InnerClass innerObj = outerObj.new InnerClass();                 // 非静态内部类
        OuterClass.StaticInnerClass innerObj2 = new OuterClass.StaticInnerClass();  // 静态内部类
    }

    // 多态、动态绑定
    void fun2() {
        BaseClass bClass = null;
        bClass = new SubClass();        // 多态：父类型变量可以引用子类型对象，可以给将子类实例传给父类型参数
        bClass.description();           // 动态绑定：对象调用父类还是子类的重写实例方法，是看对象的实际类型而不是声明类型。
                                        // 调用时查找方法实现会沿着继承链向上找，调用找到的第一个实现。此处调用SubClass.description()
        // 注意：匹配方法签名和绑定方法实现不同。编译器根据声明类型匹配方法签名，虚拟机在运行时根据实际类型动态绑定方法实现

        if (bClass instanceof SubClass) {       // instanceof 测试变量所指对象是否是某个类的实例
            SubClass sClass = (SubClass)bClass; // 父类变量转换为子类变量需要强制转换，且父类变量必须实际引用的是子类实例
        }
    }

    // 判断相同
    void fun3() {
        BaseClass bcls1 = new BaseClass();
        BaseClass bcls2 = new BaseClass();
        System.out.println(bcls2.equals(bcls1));    // equals默认实现为==，false
        System.out.println(bcls2 == bcls1);         // 是否同一对象，false
        bcls2 = bcls1;
        System.out.println(bcls2.equals(bcls1));    // true
        System.out.println(bcls2 == bcls1);         // true

        SubClass scls1 = new SubClass(1);
        SubClass scls2 = new SubClass(1);
        System.out.println(scls2.equals(scls1));    // equals被重载，true
        System.out.println(scls2 == scls1);         // 是否同一对象，false
        scls2 = scls1;
        System.out.println(scls2.equals(scls1));    // true
        System.out.println(scls2 == scls1);         // true
    }
}

// 同一文件中其他类不能public，因此也只能从包内访问到，包外不可见
// 类前可见性修饰符只有public和空，没有private、protected
// 类的字段与方法介绍
class ExampleClass1 {
    // 字段（属性）：实例变量（对象变量），静态变量（类变量）
    // 一般字段和方法声明可以在类中按任意顺序出现，不写可见性修饰符则默认public
    int intNum;
    int intNum2 = intNum + 1;   // 除了这种一个字段需要别的字段的值来初始化的情况
    double doubleNum;           // 实例变量，数值类型不写初始值时默认值0
    String str;                 // 引用类型变量默认值null
    boolean b;                  // boolean类型变量默认值false
    char c;                     // char类型变量默认值'\u0000'
    int defaultInt = 1;         // 带默认初始值的实例变量
    private double doubleNum2;
    static int classInt = 3;    // 静态变量，类的多个实例共有，需要外部设置时最好提供单独的方法进行设置，而不是与实例变量共用初始化方法

    // 构造方法，默认public，但是可以private以避免用户创建该类实例
    // 类中没有任一构造方法时，编译器自动提供一个无参且方法体为空的构造方法，即 ExampleClass1() {}
    // 但是类中有任一带参构造方法时，无参的构造方法不会生成，作为基类会编译失败
    ExampleClass1() {
        doubleNum = 1;
    }

    ExampleClass1(double newDoubleNum) {
        doubleNum = newDoubleNum;
    }

    // 方法：实例方法、静态方法（类方法）
    // 私有字段需要给外部提供访问器（getter）、修改器（setter）时，约定俗成方法名用get、set开头
    double getDoubleNum() {     // 实例方法可以调用静态方法、实例方法，访问静态变量、实例变量
        return doubleNum;
    }

    static int getClassInt() {  // 静态方法，只能调用静态方法、访问静态变量，不能调用实例方法、访问实例变量
        return classInt;
    }

    // 用System.out.print(object)之类打印一个对象时是调用的该对象的toString方法，见IntroductionToJavaBasic.java的fun4
    // 为了给出更多信息应当在自己的类重写toString方法
    public String toString() {
        return "intNum" + intNum;
    }
}

// 类的this使用方法、获取被隐藏的字段
class ExampleClass2 {
    private int i;
    private static double k = 0;

    ExampleClass2() {
        this(1);                // 使用this调用另外的构造方法，且本方法中其他语句要在这个调用之后
    }

    ExampleClass2(int intNum) {
        i = intNum;
    }

    void setI(int i) {
        this.i = i;             // 同名局部变量会隐藏类的相应字段，但是可以通过this来重新引用到
    }

    static void setK(int k) {
        ExampleClass2.k = k;    // 而静态变量可以且只能通过类名来引用到
    }
}
// 直接在类中加main方法来测试
/*
public class MyCircle {
    public static void main(Strings[] args) {
        MyCircle circle = new myCircle();
        System.out.println(circle.getArea());
    }
    double radius;
    // 构造方法
    MyCircle() {
        radius = 1;
    }
    MyCircle(double newRadius) {
        radius = newRadius;
    }
    // 方法
    double getArea() {
        return radius * radius * Math.PI;
    }
    double getPerimeter() {
        return 2 * radius * Math.PI;
    }
    void setRadius(double newRadius) {
        radius = newRadius;
    }
}
*/

// 内部类
// 1.内部类编译成的类名为OuterClass$InnerClass
// 2.内部类可以直接引用所有定义在外部类的数据和方法，包括私有。
// 3.内部类可以使用可见性修饰符定义
// 4.内部类可以使用static定义，此时外面访问内部类需要使用外部类的名字访问，内部类无法访问非静态的外部类成员
// 5.从外面创建内部类对象：内部类非静态与静态创建方式不同，见IntroductionToJavaClassInterface.fun1注释
class OuterClass {
    public class InnerClass {}
    static public class StaticInnerClass {}

    public void fun() {
        // 匿名内部类，同时定义内部类且创建内部类实例
        // 1.必须继承某基类或者实现某接口，但是不能写extends或者implements
        // 2.匿名内部类必须实现父类或者接口中所有抽象方法
        // 3.匿名内部类的“创建”是使用无参构造方法创建的，就是代码中基类或者接口后的一对小括号
        // 4.匿名内部类编译成的类名为OuterClass$n，n代表第几个出现的内部类
        new Edible() {
            public String howToEat() {
                return "fun";
            }
        };
    }
}

// 不可变类、不可变对象
// 指字段全部私有、没有setter、没有返回可变引用变量字段的getter
class ExampleImmutableClass {
    private int id;                     // 私有基本类型变量且没有对应setter，可以
    private String name;                // 私有引用变量没有setter，String不可变getter返回的也就不可变，可以
    // private java.util.Date dateCreated; // ×，私有引用变量没有setter，但是getter返回Date是可变对象，不能出现

    public ExampleImmutableClass(int ssn, String newName) {
        id = ssn;
        name = newName;
        // dateCreated = new java.util.Date();
    }
    
    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    // public java.util.Date getDateCreated() {
    //     return dateCreated;
    // }
}

// 类继承，超类（父类、基类）
// 其实没标注extends的类也是一个类的子类，是java.lang.Object类的子类
class BaseClass {
    private String strParam;
    
    public BaseClass() {}

    public BaseClass(String str) {
        strParam = str;
    }
    
    public String getStrParam() {
        return strParam;
    }

    public void setStrParam(String str) {
        strParam = str;
    }

    public void description() {
        System.out.println("BaseClass");
    }

    protected void exFun() {

    }

    // final方法不可被子类重写
    public final void finalfun() {

    }
}

// 类继承，次类（子类、扩展类、派生类），仅有单继承，无多重继承
// 子类继承所有基类的public方法（即可以用子类对象调用基类声明的public方法），但是不继承父类构造函数
// 子类对父类数据域的访问同其他外部类一样，可以直接访问public字段，只能通过父类提供的getter、setter访问private字段
class SubClass extends BaseClass {
    private int intParam;

    public SubClass() {
        super();            // 调用父类构造方法，必须出现在子类构造方法第一行，不能使用父类名字调用父类构造方法如BaseClass()
        intParam = 42;
    }
    
    public SubClass(int i) {
        // 不显式调用父类构造方法，编译器则会先隐式调用父类的无参构造方法
        // 即使类内构造方法互相调用，对父类的构造方法的隐式调用也只调用一次，位置是第一个被调用的本类构造函数（外部new或者子类调用之类）最开始
        intParam = i;
    }

    public SubClass(int i, String str) {
        super(str);
        intParam = i;
    }

    public int getIntParam() {
        return intParam;
    }

    public void setIntParam(int i) {
        intParam = i;
    }

    public void fun(String str) {
        setStrParam(str);       // 调用父类方法，也可写作super.setStrParam(str);
    }

    // 方法重写，返回值类型、方法名、形参类型数量顺序（即返回值类型与方法签名）要完全相同
    // 父类方法在子类中可见才再能被重写，如果父类是private方法，则这个方法就与父类的那个方法完全互相独立
    // 父类静态方法没法在子类中重写，子类静态方法会隐藏父类同返回值类型与方法签名的静态方法，父类该方法可用父类名.静态方法名调用
    @Override                   // 重写标注，表示被标注的方法必须重写父类的一个方法，否则编译器报错
    public void description() {
        super.description();    // 调用父类该方法，但是SubClass的子类就没法调到BaseClass的description方法了（即super.super.description();语法错误）
        System.out.println("SubClass");
    }

    // 重写比较相等方法示例，注意形参类型为Object而不是具体某个类型
    @Override
    public boolean equals(Object o) {
        if (o instanceof SubClass) {
            return intParam == ((SubClass)o).getIntParam();
        } else {
            return this == o;
        }
    }

    // 重写父类方法可以扩大可访问性，但是不能缩小
    @Override
    public void exFun() {
        super.exFun();
    }

    // final方法不可重写
    // @Override
    // public void finalfun(){}
}

// final类不可被继承
final class FinalClass{}
// class SubFinalClass extends FinalClass{}

// 抽象类，不可创建此类实例，可以不含有抽象方法
abstract class AbsClass {
    private int intNum;

    // 即使public也不能被外部调用来初始化，只能用于子类super来调用
    public AbsClass() {
        this.intNum = 1;
    }

    public void setIntNum(int intNum) {
        this.intNum = intNum;
    }

    public int getIntNum() {
        return intNum;
    }

    // 抽象方法，只定义不实现，只能出现在抽象类中，不能是静态方法
    public abstract void someFun();

    public void someFunToBeAbs() {
        System.out.println("someFunToBeAbs");
    }
}

class DeriveAbsClass extends AbsClass{
    @Override
    public void someFun() {
        System.out.println("DeriveAbsClass");
    }
}

abstract class DeriveAbsClass2 extends AbsClass {
    // 有必要时，子类可将父类非抽象方法覆盖为抽象方法
    @Override
    public abstract void someFunToBeAbs();
}

// 接口，不可创建接口实例，一个接口可以继承自多个接口（接口有多继承，类没有），接口类型变量可以引用所有实现该接口的对象（类似于基类类型变量）
/* public */ interface Edible {
    // 接口数据域都是public static final的（常量），方法都是public abstract的（抽象方法）
    public static final int K = 1;      // 可省略为int K = 1;
    public abstract String howToEat();  // 可省略为String howToEat();
}

// 接口多继承
interface MultiInterface<E> extends Comparable<E>, Cloneable, Edible{
}

class Orange extends BaseClass implements Edible, Comparable<Orange>, Cloneable {
    // 继承只能继承一个，接口可以实现多个
    Integer size;
    java.util.Date dt;

    @Override
    public String howToEat() {
        return "Eat";
    }

    // Comparable<E> 可比较泛型接口，实现接口的类要指明泛型的类，实现compareTo()方法，返回正、负、0值。实现了可比较接口的类就可以使用sort排序
    @Override
    public int compareTo(Orange o) {
        return size.compareTo(o.size);
    }

    // Cloneable 可克隆接口，不含有方法，称为“标记接口”，表示某个类有某特定属性。自定义类继承Cloneable接口时必须覆盖实现clone()方法
    @Override
    public Object clone() throws CloneNotSupportedException {
        // return super.clone();   // 浅复制
        
        // 深复制，对引用对象手动调用复制，以便拷贝出副本而不是仅仅拷贝一个引用
        Orange orangeClone = (Orange)super.clone();
        orangeClone.dt = (java.util.Date)(dt.clone());
        return orangeClone;
    }
}