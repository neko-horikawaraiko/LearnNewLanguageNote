// package IntroductionToJava;  // 声明该文件（类）放在哪个包中，不写则放在默认包。声明时这一句前面不能有注释也不能有空白

//import java.util.*;			// 导入包中的所有类
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import java.io.RandomAccessFile;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.RoundingMode;
import java.util.ArrayList;
import java.util.Arrays;		// 仅导入一个类
import java.util.HashMap;
import java.util.Locale;
import java.util.Random;
import java.util.Scanner;

// Java是大小写敏感语言，大小写不同表示不同的标识符
// 命名规范：类名每个单词首字母大写（帕斯卡命名法），变量名方法名第一个单词首字母小写，其余首字母大写（(小)驼峰命名法），
// 常量名所有字母大写，单词间下划线连接

// 编译器将.java文件（源代码）中所有类分别编译成.class文件（字节码），Java虚拟机（JVM）解释执行字节码，每次只解释并执行一步字节码

// Java文件名必须与文件里的唯一public类名相同
public class IntroductionToJavaBasic {
    // main函数为程序入口，方法头永远都这样写，必须public static void main，形参String[]
    public static void main(String []args) {
    }

    // 类内数据、方法的可见性修饰符（注意protected，在包内，不是子类也可见）
    //              | 类内可见 | 包内可见 | 子类可见 | 包外可见
    // public       |   √    |   √    |   √    |   √
    // protected    |   √    |   √    |   √    |
    // (空)         |   √    |   √    |        |
    // private      |   √    |        |        |
    public void fun1() {
        // 杂项小点
        // 由于浮点数精确度问题，比如100000000.0+0.000000001=100000000.0，相加的时候最好从小数加到大数

        int intNum;                                         // 不初始化时无默认值，如果使用则编译报错
        final double PI = 3.1415926;						// final声明常量
        Scanner input = new Scanner(System.in);				// System.in标准输入设备，默认键盘。Scanner可以从控制台或者文件读取数据
        System.out.print("Enter a number for radius: ");    // System.out标准输出设备，默认显示器
        double radius = input.nextDouble();
        double area = Math.pow(radius, 2) * PI;             // java也定义了常量Math.PI
        // println()输出函数，字符串连接可以直接用+，一对引号里的字符串常量不能跨行，要显示跨行需要\n
        System.out.println("The radius: " + radius + ",\nthe area of circle: " + area);
        // 格式化输出，输出后不换行。常用格式标识符：%b布尔，%c字符，%d整数，%f浮点，%e科学计数法，%s字符串，%%百分号
        // 格式中整数表示至少显示多长（空格+内容，对于数字含小数点与小数部分），小数表示小数点后一定保留多少位，负号表示左对齐
        System.out.printf("The radius: %f,\nthe area of circle: %.2f\n", radius, area);
        input.close();

        int i = 1;
        int j = 2;
        System.out.println("i + j is " + i + j);            // i + j is 12 (+号左结合，按字符串拼)
        System.out.println("i + j is " + (i + j));          // i + j is 3
    }

    private void fun2() {
        // 基本数据类型8种
        // 数字字面量数字之间可以加下划线连接
        byte byteNum = 0;		//8位带符号整数
        short shortNum = 0;		//16位带符号整数
        int intNum = 789_78;	//32位带符号整数，78978
        long longNum = 0;		//64位带符号整数
        float floatNum = 0.0f;	//32位浮点数
        double doubleNum = 0.0; //64位浮点数

        boolean b = true;

        char c = 'A';           // 2字节（16位）长度
    }
    
    protected void fun3() {
        // 从标准输入设备读取输入，读取到不正确范围或者类型的值会出现运行时错误
        Scanner input = new Scanner(System.in);
        byte byteValue = input.nextByte();
        short shortValue = input.nextShort();
        int intValue = input.nextInt();
        long longValue = input.nextLong();
        float floatValue = input.nextFloat();
        double doubleValue = input.nextDouble();

        String s1 = input.next();       // 读以空白字符结束的字符串（' '空格、'\t'制表、'\f'换页、'\r'回车、'\n'换行)
        String s2 = input.nextLine();   // 读一整行文本，以按下回车键为结束标志
        // 注意不要在上述nextByte()等数字输入、next()这些方法之后用nextLine()
        input.close();
    }

    // 数组在java中是对象
    void fun4() {
        // 一维数组
        int[] intArray1;    // 声明方式1，建议使用
        int intArray2[];    // 声明方式2，适用于C/C++程序员习惯
        // 仅这样声明时只是创建了一个可以去引用一个数组的“引用变量”，此时值为null，但是编译器可能会直接不给通过
        // 此时也不能给数组内元素赋值，因为并没有引用一个真正的数组

        // 创建一个数组，赋值给变量，数组必须指定大小（可以是变量），且创建后无法更改大小
        // 不给数组内元素赋值时，各元素数值型默认值为0，char型默认值为'\u0000'(\\u表示unicode，后跟4位16进制数)，boolean型默认值为false，引用型默认null
        double[] doubleArray1 = new double[10];
        doubleArray1[0] = 10;
        double[] doubleArray2 = {4.3, 5.5};     // 用数组初始化语法声明、创建、初始化数组，数组大小等于括号内元素个数
        // double[] doubleArray3;               // ×，数组初始化语法不能写成分开形式
        // doubleArray3 = {4.3, 5.5};           // ×，数组初始化语法不能写成分开形式
        // double[] doubleArray2 = new double[]{4.3, 5.5};  // ×，数组初始化语法不能使用指定维度方式
        System.out.print(new double[]{6, 7.7}); // 匿名数组，但是此时[]内不能填大小

        char[] city = {'D', 'a', 'l', 'l'};
        System.out.println(city);               // char[]可以直接 单独 打印出来成一个字符串，不用循环，
                                                // 但是如果用+连接其他东西打印出来就不是字符串了
        String[] stringArray = {"df"};
        System.out.println(stringArray);        // 默认调用了String[]的toString()实例方法
        // 比如上述打印出来[Ljava.lang.String;@7344699f，[（一个左方括号）表示一维数组，
        // L和后面的;表示数组内含有类或者接口（不含时没有L），后面是类型名，一个@字符，最后是该数组的哈希码

        // 数组复制
        int[] intArray3 = {7, 2, 3, 5, 9};
        int[] intArray4 = new int[6];       // 目标数组大小不小于源数组(对于方法①②)
        // intArray4 = intArray3;           // 仅复制了引用，即intArray3和intArray4都指向intArray3指的数组，
                                            // 此时intArray4原来指向的数组没有被引用，会被垃圾回收
        // 数组内容复制正确方法①：使用循环挨个元素复制（略）
        // 方法②：arraycopy方法，从源数组某位置拷贝到目标数组某位置，拷贝多长
        System.arraycopy(intArray3, 0, intArray4, 0, intArray3.length);
        // 方法③：clone方法
        intArray4 = intArray3.clone();

        // 数组方法
        Arrays.sort(doubleArray2);                  // 排序
        Arrays.parallelSort(doubleArray2);          // 多线程排序
        Arrays.sort(intArray3, 0, 3);               // 局部排序
        Arrays.parallelSort(intArray3, 0, 3);       // 多线程局部排序
        Arrays.binarySearch(doubleArray2, 3);       // 在按升序排好的数组中二分查找，找不到会插入并返回带符号的+1下标，即-(插入下标+1)
        Arrays.equals(doubleArray1, doubleArray2);  // 数组是否相同，需要内容、顺序都相同，而类型不同数组无法调用equals方法
        Arrays.fill(doubleArray1, 5);               // 全部填入5
        Arrays.fill(doubleArray2, 1, 3, 5);         // 1-3位置填入5
        Arrays.toString(intArray3);                 // 数组转字符串，带有默认格式
        // 数组foreach循环见fun7
    }

    void fun5() {
        // 多维数组
        int[][] intMatrix1;     // 声明方式1，建议使用
        int intMatrix2[][];     // 声明方式2，适用于C/C++程序员习惯
        int[][] intMatrix3 = new int[5][5];
        int[][] intMatrix4 = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
        int[][] intMatrix6 = {  // 锯齿数组
            {1, 2, 3},
            {2, 3}
        };
        int[][] intMatrix7 = new int[4][];  // 后面几维长度可以省略，但第一维的长度必须指定，虽然指定了但是里面没有实际的数组，取值会报错
    }

    void fun6() {    
        // 选择结构
        boolean b = true;
        boolean b2 = false;
        if (b) {
            //...
        } else if (b2) {
            //...
        }

        int someInt = 1;
        switch (someInt) {
            case 0:break;
            case 1:         //存在没有break的case最好注释一下
            case 2:break;
            default:
        }
    }

    void fun7() {
        // 循环结构
        int count = 0;
        while (count < 100) {
            // ...
        }

        do {
            // ...
        } while (count < 100);

        for (int i = 0; i < 100; i++) {
            // ...
        }
        // 使用break;跳过整个循环过程
        // 使用continue;跳过本次循环直接进入下一次循环（与判断）

        // 数组专属循环：foreach循环
        double[] doubleArray = {3.3, 4.4, 5};
        for (double e : doubleArray) {
            System.out.println(e);
        }
    }

    // 基本数据类型参数为值传递，对象类型参数为引用传递（实际上为传引用变量值的值传递，但由于是引用变量，所以可以修改变量所指对象的内容）
    void fun8(int i, int[] j) {
        // ...
    }

    // 方法重载，同方法名不同形参列表（形参类型或者个数不同）
    // 只是形参名/修饰符/返回值类型不同的话还是和被重载的是同一个方法，报重定义错
    // 注意歧义错误，指调用一个方法时，可以匹配上重载的多个方法，这几个匹配同样精确，会编译报错，要修改调用的实参或者修改方法形参
    void fun8(int i, int j) {
        // ...
    }

    // 可变长参数列表，被当做数组对待，必须声明在形参列表最后一个的位置，且最多只能有一个可变长参数
    void fun9(double... numbers) {
        // ...
    }

    // 形参返回值都可以是二维数组，数组传给形参仍然是传引用
    int[][] fun10(int[][] numbers) {
        return numbers;
    }

    void fun11() {
        // 数学函数
        Math.sin(Math.PI);
        Math.cos(0);
        Math.tan(0);
        Math.asin(0);
        Math.acos(0);
        Math.atan(0);
        Math.toDegrees(Math.PI / 2);
        Math.toRadians(30);
        Math.exp(1);
        Math.log(Math.E);
        Math.log10(10);
        Math.pow(2, 3);
        Math.sqrt(4);
        Math.ceil(4.5);     // 向上取整，返回双精度的整数
        Math.floor(4.5);    // 向下取整，返回双精度的整数
        Math.rint(4.5);     // 向最近整数取整，如果小数是0.5这种的向最近偶数取整，返回双精度的整数
        Math.round(4.5);    // 向最近整数取整，如果小数是0.5这种的不管正负，向+0.5的整数取整，单精度/双精度小数返回int/long整数
        Math.max(2, 3);
        Math.min(2, 3);
        Math.abs(-3.4);
        Math.random();      // 0.0 <= Math.random() < 1.0
    }

    void fun12() {
        // 字符方法
        Character.isDigit('0');             // 是否数字
        Character.isLetter('A');            // 是否字母
        Character.isLetterOrDigit('9');     // 是否字母或数字
        Character.isLowerCase('d');         // 是否小写
        Character.isUpperCase('A');         // 是否大写
        Character.toLowerCase('A');         // 转小写
        Character.toUpperCase('a');         // 转大写
        Character.valueOf('c');             // 转为Character
        Character.toString('c');            // 转字符串
        Character.compare('c', 'd');        // 比较
        Character.digit('c', 16);           // 字符（的unicode码）用某进制表示下的数值
        Character.isWhitespace(0);          // 是否是Java定义的空白符
        Character.isSpaceChar(0);           // 是否是Unicode定义的空白符
        Character ch = 'c';
        ch.charValue();
        
    }

    void fun13() {
        // 字符串方法，String类内容不可变
        String str = "Welcome to Java";                             // 字符串常量创建String
        String str2 = new String("Welcome to Java");                // 字符串常量创建String
        String str3 = new String(new char[]{'W', 'e', 'l', 'l'});   // 字符数组创建String
        char[] chars = "Java".toCharArray();                        // String创建字符数组
        
        String str4 = "Welcome to Java";
        boolean b1 = str == str2;           // false，字符串使用new创建String时，不会往常量池内存入该字符串，变量引用堆上常量池外一个普通的字符串对象
        boolean b2 = str == str4;           // true，字符串直接量创建String时，直接往常量池内存入该字符串，称为“限定字符串”对象，变量引用常量池内的这个字符串对象
        str2 = str2.intern();               // 常量池内没有该字符串时先复制一份进去（有就不起作用），然后返回常量池内对象的引用
        boolean b3 = str == str2;           // true

        str.length();
        "Welcome".length();                 // 长度为7
        "".length();                        // 长度为0
        str.charAt(0);
        str.substring(3);                   // 从3开始到末尾的子字符串
        str.substring(0, 6);                // 0 <= substring < 6的子字符串（参数是开始结束的index，不是开始index和长度）
        str.concat(" hahaha");              // 实参字符串连接到str字符串后面，等于 str + " hahaha"
        str.toUpperCase();                  // 全转大写
        str.toLowerCase();                  // 全转小写
        str.trim();                         // 去掉两侧空白字符，如空格、\t等
        str.equals("Welcome");              // 是否相等
        // str == "Welcome"                 // == 比较的是是否相同对象
        str.equalsIgnoreCase("Welcome");    // 是否相等，忽略大小写
        str.compareTo("Welcome");           // 比较大小，按字典序，str小于"Welcome"则返回小于0，等于则返回0，大于则返回大于0
        str.compareToIgnoreCase("Welcome"); // 比较大小，忽略大小写
        str.startsWith("Welcome");          // 是否开头
        str.endsWith("Welcome");            // 是否结尾
        str.contains("Welcome");            // 是否包含
        str.indexOf('e');                   // 字符出现的第一个位置
        str.indexOf('e', 3);                // 从某位置开始，字符出现的第一个位置
        str.indexOf("come");                // 字符串出现的第一个位置
        str.indexOf("come", 3);             // 从某位置开始，字符串出现的第一个位置
        str.lastIndexOf('e');               // 字符出现的最后一个位置
        str.lastIndexOf('e', 3);            // 从某位置开始，字符出现的最后一个位置
        str.lastIndexOf("come");            // 字符串出现的最后一个位置
        str.lastIndexOf("come", 3);         // 从某位置开始，字符串出现的最后一个位置
        // 正则说明见正则markdown文件
        str.replace("el", "df");            // 所有匹配的地方替换
        str.replace('e', 's');              // 所有匹配的地方替换
        str.replaceFirst("ed", "df");       // 第一个匹配（正则）的地方替换
        str.replaceAll("ed", "df");         // 所有匹配（正则）的地方替换
        str.split("e");                     // 按某正则标识分割字符串，该正则标识不会出现在分割产生的数组中
        str.matches("Welcome.*");           // 符合某正则
        str.getChars(2, 4, chars, 1);       // 从str取下标[2,4)的字符替换chars字符数组从1开始的位置，注意字符替换到的位置不能超过chars的最大下标

        String.format("%7.2f", 45.556);     // 返回一个格式化字符串

        // 字符串转数字
        Integer.parseInt("3");
        Double.parseDouble("3.5");
        // 数字转字符串
        str = 0 + "";
        String.valueOf(0);
        Integer.toString(0);
        // 基本类型、字符数组转字符串
        String.valueOf('c');
        String.valueOf(new char[]{'W', 'e', 'l', 'l'});
        String.valueOf(2.44);
        String.valueOf(true);
    }

    void fun14() {
        // 时间日期
        java.util.Date currentDate = new java.util.Date();      // 为当前时间创建一个Date对象
        java.util.Date someDate = new java.util.Date(2333);     // 创建一个1970.1.1 0点往后指定毫秒数的Date对象
        currentDate.getTime();                                  // 从1970.1.1 0点到currentDate所指时间的毫秒
        currentDate.toString();                                 // 日期时间的字符串表示
        someDate.setTime(42);                                   // 设定新时间
    }

    void fun15() {
        // 随机数，种子相同时生成的随机数列也相同
        Random random1 = new Random();      // 当前时间为种子创建一个Random对象
        Random random2 = new Random(42);    // 特定值为种子创建一个Random对象
        random1.nextInt();                  // 返回随机int
        random1.nextInt(4);                 // 返回 0 <= 随机值 < 4 的随机int
        random1.nextInt(2, 5);              // 返回 2 <= 随机值 < 5 的随机int
        random1.nextLong();
        random1.nextLong(3);
        random1.nextLong(2, 5);
        random1.nextDouble();               // 0 <= 随机值 < 1 的随机double
        random1.nextDouble(4);              // 0 <= 随机值 < 4 的随机double
        random1.nextDouble(2, 5);           // 2 <= 随机值 < 5 的随机double
        random1.nextFloat();                // 0 <= 随机值 < 1 的随机float
        random1.nextFloat(4);
        random1.nextDouble(2, 5);
    }

    void fun16() {
        // 基本数据类型的包装类，包装类对象与相应值之间可以自动转换
        Integer intNum1 = 5;                // 直接赋值
        Integer intNum2 = new Integer(5);   // new方法（已废弃），改用valueOf()
        Integer intNum3 = new Integer("5"); // new方法传入字符串（已废弃），改用valueOf()
        Integer intNum4 = Integer.MAX_VALUE;// 最大值最小值（Byte/Short/Integer/Long）
        Double doubleNum = Double.MAX_VALUE;// 最大正值最小正值（Double/Float）
        Character charValue = 'c';
        Boolean boolValue = false;

        // 各个数值类型都有的方法
        Integer.compare(1, intNum1);        // 比较，返回-1,0,1来指示大小
        Double.compare(3.3, 0.3);
        Integer.toString(2);                // 转换字符串
        Integer.valueOf("2");               // 转换数值
        Integer.valueOf(2);
        Integer.parseInt("3");              // 转换数值
        Double.parseDouble("3.3");
        intNum1.compareTo(intNum4);         // 比较，返回-1,0,1来指示大小
        intNum1.byteValue();                // 转换成其他类型数值
        intNum3.shortValue();
        intNum3.intValue();
        intNum4.longValue();
        intNum1.doubleValue();
        intNum1.floatValue();

        // 特有方法
        Integer.parseInt("1A", 16);         // 以某进制转换字符串为数值，字符串表示的数字必须符合该进制的表示法（比如2进制不能出现2之类）
        String.format("%x", 26);            // 上述的反向转换

        // Boolean型方法
        Boolean.parseBoolean("true");           // 字符串转boolean
        Boolean.toString(false);                // boolean转字符串
        Boolean.valueOf("false");               // 转为Boolean
        Boolean.valueOf(false);    
        Boolean.compare(false, false);          // 比较
        Boolean.logicalAnd(false, boolValue);   // 逻辑运算
        Boolean.logicalOr(false, false);
        Boolean.logicalXor(false, false);
        boolValue.booleanValue();               // 取值
        boolValue.toString();                   // 转字符串
        boolValue.compareTo(false);             // 比较

        // 部分Character型方法见fun9
    }

    void fun17() {
        // BigInteger、BigDecimal任意大小任意精度的数
        BigInteger a = new BigInteger(Long.MAX_VALUE + ""); // 字符串方式初始化
        BigInteger b = new BigInteger("2");
        BigInteger c = a.multiply(b);       // a*b
        c = a.add(b);                       // a+b
        c = a.subtract(b);                  // a-b
        c = a.divide(b);                    // a/b

        BigDecimal ad = new BigDecimal("1.0");              // 字符串方式初始化
        BigDecimal bd = new BigDecimal("3");
        BigDecimal cd = ad.multiply(bd);        // ad*bd
        cd = ad.add(bd);                        // ad+bd
        cd = ad.subtract(bd);                   // ad-bd
        // cd = ad.divide(bd);                  // ad/bd，计算结果无限小数会抛异常
        cd = ad.divide(bd, 20, RoundingMode.UP);// 使用指定小数长度和舍入方式的方法避免上述异常
    }

    void fun18() {
        // StringBuilder StringBuffer，可更改的String，StringBuffer用于多线程访问，StringBuilder用于单线程访问
        StringBuilder builder = new StringBuilder();    // 创建容量16的空StringBuilder
        builder = new StringBuilder(5);                 // 创建容量5的空StringBuilder
        builder = new StringBuilder("welcome");         // 带初始内容的StringBuilder

        char[] chars = {'f', 's', 'e', 'd'};
        builder.append(chars);                          // 添加字符数组到后面
        builder.append(chars, 2, 3);                    // 添加字符数组从2及以后的3个字符到后面
        builder.append("fe");                           // 添加字符串/String到后面
        builder.append(3);                              // 添加基本类型的值到后面
        builder.delete(1, 2);                           // 删除[1,2)之间的字符
        builder.deleteCharAt(3);                        // 删除位置3的字符
        builder.insert(4, chars);                       // 插入字符数组到指定位置
        builder.insert(4, chars, 2, 3);                 // 插入字符数组从2及以后的3个字符到指定位置
        builder.insert(4, "fe");                        // 插入字符串/String到指定位置
        builder.insert(4, 3);                           // 插入基本类型的值到指定位置
        builder.replace(0, 1, "ddd");                   // [0,1)替换为新字符串，长度可不同
        builder.reverse();                              // 反转字符串
        builder.setCharAt(0, '0');                      // 替换指定位置字符
        builder.toString();                             // 返回字符串
        builder.capacity();                             // builder的容量（能存的字符数量，会自动增加不会自动减小，永远不小于length()）
        builder.length();                               // builder中字符串长度（实际存的字符数量）
        builder.setLength(10);                          // 设置其中字符串新长度（不改变容量，比原来length大则在字符串后添加空字符'\u0000'补齐，比原来length小则其中字符串截断）
        builder.charAt(3);                              // 返回下标位置的字符
        builder.substring(3);                           // 从下标开始到末尾的子字符串
        builder.substring(0, 2);                        // 下标为[0,2)的子字符串
        builder.trimToSize();                           // 减小容量至字符串长度
    }

    void fun19() {
        // ArrayList可变长列表（数组）
        ArrayList<Double> doubleArray = new ArrayList<>();          // 声明、创建列表（列表所存为对象类型，没法存基本类型）
        ArrayList<Double> doubleArray2 = new ArrayList<Double>();   // 实质同上
        Double[] doubles = {3.0, 4.5, 7.2};
        doubleArray2 = new ArrayList<>(Arrays.asList(doubles));     // 普通数组转列表
        doubles = new Double[doubleArray2.size()];
        doubleArray2.toArray(doubles);                              // 列表转普通数组（doubleArray2直接复制到doubles中）

        doubleArray.add(3.0);           // 添加到末尾
        doubleArray.add(1, 4.0);        // 添加到下标位置处（下标<=size）
        doubleArray.contains(2.0);      // 是否包含元素
        doubleArray.get(1);             // 获取下标所指元素
        doubleArray.set(1, 5.0);        // 设置（更新）下标所指元素（下标<size）
        doubleArray.indexOf(4.0);       // 第一个匹配该元素的下标，没有则返回-1
        doubleArray.lastIndexOf(4.0);   // 最后一个匹配该元素的下标，没有则返回-1
        doubleArray.isEmpty();          // 是否为空
        doubleArray.remove(1);          // 移除下标所指元素
        doubleArray.remove(3.0);        // 移除指定元素
        doubleArray.size();             // 列表大小
        doubleArray.clear();            // 清空列表
        doubleArray.toString();         // 返回列表的字符串表示法
        java.util.Collections.sort(doubleArray);    // 列表的排序方法
                                                    // 数组排序方法用java.util.Arrays.sort()，见fun4
        java.util.Collections.max(doubleArray);     // 最大值
        java.util.Collections.min(doubleArray);     // 最小值
        java.util.Collections.shuffle(doubleArray); // 随机打乱
        for (Double d : doubleArray) {  // 数组列表循环，与普通数组相同，见fun7
        }
    }

    void fun20() {
        // HashMap散列表，无序容器
        HashMap<Integer, String> mMap = new HashMap</*Integer, String*/>();
        HashMap<Integer, String> mMap2 = new HashMap<>(){};
        mMap.put(1, "null");            // 添加键值对
        mMap.put(3, "sojdo");
        mMap.putAll(mMap2);             // 添加另一个HashMap
        mMap.putIfAbsent(4, "saaa");    // 如果没有key则添加
        mMap.replace(3, "aaa");         // key替换为新value
        mMap.size();                    // 大小
        mMap.isEmpty();                 // 是否空
        mMap.containsKey(1);            // 是否含某个key
        mMap.containsValue("null");     // 是否含某个value
        mMap.keySet();                  // 所有key
        mMap.values();                  // 所有value
        mMap.get(1);                    // 获取key对应的value
        mMap.remove(3);                 // 移除某个键值对
        mMap.clear();                   // 清空HashMap

        for (Integer i : mMap.keySet()) {}
        for (String s : mMap.values()) {}
    }

    void fun21() {
        // File文件管理类
        File file1 = new File("pathname");          // 用指定路径名创建File对象，创建目录或文件
        file1 = new File("directory", "pathname");  // 用指定目录路径+子路径名创建File对象，创建目录或文件
        File file2 = new File(file1, "pathname");   // 用目录File对象+子路径名创建File对象，创建目录或文件

        file1.exists();             // 对象代表的文件/目录是否存在
        file1.canRead();            // 对象代表的文件/目录是否可读
        file1.canWrite();           // 对象代表的文件/目录是否可写
        file1.isDirectory();        // 对象代表的是否是目录
        file1.isFile();             // 对象代表的是否是文件
        file1.isAbsolute();         // 对象代表的文件/目录是否是绝对路径创建
        file1.isHidden();           // 对象代表的文件/目录是否隐藏
        file1.getAbsolutePath();    // 返回对象初始化使用的路径名的绝对路径
        try {
            file1.getCanonicalPath();   // 返回对象代表的文件/目录的规整后绝对路径
        } catch (IOException e) {}
        file1.getName();            // 返回对象初始化使用的路径名的文件名/目录名
        file1.getPath();            // 返回对象初始化使用的路径名（约等于getParent() + getName()）
        file1.getParent();          // 返回对象初始化使用的路径名的父目录
        file1.lastModified();       // 返回对象代表的文件/目录的最后修改时间
        file1.length();             // 返回对象代表的文件的文件大小，目录则返回0
        file1.listFiles();          // 返回对象代表的目录内的文件
        file1.delete();             // 删除对象代表的文件/目录
        file1.renameTo(file2);      // 将对象代表的文件/目录换成指定名字（重命名/移动文件）
        file1.mkdir();              // 创建对象代表的目录
        file1.mkdirs();             // 创建对象代表的目录及其父目录
    }

    void fun22() throws FileNotFoundException{
        // 文本类文件读写PrintWriter、Scanner，网络文件URL
        File file1 = new File("null.txt");
        PrintWriter pw1 = new PrintWriter(file1);   // 用File对象创建PrintWriter对象
        PrintWriter pw2 = new PrintWriter("filename");// 用文件名字符串创建PrintWriter对象
        Scanner scanner1 = new Scanner(file1);      // 用File对象创建Scanner对象。Scanner可以从控制台或者文件读取数据
        Scanner scanner2 = new Scanner("acb");      // 用String对象创建Scanner对象

        pw1.print("dd");                            // 写入字符串
        pw1.print('d');                             // 写入字符
        pw1.print(new char[]{'a', 'b'});            // 写入字符数组
        pw1.print(3);                               // 写入int/long/float/double/boolean
        pw1.println("dd");                          // 同上，但是多加一个换行
        pw1.println('d');
        pw1.println(new char[]{'a', 'b'});
        pw1.println(3);
        pw1.printf("dd%d", 3);                      // 格式化输出
        pw1.close();
        pw2.close();

        // next()、nextByte()...为“标记读取方法”，先删分隔符，从第一个非分隔符开始读取一段数据，直到分隔符为止，且将分隔符留下
        // nextLine()读取一行，从当前位置开始直到行分隔符为止，所以使用控制台读取数字时，若连续使用nextInt()、nextLine()，而用户使用回车作为输入结束，则nextLine()读取出空值
        // 因为nextInt()读取了数字到行分隔符截止，并留下了分隔符，nextLine()读取剩下的（什么都没有）到行分隔符，所以读取空值
        scanner1.hasNext();                         // 是否还有更多数据
        scanner1.nextLine();                        // 读取下一行，以换行结束
        scanner1.next();                            // 读取下一段数据作为字符串返回（一段以分隔符作为划分标准，默认为Java空白符，即Character.isWhitespace）
        scanner1.nextByte();                        // 读取下一段数据作为byte值返回
        scanner1.nextShort();
        scanner1.nextInt();
        scanner1.nextLong();
        scanner1.nextFloat();
        scanner1.nextDouble();
        scanner1.useDelimiter(",");                 // 更换分割符
        scanner1.useRadix(16);                      // 更换获取数字基数
        scanner1.useLocale(Locale.CHINA);           // 更换区域属性（影响某些格式读取，比如小数，123,123区域是德国时读取为123.123，区域是中国时读取为123123.0）
        scanner1.reset();                           // 重置为默认分割符、默认基数、默认区域
        scanner1.close();
        scanner2.close();

        // try-with-resource方式，try跟一个圆括号里面声明及创建资源，该资源需要继承自AutoCloseable，后面花括号里使用资源
        // 结束时这种方式不需要手动close，程序会自动调用close
        try (
            PrintWriter pr3 = new PrintWriter(file1);// 声明与创建资源需要在同一行语句，不过可以多行创建多个资源
            Scanner scanner3 = new Scanner(file1);
        ) {
            pr3.print("dd");
            scanner3.hasNext();
        }

        // 网络文件URL读取
        try {
            java.net.URL url = new java.net.URL("http://www.baidu.com");
            Scanner input = new Scanner(url.openStream());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    void fun23() throws FileNotFoundException, IOException {
        // 二进制文件读写，根类二进制IO类InputStream/OutputStream，二进制文件IO类（字节读取）FileInputStream/FileOutputStream
        // 二进制文件IO类（基本数据类型基类）FilterInputStream/FilterOutputStream，二进制文件IO类（基本数据类型）DataInputStream/DataOutputStream
        // 带缓冲区二进制文件IO类BufferedInputStream/BufferedOutputStream，二进制文件IO类（对象类型）ObjectInputStream/ObjectOutputStream
        // 随机位置IO类RandomAccessFile
        // 可以使用try-with-resource方式声明二进制文件IO类对象

        // FileInputStream/FileOutputStream方法全部继承自InputStream/OutputStream
        File file1 = new File("null.dat");
        FileInputStream fis1 = new FileInputStream(file1);      // 用File对象创建FileInputStream对象
        FileInputStream fis2 = new FileInputStream("filename"); // 用文件名字符串创建FileInputStream对象
        
        fis1.read();                // 读下一个字节，没有可读的返回-1
        byte[] bts = new byte[133];
        fis1.read(bts);             // 读取bts.length个字节到数组bts中，返回实际读取的字节，没有可读的返回-1
        fis1.read(bts, 10, 11);     // 读取11个字节到数组bts中从第10位开始的位置，返回实际读取的字节，没有可读的返回-1
        fis1.available();           // 返回可读字节数估计值
        fis1.skip(82);              // 跳过且丢弃指定字节数，返回实际跳过的字节数
        fis1.markSupported();       // 输入流是否支持mark()和reset()方法
        fis1.mark(5);               // 在输入流中标记当前位置，5为mark标记失效前可读取的最大字节数（但实际上与这个数字和输入流的具体类型两者有关）
        fis1.reset();               // 输入流重新定位到最后一次调用mark()方法时的位置
        fis1.close();               // 关闭流
        fis2.close();

        FileOutputStream fos1 = new FileOutputStream(file1);        // 用File对象创建FileOutputStream对象
        FileOutputStream fos2 = new FileOutputStream("filename");   // 用文件名字符串创建FileOutputStream对象
        FileOutputStream fos3 = new FileOutputStream(file1, true);  // 参数2设为true为向文件中追加

        fos1.write(65);         // 将指定字节写出到输出流(写出去的是表示65的字节，按数字读出来就是65，按字符读出来就是'A')
        fos1.write(bts);        // 将bts数组中所有字节写出到输出流
        fos1.write(bts, 1, 3);  // 将bts数组中从1开始的3个字节写出到输出流
        fos1.flush();           // 写出在缓冲区中的所有字节
        fos1.close();           // 关闭流
        fos2.close();
        fos3.close();

        // 写入与读取需要配套使用
        DataInputStream dis1 = new DataInputStream(fis1);   // 用InputStream对象创建DataInputStream对象
        DataOutputStream dos1 = new DataOutputStream(fos1); // 用OutputStream对象创建DataOutputStream对象

        dis1.readBoolean();         // 读一个boolean值
        dis1.readByte();            // 读一个byte值
        dis1.readUnsignedByte();    // 读一个无符号byte值
        dis1.readChar();            // 读一个字符值
        dis1.readFloat();           // 读一个float值
        dis1.readDouble();          // 读一个double值
        dis1.readInt();             // 读一个int值
        dis1.readLong();            // 读一个long值
        dis1.readShort();           // 读一个short值
        dis1.readUnsignedShort();   // 读一个无符号short值
        dis1.readLine();            // 读一行字符（已废弃，SDK文档指明读取不准确）
        dis1.readUTF();             // 以UTF格式读字符串

        dos1.writeBoolean(false);   // 写一个boolean值
        dos1.writeByte(41);         // 写一个参数的低位字节
        dos1.writeBytes("sce");     // 写一个字符串中各字符的低位字节
        dos1.writeChar('1');        // 写一个字符（2字节）
        dos1.writeChars("daa");     // 写一个字符串中所有字符（每个字符2字节）
        dos1.writeFloat(8.1f);      // 写一个float值
        dos1.writeDouble(3.12);     // 写一个double值
        dos1.writeInt(42);          // 写一个int值
        dos1.writeLong(6903912313L);// 写一个long值
        dos1.writeShort(45);        // 写一个short值
        dos1.writeUTF("xia");       // 以UTF格式写字符串

        dis1.close();
        dos1.close();
        // readUTF/writeUTF注意事项：writeUTF会先写入2字节无符号数字作为长度信息，所以写入时需要对过长的字符串进行分片，读取是同样是分片读取再拼接
        // writeBytes、writeChars、writeUTF使用区别：writeBytes适用于全都是ASCII码的字符组成的字符串；
        // writeChars写入2字节字符的序列，用readChar读取适当长度；writeUTF适用于存在ASCII之外字符（如中文）的字符串，读取时可以自动确定读取长度

        // 检测文件尾
        try {
            try (DataInputStream dis2 = new DataInputStream(fis1)) {
                while (true) {
                    System.out.println(dis2.readInt());
                }
            }
        } catch (EOFException e) {  // 用EOFException检测文件尾
            // handle exception
        }

        // 带缓冲区的IO类，方法同FileInputStream/FileOutputStream一样，全部继承自InputStream/OutputStream
        BufferedInputStream bis1 = new BufferedInputStream(fis1);       // 用InputStream对象创建BufferedInputStream对象（默认缓存大小512字节）
        BufferedInputStream bis2 = new BufferedInputStream(fis1, 1024); // 用InputStream对象创建BufferedInputStream对象（缓存大小1024字节）
        BufferedOutputStream bos1 = new BufferedOutputStream(fos1);     // 用OutputStream对象创建BufferedOutputStream对象（默认缓存大小512字节）
        BufferedOutputStream bos2 = new BufferedOutputStream(fos1, 1024);// 用OutputStream对象创建BufferedOutputStream对象（缓存大小1024字节）
        bis1.close();
        bis2.close();
        bos1.close();
        bos2.close();

        // 对象IO类，可写入的对象必须实现java.io.Serializable接口，但该接口并不需要多实现什么方法
        ObjectInputStream ois1 = new ObjectInputStream(bis1);   // 用InputStream对象创建ObjectInputStream对象
        ObjectOutputStream oos1 = new ObjectOutputStream(bos1); // 用OutputStream对象创建ObjectOutputStream对象

        oos1.writeObject(new java.util.Date());
        try {
            java.util.Date date1 = (java.util.Date)(ois1.readObject());
        } catch (ClassNotFoundException e) {
            // ...
        }

        // 随机位置IO类RandomAccessFile
        RandomAccessFile raf1 = new RandomAccessFile("null.dat", "rw"); // 用文件名创建RandomAccessFile对象，指定可读可写模式
        RandomAccessFile raf2 = new RandomAccessFile(file1, "r");       // 用文件对象创建RandomAccessFile对象，指定可读模式

        raf1.setLength(0);          // 设置文件大小为0字节（清空原文件内容）
        raf1.seek(0);               // 文件指针移动到文件开头
        raf1.writeInt(42);
        raf1.writeChar('D');
        raf1.seek(Integer.BYTES);
        raf1.readChar();
        raf1.length();              // 获取文件大小（字节）
    }

    void fun24() {
        // 日历日期
        java.util.Calendar calendar = new java.util.GregorianCalendar(2002, 00, 14);    // GregorianCalendar公历，2002-01-14，月份以0为开始

        // 日历域部分常量
        int value = java.util.Calendar.YEAR;        // 年
        value = java.util.Calendar.MONTH;           // 月（0表示一月）
        value = java.util.Calendar.DATE;            // 日
        value = java.util.Calendar.HOUR;            // 小时（12小时制）
        value = java.util.Calendar.HOUR_OF_DAY;     // 小时（24小时制）
        value = java.util.Calendar.MINUTE;          // 分钟
        value = java.util.Calendar.SECOND;          // 秒
        value = java.util.Calendar.DAY_OF_WEEK;     // 一周内第几天（1-7表示星期日-星期六）
        value = java.util.Calendar.DAY_OF_MONTH;    // 一月内第几天，等于DATE
        value = java.util.Calendar.DAY_OF_YEAR;     // 一年内第几天（1为第一天）
        value = java.util.Calendar.WEEK_OF_MONTH;   // 一月内第几周（1为第一周）
        value = java.util.Calendar.WEEK_OF_YEAR;    // 一年内第几周（1为第一周）
        value = java.util.Calendar.AM_PM;           // 上午还是下午（0-上午，1-下午）

        calendar.get(java.util.Calendar.DATE);      // 返回一个给定日历域（年月日小时等上述常量）的值
        calendar.set(java.util.Calendar.DATE, 3);   // 给指定日历域设指定值
        calendar.set(2001, 10, 12, 23, 9, 2);       // 设置指定日历
        calendar.getActualMaximum(java.util.Calendar.DATE);// 获取该日历对象某日历域的最大值
        calendar.add(java.util.Calendar.DATE, 2);   // 给指定日历域增加指定数量时间
        calendar.getTime();                         // 获取该日历对应的Date日期对象
        calendar.setTime(new java.util.Date());     // 使用指定Date日期对象设置日历的时间
    }

    // 迭代器
    // 迭代器仅适用于集合类（Collection，包括List、Set、Queue，如Vector、ArrayList、HashSet），
    // List 接口实例存储的是有序的，可以重复的元素，可以动态增长，查找元素效率高，插入删除效率低，插入和删除会引起其他元素位置改变
    // Set 接口实例存储的是无序的，不重复的数据，可以动态增长，查找元素效率低，插入删除效率高，插入和删除不会引起元素位置改变 
    void fun25() {
        ArrayList<String> als = new ArrayList<>();
        java.util.Iterator<String> its = als.iterator();
        its.hasNext();  // 是否有下一个元素
        its.next();     // 返回下一个元素
        its.remove();   // 移除下一个元素
        // 可以认为iterator所指的位置在元素与元素之间的夹缝，一开始iterator则处在第0个元素前，此时所有“下一个元素”都是指第0个元素
    }

    // 异常处理
    // 如果发生的错误需要调用者处理，则方法应当抛出异常。如果发生的错误简单、可预见，则应局部处理掉而不抛出异常。如果发生的异常本方法中可以处理，则不要抛出异常
    void fun26() {
        Scanner input = new Scanner(System.in);
        int number1 = input.nextInt();
        int number2 = input.nextInt();
        
        // 方法1：主动终止，不推荐，应当让调用者决定是否直接退出程序
        System.out.println(quotient1(number1, number2));
        // 方法2：try-catch异常
        try {                                   // try块内执行正常语句
            System.out.println(quotient2(number1, number2));
        } catch (ArithmeticException e) {       // catch块指定可以捕获的异常类型，执行异常处理
            System.out.println(e.getMessage());
        } catch (Exception ee) {                // catch块要按照异常类型从子类到父类顺序出现，因为父类的catch块可以捕获所有子类异常，否则永远不会走到子类catch块（当然不一定出现直接父类的catch块）
            System.out.println(ee.getMessage());
        } finally {                             // finally块一定会执行，即使try或者catch中出现了return语句。如果出现了finally块，则可省略catch块
            input.close();
        }

        // 抛出必检异常的方法必须用try-catch块处理，且抛出的异常必须都要处理
        try {
            fun27(1);
        } catch (IOException ei){
            System.out.println(ei.getMessage());
        } catch (ClassNotFoundException ec) {
            System.out.println(ec.getMessage());
        }

        // JDK7加入的多捕获特性，多个异常处理方式相同时可以这样简化写，但是如果有继承关系的类会不让写子类，比如 IOException | Exception 中IOException报错
        try {
            fun27(1);
        } catch (IOException | ClassNotFoundException e){
            System.out.println(e.getMessage()); // 描述该对象的信息（构造函数传入的String）
            e.toString();                       // 返回 类名: 描述该对象的信息
            e.printStackTrace();                // 控制台上打印对象及调用堆栈信息（约等于 toString + getStackTace）
            e.getStackTrace();                  // 获取该对象相关的堆栈跟踪元素的数组，堆栈跟踪元素中存储了抛出异常的方法、所在类、行号等信息
        }

        // 异常无法再本函数内处理，或者希望调用者关注到这个异常，那么可以重新抛出这个异常
        try {
            System.out.println(quotient2(number1, number2));
        } catch (Exception e) {
            // ... 做些退出方法之前要做的事
            throw e;                            // 重新抛出异常给调用者
        }
        
        // 链式异常，抛出的新异常附带上原始异常的信息
        try {
            chainExceptionMethod1();
        } catch (Exception e) {
            e.printStackTrace();                // 打印chainExceptionMethod1中创建的链式异常
        }
        /*
        * 从异常根类（Throwable）分出的异常类型：
        *   系统错误（Error）：Java虚拟机抛出，表示内部系统错误，极少发生，但是发生了基本只能终止程序，例如
        *     链接错误（LinkageError）：A依赖B，但是A编译后B进行了改动而不兼容
        *     虚拟机错误（VirtualMachineError）：Java虚拟机崩溃，或者运行所需资源耗尽
        *   异常（Exception）：表示程序或外部环境引起的错误，可以由程序捕获处理，例如
        *     类未找到异常（ClassNotFoundException）：试图使用一个不存在的类
        *     输入输出异常（IOException）：如无效输入、读文件超过文件尾、打开不存在的文件等。子类有InterruptedlOException、EOFException、FileNotFoundException
        *     运行时异常（RuntimeException）：程序设计错误，例如
        *       算数异常（ArithmeticException）：整数除以0、数字溢出等
        *       空指针异常（NullPointerException）：null引用变量访问对象
        *       索引超边界异常（IndexOutOfBoundsException）：数组下标超出范围
        *       非法参数异常（IllegalArgumentException）：传给方法的参数非法或者不合适
        * RuntimeException、Error及它们的子类称为免检异常，Java不强制要求编写代码捕获或声明免检异常。其他异常称为必检异常，指编译器强制程序员检查并使用try-catch块处理这些异常
        */
    }
    
    // 方法1：主动终止，不推荐，应当让调用者决定是否直接退出程序
    int quotient1(int number1, int number2) {
        if (number2 == 0) {
            System.out.println("Divisor cannot be zero");
            System.exit(1);         // 直接退出程序
        }
        return number1 / number2;
    }
    // 方法2：try-catch异常
    int quotient2(int number1, int number2) {
        if (number2 == 0) {
            throw new ArithmeticException("Divisor cannot be zero");// 抛异常
        }
        return number1 / number2;
    }
    
    public static void chainExceptionMethod1() throws Exception {
        try {
            chainExceptionMethod2();
        } catch (Exception e) {
            throw new Exception("New from method1", e); // 创建一个链式异常。如果不带参数e，则fun20中只会打印该方法的异常信息，
                                                        // 不会打印chainExceptionMethod2中的异常（因为捕获到的异常对象e未使用）
        }
    }

    public static void chainExceptionMethod2() throws Exception {
        throw new Exception("New from method2");
    }

    // 必检异常必须在方法头声明异常，免检异常不需要
    // 如果一个方法在父类没声明异常，则子类不能通过重写来声明异常
    void fun27(int i) throws ClassNotFoundException, IOException {  // 声明异常（关键字throws，多个异常中间用逗号分隔）
        if (i == 1) {
            throw new ClassNotFoundException();
        }
        throw new IOException();
    }

    // lambda表达式
    // 形式 (type1 param1, type2 param2, ...) -> expression 或者 (type1 param1, type2 param2, ...) -> { statements; }
    // 形参可以省略类型声明；形参只有一个时可以省略圆括号；不需要形参时可省略形参，但此时需要保留圆括号
    // 方法体如果主体只有一个表达式则编译器会自动推断返回值，大括号则需要语句块内指定返回值
    // 只适用于只含有一个方法的接口（也称为功能接口或者单抽象方法接口）
    void fun28() {
        // 相当于声明了一个实现了L1接口的类即一个该类实例，并将其中唯一的m方法实现为System.out.println("null"); ，使用声明的匿名实例调用lambdaFun1
        lambdaFun1(() -> System.out.println("null"));

        // 相当于声明了一个实现了L2接口的类，并将其中唯一的m方法实现为System.out.println(L); ，以及调用了m方法
        lambdaFun2(L -> System.out.println(L));

        // 相当于声明了一个实现了L3接口的类，并将其中唯一的m方法实现为{ return d + d2; } ，以及调用了m方法
        lambdaFun3((d, d2) -> { return d + d2; });
    }

    void lambdaFun1(LambdaInterface1 l1) {
        l1.m();
    }

    void lambdaFun2(LambdaInterface2 l2) {
        l2.m(3);
    }

    void lambdaFun3(LambdaInterface3 l3) {
        l3.m(3, 5);
    }

    // 枚举
    // 枚举类型命名类似一个类，类型值命名类似常量，枚举类型变量是引用变量
    // 枚举是Object类和Comparable接口、Serializable接口子类，继承了Object类所有方法和Comparable接口的compareTo方法
    // 类内枚举编译为 IntroductionToJavaBasic$MyDay.class，类外枚举编译为 MyColor.class
    // 类内枚举需要声明为静态成员，而不能声明在方法中，static关键字可省略
    /*static*/ enum MyDay {MONDAY, FRIDAY};
    void fun29() {
        MyColor color = null;
        color = MyColor.RED;

        color.compareTo(MyColor.GREEN); // 枚举值之间的序号差
        color.name();                   // 枚举值对应的名字
        color.ordinal();                // 枚举值对应的序号
        MyColor.values();               // 枚举类所有枚举值
        MyColor.valueOf("RED");         // 字符串对应的枚举常量，该枚举常量必须存在
        color.colorInfo();              // 枚举自定义方法

        switch (color) {
            case GREEN:
            break;
            case BLUE:
            break;
            case RED:
            break;
            default:
        }
        for (MyColor m : MyColor.values()) {}
    }
}

// -------- 自定义异常 ----------
// 自定义异常类，可以继承自任意异常类，但是如果自定义了最好还是继承自必检异常比如Exception等，而不是RuntimeException等免检异常
class InvalidRadiusException extends Exception {
    private double radius;
    public InvalidRadiusException(double radius) {
        super("Invalid radius" + radius);
        this.radius = radius;
    }
    public double getRadius() {
        return radius;
    }
}

class CircleWithCustomException {
    private double radius;
    public CircleWithCustomException() throws InvalidRadiusException {      // 使用了自定义必检异常，所有抛异常的方法都要声明抛出异常
        this(1.0);
    }
    public CircleWithCustomException(double newRadius) throws InvalidRadiusException {
        setRadius(newRadius);
    }
    public double getRadius() {
        return radius;
    }
    public void setRadius(double newRadius) throws InvalidRadiusException {
        if (newRadius >= 0) {
            radius = newRadius;
        } else {
            throw new InvalidRadiusException(newRadius);
        }
    }
}

class SerializableClass implements java.io.Serializable {
    private int v1;
    private static double v2;               // 静态变量不会序列化
    private transient UnerializableClass v3;// 未声明实现java.io.Serializable的类无法序列化，要使SerializableClass可序列化需要加transient关键字忽略掉
}

class UnerializableClass {}

interface LambdaInterface1 {
    void m();
}

interface LambdaInterface2 {
    void m(double d);
}

interface LambdaInterface3 {
    double m(double d, double d2);
}

enum MyColor {
    RED, GREEN, BLUE;

    // 枚举类可以有自己的构造函数、方法，但是构造函数必须private
    private MyColor() {
        System.out.println("Constructor called for : " + this.toString());
    }

    public void colorInfo()
    {
        System.out.println("Universal Color");
    }
};