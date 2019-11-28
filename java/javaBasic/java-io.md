# IO

[链接1](https://www.cnblogs.com/runningTurtle/p/7088125.html)

[链接2](https://zhuanlan.zhihu.com/p/25418336)

![](E:\notes\java\javaBasic\img\io1.png)

![](img/io2.png)

## 流的概念和作用

- Java IO 也称为IO流，IO = 流，它的核心就是对文件的操作，对于 字节 、字符类型的输入和输出流。
- IO流的本质是数据传输，并且流是单向的。

流是一组有顺序的，有起点和终点的字节集合，是对数据传输的总称或抽象。即数据在两设备间的传输称为流，**流的本质是数据传输，并且流是单向的。根据数据传输特性将流抽象为各种类，方便更直观的进行数据操作。** 

## IO流的分类

- 根据处理数据类型的不同分为：字符流和字节流
- 根据数据流向不同分为：输入流和输出流

### 字符流和字节流

字符流的由来： <font color="#00dd00">因为数据编码的不同</font>，而有了对字符进行高效操作的流对象。本质其实就是基于字节流读取时，去查了指定的码表。 字节流和字符流的区别：

- 读写单位不同：字节流以字节（8bit）为单位，字符流以字符为单位，根据码表映射字符，一次可能读多个字节。
- 处理对象不同：字节流能处理所有类型的数据（如图片、avi等），而字符流只能处理字符类型的数据。

结论：只要是处理纯文本数据，就优先考虑使用字符流。 除此之外都使用字节流。

### 输入流和输出流

输入流和输出流是以程序为参照物，输入到程序，或是从程序输出。

- 输入流：从文件读入到程序。只能进行读操作。
- 输出流：从程序输出到文件。只能进行写操作。

![](img/java-io3.png)

### 节点流和处理流：

- 节点流：直接与数据源相连，读入或读出。
- 处理流：与节点流一块使用，在节点流的基础上，再套接一层，套接在节点流上的就是处理流。处理流的构造方法总是要带一个其他的流对象做参数。一个流对象经过其他流的多次包装，称为流的链接。

## IO流对象

### 输入字节流 InputStream

从IO中输入字节流的继承图中可以看出。

1）InputStream是所有数据字节流的父类，它是一个抽象类。

2）ByteArrayInputStream、StringBufferInputStream、FileInputStream是三种基本的介质流，它们分别从Byte数组、StringBuffer、和本地文件中读取数据，PipedInputStream是从与其他线程共用的管道中读取数据。

3）ObjectInputStream和所有FileInputStream 的子类都是装饰流（装饰器模式的主角）。



### 输出字节流 OutputStream

从IO中输入字节流的继承图中可以看出。

1）OutputStream是所有输出字节流的父类，它是一个抽象类。

2）ByteArrayOutputStream、FIleOutputStream是两种基本的介质，它们分别向Byte 数组，和本地文件中写入数据。PipedOutputStream是从与其他线程共用的管道中写入数据。

3）ObjectOutputStream和所有FileOutputStream的子类都是装饰流。

![](img/io3.jpg)

图中蓝色为主要对应部分，红色为不对应部分，黑色的虚线部分代表这些流一般需要搭配使用。从上面的图中可以看出Java IO中的字节流是非常对称的。我们来看看这些字节流中不对称的几个类。

1. LineNumberInputStream 主要完成从流中读取数据时，会得到相应的行号，至于什么时候分行、在哪里分行是由改类主动确定的，并不是在原始中有这样一个行号。在输出部分没有对应的部分，我们完全可以自己建立一个LineNumberOutputStream，在最初写入时会有一个基准的行号，以后每次遇到换行时会在下一行添加一个行号，看起来也是可以的。好像更不入流了。
2. PushbackInputStream 的功能是查看最后一个字节，不满意就放入缓冲区。主要用在编译器的语法、词法分析部分。输出部分的BufferedOutputStream 几乎实现相近的功能。
3. StringBufferInputStream 已经被Deprecated，本身就不应该出现在InputStream 部分，主要因为String 应该属于字符流的范围。已经被废弃了，当然输出部分也没有必要需要它了！还允许它存在只是为了保持版本的向下兼容而已。
4. SequenceInputStream 可以认为是一个工具类，将两个或者多个输入流当成一个输入流依次读取。完全可以从IO 包中去除，还完全不影响IO 包的结构，却让其更“纯洁”――纯洁的Decorator 模式。
5. PrintStream 也可以认为是一个辅助工具。主要可以向其他输出流，或者FileInputStream 写入数据，本身内部实现还是带缓冲的。本质上是对其它流的综合运用的一个工具而已。一样可以踢出IO 包！System.out 和System.out 就是PrintStream 的实例。

### 字符输入流Reader

在上面的继承关系图中可以看出：

1、Reader是所有的输入字符流的父类，它是一个抽象类。

2、CharReader、StringReader 是两种基本的介质流，它们分别将Char数组、String中读取数据。PipedInputReader 是从与其他线程共用的管道中读取数据。

3、BuffereReader 很明显的是一个装饰器，它和其子类复制装饰其他Reader对象。

4、FilterReader 是所有自定义具体装饰流的父类，其子类PushbackReader 对Reader 对象进行装饰，回增加一个行号。

5、InputStreamReader 是一个连接字节流和字符流的桥梁，它将字节流转变为字符流。FileReader 可以说是一个达到此功能、常用的工具类，在其源代码中明显使用了将FileInputStream转变为Reader 的方法。我们可以从这个类中得到一定的技巧。Reader 中各个类的用途和使用方法基本和InputStream 中的类使用一致。



### 字符输出流Writer

在上面的关系图中可以看出：

1、Writer 是所有输出字符流的父类，它是一个抽象类。

2、CharArrayWriter、StringWriter 是两种基本的介质流，它们分别向Char 数组、String 中写入数据。PipedInputWriter 是从与其他线程共用的管道中读取数据。



3、BuffereWriter 很明显是一个装饰器，他和其子类复制装饰其他Reader对象。

4、FilterWriter 和PrintStream 及其类似，功能和使用也非常相似。

5、OutputStreamWriter 是OutputStream 到Writer 转换到桥梁，它的子类FileWriter 其实就是一个实现此功能的具体类（具体可以研究一下SourceCode）。功能和使用OutputStream极其类似。



字符流的输入与输出对应图

![](img/io4.jpg)

### 字符流与字节流的转换

转换流的特点：

1、其是字符流和字节流之间的桥梁。

2、可对读取到的字节数据经过指定编码转换成字符。

3、可对读取到的字符数据经过指定编码转换成字节。



何时使用转换流？

1、当字节和字符之间有转换动作时。

2、流操作的数据需要编码或解码时。



具体的对象体现：

1、InputStreamReader：字节到字符的桥梁。

2、OutputStreamWriter：字符到字节的桥梁。

这两个流对象时字符体系中的成员，它们有转换的作用，本身又是字符流，所以在构造的时候需要传入字节流对象进来。

### File类

File类是对文件系统中文件以及文件夹进行封装的对象，可以通过对象的思想来操作文件和文件夹。File类保存文件或目录的各种数据信息，包括文件名、文件长度、最后修改时间、是否可读、获取当前文件的路径名、判断文件是否存在、获取当前目录中的文件列表、创建、删除文件和目录等方法。



### RandomAccessFile类

该对象不是流体系中的一员，其封装了字节流，同时还封装了一个缓冲区（字符数组），通过内部的指针来操作字符数组中的数据。该对象特点：

1、该对象只能操作文件，所以构造函数接受两种数据类型的参数：

· 字符串文件路径

· File对象

2、该对象既可以对文件进行读操作，也能进行写操作，在进行对象实例化时可指定操作模式（r , rw）。

注意：**该对象在实例化时，如果要操作的文件不存在，会自动创建；如果文件存在，写数据未指定位置，会从头开始写，即覆盖原有的内容。**可以用于多线程下载或多个线程同时写数据到文件。



## 各IO类关系梳理

- InputStream: Java IO中的顶级的字节输入流的抽象类，定义了最基础的输入、读取的相关方法。实现了Closeable

  接口。

  - FileInputStream: 继承自`InputStream`的文件输入流类，用于从本地文件中读取字节数据。

  - ByteArrayInputStream: 继承自`InputStream`的字节数组输入流类，它包含一个内部缓冲区，该缓冲区包含从流中读取的字节；通俗点说，它的内部缓冲区就是一个字节数组，而 ByteArrayInputStream 本质就是通过字节数组来实现的。InputStream通过`read()`向外提供接口，供它们来读取字节数据；而 ByteArrayInputStream 的内部额外的定义了一个计数器，它被用来跟踪`read()`方法要读取的下一个字节。

  - StringBufferInputStream: 继承自`InputStream`的字节输入流类，其中读取的字节由字符串的内容提供的输入流。该类已过时，不推荐使用，由 StringReader 代替。

  - PipedInputStream: 继承自`InputStream`的管道输入流类，在使用管道通信时，必须与 PipedOutputStream 配合使用。让多线程可以通过管道进行线程间的通讯。

  - ObjectInputStream: 继承自`InputStream`的对象输入流类，实现了 ObjectInput 和 ObjectStreamConstants 接口。作用是从输入流中读取Java对象和基本数据。只有支持 Serializable 或 Externalizable 接口的对象才能被`ObjectInputStream/ObjectOutputStream`所操作！

  - SequenceInputStream: 继承自`InputStream`的输入合并流类。SequenceInputStream 会将与之相连接的流集组合成一个输入流并从第一个输入流开始读取，直到到达文件末尾，接着从第二个输入流读取，依次类推，直到到达包含的最后一个输入流的文件末 尾为止。合并流的作用是将多个源合并合一个源。

  - AudioInputStream: 继承自`InputStream`的音频输入流类。音频输入流是具有指定音频格式和长度的输入流。长度用示例帧表示，不用字节表示。提供几种方法，用于从流读取一定数量的字节，或未指定数量的字节。音频输入流跟踪所读取的最后一个字节。可以跳过任意数量的字节以到达稍后的读取位置。音频输入流可支持标记。设置标记时，会记住当前位置，以便可以稍后返回到该位置。

  - FilterInputStream: 继承自InputStream的过滤输入流类（装饰器超类），是用来“封装其它的输入流，并为它们提供额外的功能”。

    - BufferedInputStream: 继承自`FilterInputStream`的带缓冲区功能的输入流类（装饰器子类），默认缓冲区大小是8K，能够减少访问磁盘的次数，提高文件读取性能。

    - DataInputStream: 继承自`FilterInputStream`的数据输入流类，实现了 DataInput 接口。它允许应用程序以与机器无关方式从底层输入流中读取基本 Java 数据类型。

    - PushbackInputStream: 继承自`FilterInputStream`的回退输入流类。允许试探性的读取数据流，如果不是我们想要的则返还回去。

    - LineNumberInputStream: 继承自`FilterInputStream`的行号输入流类。可以获取当前的行号或设置当前行号，已过时，已经被 LineNumberReader 替代。

    - CheckedInputStream: 继承自`FilterInputStream`的校验输入流类。

    - CipherInputStream: 继承自`FilterInputStream`的密钥输入流类。

    - DigestInputStream: 继承自`FilterInputStream`的摘要处理输入流类。

    - InflaterInputStream: 继承自FilterInputStream

      的解压缩处理输入流类。

      - GZIPInputStream: 继承自`InflaterInputStream`的gzip文件处理输入流类。

      - ZipInputStream: 继承自InflaterInputStream

        的解压缩处理输入流类。

        - JarInputStream: 继承自`ZipInputStream`的解压缩处理输入流类。

    - DeflaterInputStream: 继承自`FilterInputStream`的压缩数据输入流类。

    - ProgressMonitorInputStream: 继承自`FilterInputStream`的进度监控输入流类。

- OutputStream: JavaIO中的顶级的字节输出流的抽象类，定义了最基础的输出、写入的相关方法。实现了

  Closeable和Flushable接口。

  - FileOutputStream: 继承自`OutputStream`的文件输出流类，用于向本地文件中写入字节数据。

  - ByteArrayOutputStream: 继承自`OutputStream`的字节数组输出流类，ByteArrayOutputStream 中的数据会被写入一个 byte 数组。缓冲区会随着数据的不断写入而自动增长。可使用 toByteArray() 和 toString() 获取数据。

  - PipedOutputStream: 继承自`OutputStream`的管道输出流类，在使用管道通信时，必须与 PipedInputStream 配合使用。让多线程可以通过管道进行线程间的通讯。

  - ObjectOutputStream: 继承自`OutputStream`的对象输出流类，实现了 ObjectOutput 和 ObjectStreamConstants 接口。作用是把Java对象和基本数据写入到对象输出流中。只有支持 Serializable 或 Externalizable 接口的对象才能被`ObjectInputStream/ObjectOutputStream`所操作！

  - FilterOutputStream: 继承自OutputStream

    的过滤输出流类，是用来“封装其它的输出流，并为它们提供额外的功能”。

    - BufferedOutputStream: 继承自`FilterOutputStream`的带缓冲区功能的输出流类，默认缓冲区大小是8K，能够提高文件的写入效率。

    - DataOutputStream: 继承自`FilterOutputStream`的数据输出流类，实现了 DataOutput 接口。它允许应用程序以与机器无关方式向底层输入流中写入基本 Java 数据类型。

    - PrintStream: 继承自`FilterOutputStream`的打印输出流类，实现了 Appendable 和 Closeable 接口。使它们能够方便地打印各种数据值表示形式。PrintStream 永远不会抛出 IOException。PrintStream 提供了自动flush 和 字符集设置功能。所谓自动flush，就是往PrintStream写入的数据会立刻调用flush()函数。

    - CheckedOutputStream: 继承自`FilterOutputStream`的校验输出流类。

    - CipherOutputStream: 继承自`FilterOutputStream`的密钥输出流类。

    - DigestOutputStream: 继承自`FilterOutputStream`的摘要处理输出流类。

    - InflaterOutputStream: 继承自`FilterOutputStream`的解压缩处理输出流类。

    - DeflaterOutputStream: 继承自FilterOutputStream

      的解压缩数据输出流类。

      - GZIPOutputStream: 继承自`DeflaterOutputStream`的gzip文件解压缩输出流类。
      - ZipOutputStream: 继承自DeflaterOutputStream的zip文件解压缩输出流类。
        - JarOutputStream: 继承自`ZipOutputStream`的zip文件解压缩输出流类。

- Reader: JavaIO中的顶级的字符读取的抽象类，定义了最基础的读取方法。实现了 Readable 和 Closeable 接口。

  - BufferedReader: 继承自Reader

    的带缓冲功能的字符流类，默认缓冲区大小是8K，从字符输入流中读取文本，缓冲各个字符，从而实现字符、数组和行的高效读取。创建 BufferReader 时，我们会通过它的构造函数指定某个 Reader 为参数。BufferReader 会将该 Reader 中的数据分批读取，每次读取一部分到缓冲中；操作完缓冲中的这部分数据之后，再从 Reader 中读取下一部分的数据。

    - LineNumberReader: 继承自`BufferedReader`，可以获取当前的行号或设置当前行号。

  - InputStreamReader: 继承自Reader，用于将从字节流转换成字符流的 Reader。是字节流通向字符流的桥梁。如果不指定字符集编码，该解码过程将使用平台默认的字符编码。

    - FileReader: 继承自`InputStreamReader`，用来读取字符文件的便捷类。

  - StringReader: 继承自`Reader`，用于字符串读取的字符流。

  - CharArrayReader: 继承自`Reader`的字符数组输入流类。

  - FilterReader: 继承自Reader的字符过滤输入流抽象类。

    - PushbackReader: 继承自`FilterReader`的字符回退输入流类。

  - PipedReader: 继承自`Reader`的字符管道输入流类。作用是可以通过管道进行线程间的通讯。必须和 PipedWriter 配合使用。

- Writer: JavaIO中的顶级的字符写入的抽象类，定义了最基础的写入方法。实现了 Appendable 、 Closeable 和 Flushable 接口。

  - BufferedWriter: 继承自`Writer`的带缓冲功能的字符流类，默认缓冲区大小是8K，从字符输出流中写入字符到文本中，缓冲各个字符，从而实现字符、数组和行的高效写入。
  - OutputStreamWriter: 继承自Reader，用于将从字节流转换成字符流的 Writer。是字节流通向字符流的桥梁。如果不指定字符集编码，该解码过程将使用平台默认的字符编码。
    - FileWriter: 继承自`OutputStreamWriter`，用来向文件中写入字符的便捷类。
  - FilterWriter: 继承自`Writer`的字符过滤输出流抽象类。与 FilterOutputStream 功能一样、只是简单重写了父类的方法、目的是为所有装饰类提供标准和基本的方法、要求子类必须实现核心方法、和拥有自己的特色。这里 FilterWriter 没有子类、可能其意义只是提供一个接口、留着以后的扩展，本身是一个抽象类。
  - StringWriter: 继承自`Writer`，用于字符串写入的字符流。
  - PipedWriter: 继承自`Writer`的字符管道输出流类。作用是可以通过管道进行线程间的通讯。必须和 PipedReader 配合使用。
  - CharArrayWriter: 继承自`Writer`的字符数组输出流类。
  - PrintWriter: 继承自`Writer`的打印写入类，提供了PrintStream的所有打印方法，其方法也从不抛出IOException。与PrintStream的区别：作为处理流使用时，PrintStream只能封装OutputStream类型的字节流，而PrintWriter既可以封装OutputStream类型的字节流，还能够封装Writer类型的字符输出流并增强其功能。



![](img/java-io1.png)



![](img/java-io2.png)



## 流中主要方法梳理

### InputStream中的主要方法

- `abstract int read()`: 从流中读取数据，读取一个字节，返回值为所读得字节
- `int read(byte b[])`: 从流中读取数据，读取多个字节，放置到字节数组 b 中，通常读取的字节数量为 b 的长度，返回值为实际独取的字节的数量。
- `int read(byte b[], int off, int len)`: 从流中读取数据，读取 len 个字节，放置到以下标 off 开始字节数组 b 中，返回值为实际读取的字节的数量。
- `long skip(long n)`: 读指针跳过n个字节不读，返回值为实际跳过的字节数量。
- `int available()`: 返回值为流中尚未读取的字节的数量。
- `void close()`: 关闭输入流。
- `synchronized void mark(int readlimit)`: 记录当前指针的所在位置，`readlimit`表示读指针读出的`readlimit`个字节后，所标记的指针位置才实效。
- `synchronized void reset()`: 把读指针重新指向用 mark 方法所记录的位置。
- `boolean markSupported()`: 当前的流是否支持读指针的记录功能。

> **注**：其中`read()`返回的是读入的一个字节所对应的 int 值(0-255),而`read(byte[] b)`和`read(byte[] b, int off, int len)`返回的是读入的字节数。

### OutputStream中的主要方法

- `abstract void write(int b)`: 输出数据，往流中写一个字节 b。
- `void write(byte b[])`: 输出数据，往流中写一个字节数组 b。
- `void write(byte b[], int off, int len)`: 输出数据，把字节数组 b 中从下标 off 开始，长度为 len 的字节写入到流中。
- `void flush()`: 刷空输出流，并输出所有被缓存的字节。由于某些流支持缓存功能，该方法将把缓存中所有内容强制输出到流中。
- `void close()`: 关闭输出流。

### Reader中的主要方法

- `int read(java.nio.CharBuffer target)`: 试图读取字符入指定的字符缓冲区。
- `int read()`: 读取单个字符。
- `int read(char cbuf[])`: 读取字符到一个数组中。
- `int read(char cbuf[], int off, int len)`: 读取字符到一个数组中的一部分。
- `long skip(long n)`: 跳过n个字符。
- `boolean ready()`: 通知此流是否已准备好被读取。
- `boolean markSupported()`: 告诉此流是否支持`mark()`操作。
- `void mark(int readAheadLimit)`: 标记流中的当前位置。
- `void reset()`: 重置流。
- `void close()`: 关闭该流并释放与之关联的所有系统资源。

### Writer中的主要方法

- `void write(int c)`: 写入单个字符。
- `void write(char cbuf[])`: 写入字符数组。
- `abstract void write(char cbuf[], int off, int len)`: 写入字符数组的一部分。
- `void write(String str)`: 写入一个字符串。
- `void write(String str, int off, int len)`: 写入一个字符串的一部分。
- `Writer append(CharSequence csq)`: 将指定的字符序列追加写到writer中。
- `Writer append(CharSequence csq, int start, int end)`: 将指定的字符序列的子序列追加写入此writer。
- `Writer append(char c)`: 将指定字符追加到这个writer。
- `abstract void flush()`: 刷新流。
- `abstract void close()`: 关闭流，但要先刷新它



## 使用示例



```java
package com.juc.demo.io;

import org.junit.Test;

import java.io.*;
import java.nio.charset.StandardCharsets;

public class IoTest {

    /**
     * ByteArrayInputStream:
     * 1.构造函数中需要传入一个byte数组作为数据源，当执行read操作时，就会从该数组中读取数据;
     * 2.如果在构造函数中传入了null作为字节数据，那么在执行read操作时就会出现NullPointerException异常，
     * 但是在构造函数初始化阶段不会抛出异常
     * <p>
     * ByteArrayOutputStream:
     * 1.其内部也有一个字节数组用于存储write操作时写入的数据，在构造函数中可以传入一个size指定
     * 其内部的byte数组的大小，如果不指定，那么默认它会将byte数组初始化为32字节，
     * 当持续通过write向ByteArrayOutputStream中写入数据时，如果其内部的byte数组的剩余空间
     * 不能够存储需要写入的数据，那么那么它会通过调用内部的ensureCapacity()方法对其内部
     * 维护的byte数组进行扩容以存储所有要写入的数据，所以不必担心其内部的byte数组太小
     * 导致的IndexOutOfBoundsException之类的异常。
     */
    @Test
    public void testByteArrayInputOutputStream() {

        String str = "I am String!";
        byte[] bytes = str.getBytes();

        ByteArrayInputStream bais = new ByteArrayInputStream(bytes);
        ByteArrayOutputStream baos = new ByteArrayOutputStream();

        byte[] buf = new byte[1024];
        int len = 0;
        try {
            while ((len = bais.read(buf)) > 0) {
                baos.write(buf, 0, len);
            }
            System.out.println(baos.toString("UTF-8"));
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
    }

    /**
     * FileInputStream 能够将文件作为数据源，读取文件中的流，通过File对象或文件路径等初始化
     *
     * @throws IOException
     */
    @Test
    public void testFileInputOutputStream() throws IOException {
        String inputFileName = "E:/notes/java/javaBasic/java-Enum.md";
        String outputFileName = "E:/temp/file.md";
        FileInputStream fis = null;
        FileOutputStream fos = null;
        try {
            fis = new FileInputStream(inputFileName);
            fos = new FileOutputStream(outputFileName);
            byte[] buf = new byte[1024];
            int length = 0;
            while ((length = fis.read(buf)) > 0) {
                fos.write(buf, 0, length);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            fis.close();
            fos.close();
        }
    }

    /**
     * PipedInputStream和PipedOutputStream一般是结合使用的，这两个类用于在两个线程间进行管道通信，
     * 一般在一个线程中执行PipedOutputStream 的write操作，而在另一个线程中执行PipedInputStream的read操作。
     * <p>
     * 可以在构造函数中传入相关的流将PipedInputStream 和PipedOutputStream 绑定起来，
     * 也可以通过二者的connect方法将二者绑定起来，一旦二者进进行了绑定，那么PipedInputStream
     * 的read方法就会自动读取PipedOutputStream写入的数据。
     * <p>
     * PipedInputStream的read操作是阻塞式的，当执行PipedOutputStream的write操作时，PipedInputStream会在
     * 另一个线程中自动读取PipedOutputStream写入的内容，如果PipedOutputStream一直没有执行write操作写入
     * 数据，那么PipedInputStream的read方法会一直阻塞PipedInputStream的read方法所运行的线程直至读到数据。
     * 单独使用PipedInputStream或单独使用PipedOutputStream时没有任何意义的，必须将二者通过connect方法（或在
     * 构造函数中传入对应的流）进行连接绑定，如果单独使用其中的某一个类，就会触发IOException: Pipe Not Connected.
     */
    @Test
    public void testPipeInputOutputStream() {

        PipedOutputStream pos = new PipedOutputStream();
        PipedInputStream pis = new PipedInputStream();

        Thread writerThread = new Thread(() -> {
            String msg = "hello world.";
            System.out.println("writer thread send msg");
            try {
                pos.write(msg.getBytes());
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    pos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        Thread readerThread = new Thread(() -> {
            byte[] buf = new byte[1024];

            System.out.println("reader thread revceive msg");
            try {
                int len = pis.read(buf);
                String message = new String(buf, 0, len, StandardCharsets.UTF_8);
                System.out.println(message);
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    pis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        // 通过pos.connect(pis)将这两种流绑定在一起
        try {
            pos.connect(pis);
        } catch (IOException e) {
            e.printStackTrace();
        }
        readerThread.start();
        writerThread.start();
    }

    /**
     * 此处Person应该声明为static且实现Serializable接口
     * 如果为非static类型，则下述示例会报错 java.io.NotSerializableException
     * <p>
     * 原因：
     * 非静态内部类拥有对外部类的所有成员的完全访问权限，包括实例字段和方法。为实现这一行为，
     * 非静态内部类存储着对外部类的实例的一个隐式引用。序列化时要求所有的成员变量是Serializable,
     * 现在外部的类并没有implements Serializable,所以就抛出java.io.NotSerializableException异常。
     */
    static class Person implements Serializable {

        private String name;
        private int age;

        public Person() {
        }

        public Person(String name, int age) {
            this.name = name;
            this.age = age;
        }

        public String getName() {
            return name;
        }

        public int getAge() {
            return age;
        }

        public void setName(String name) {
            this.name = name;
        }

        public void setAge(int age) {
            this.age = age;
        }
    }

    /**
     * ObjectOutputStream:
     * 具有一系列writeXXX方法，在其构造函数中可以掺入一个OutputStream，可以方便的向指定的输出流中
     * 写入基本类型数据以及String，比如writeBoolean、writeChar、writeInt、writeLong、writeFloat、
     * writeDouble、writeCharts、writeUTF等，除此之外，ObjectOutputStream还具有writeObject方法。
     * writeObject方法中传入的类型必须实现了Serializable接口，从而在执行writeObject操作时将对象
     * 进行序列化成流，并将其写入指定的输出流中。
     * ObjectInputStream:
     * 与ObjectOutputStream相对应，ObjectInputStream有与OutputStream中的writeXXX系列方法完全对应
     * 的readXXX系列方法，专门用于读取OutputStream通过writeXXX写入的数据。
     */
    @Test
    public void testObjectInputOutputStream() {
        try {
            String fileName = "E:/temp/file.txt";

            //将内存中的对象序列化到物理文件中
            FileOutputStream fos = new FileOutputStream(fileName);
            ObjectOutputStream oos = new ObjectOutputStream(fos);

            String description = "以下是人员数组";

            Person[] persons = new Person[]{
                    new Person("iSpring", 26),
                    new Person("Mr.Sun", 27),
                    new Person("Miss.Zhou", 27)
            };

            oos.writeObject(description);

            oos.writeInt(persons.length);

            for (Person person : persons) {
                oos.writeObject(person);
            }
            oos.close();

            //从物理文件中反序列化读取对象信息
            FileInputStream fis = new FileInputStream(fileName);
            ObjectInputStream ois = new ObjectInputStream(fis);
            String str = (String) ois.readObject();
            System.out.println(str);
            int personCount = ois.readInt();
            for (int i = 0; i < personCount; i++) {
                Person person = (Person) ois.readObject();
                StringBuilder sb = new StringBuilder();
                sb.append("姓名: ").append(person.getName()).append(", 年龄: ").append(person.getAge());
                System.out.println(sb);
            }
        } catch (ClassNotFoundException | IOException e) {
            e.printStackTrace();
        }

    }

    /**
     * SequenceInputStream:
     * 主要是将两个（或多个）InputStream在逻辑上合并为一个InputStream，
     * 比如在构造函数中传入两个InputStream，分别为in1和in2,那么SequenceInputStream在
     * 读取操作时会先读取in1，如果in1读取完毕，就会接着读取in2。
     * <p>
     * 在我们理解了SequenceInputStream 的作用是将两个输入流合并为一个输入流之后，
     * 我们就能理解为什么不存在对应的SequenceOutputStream 类了，因为将一个输出流拆分为多个输出流是没有意义的。
     */
    @Test
    public void testSequenceInputOutputStream() {
        String inputFileName1 = "E:/temp/file.txt";
        String inputFileName2 = "E:/temp/file1.txt";
        String outputFileName = "E:/temp/file2.txt";

        try {
            FileInputStream fis1 = new FileInputStream(inputFileName1);
            FileInputStream fis2 = new FileInputStream(inputFileName2);
            SequenceInputStream sis = new SequenceInputStream(fis1, fis2);
            FileOutputStream fos = new FileOutputStream(outputFileName);

            byte[] buf = new byte[1024];
            int length;
            while ((length = sis.read(buf)) > 0) {
                fos.write(buf, 0, length);
            }

            sis.close();
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * DataInputStream可以让你从InputStream读取Java基本类型来代替原始的字节。
     * DataInputStream来包装InputStream，你就可以从DataInputStream直接以Java基本类型来读取数据。
     * <p>
     * 如果你需要读取的数据是由大于一个字节的java基础类型构成，比如int, long, float, double等，
     * 那么用DataInputStream是很方便的。DataInputStream希望的数据是写入到网络的有序多字节数据。
     */
    @Test
    public void testDataInputStream() {
        DataOutputStream dataOutputStream = null;
        try {
            dataOutputStream = new DataOutputStream(new FileOutputStream("E:/temp/file.txt"));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        try {
            dataOutputStream.writeInt(123);
            dataOutputStream.writeFloat(123.45F);
            dataOutputStream.writeLong(789);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                dataOutputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        DataInputStream dataInputStream = null;
        try {
            dataInputStream = new DataInputStream(new FileInputStream("E:/temp/file.txt"));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        int int123 = 0;
        float float12345 = 0;
        long long789 = 0;
        try {
            int123 = dataInputStream.readInt();
            float12345 = dataInputStream.readFloat();
            long789 = dataInputStream.readLong();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                dataInputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        System.out.println("int123     = " + int123);
        System.out.println("float12345 = " + float12345);
        System.out.println("long789    = " + long789);
    }

    /**
     * 打印流 -> PrintWriter
     */
    @Test
    public void testPrintStream() {
        PrintStream ps = null;        // 声明打印流对象
        // 使用FileOuputStream实例化，意味着所有的输出是向文件之中
        try {
            String inputFileName = "E:/temp/file.txt";
            ps = new PrintStream(new FileOutputStream(inputFileName));
            ps.print("hello ");
            ps.println("world!!!");
            ps.print("1 + 1 = " + 2);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
        }
    }

    /**
     * 回退流：
     * 在JAVA IO中所有的数据都是采用顺序的读取方式，即对于一个输入流来讲都是采用从头到尾的顺序读取的，
     * 如果在输入流中某个不需要的内容被读取进来，则只能通过程序将这些不需要的内容处理掉，为了解决这样
     * 的处理问题，在JAVA中提供了一种回退输入流（PushbackInputStream、PushbackReader），可以把读取进
     * 来的某些数据重新回退到输入流的缓冲区之中。
     * <p>
     * 常用方法
     * 1、public PushbackInputStream(InputStream in) 构造方法 将输入流放入到回退流之中。
     * 2、public int read() throws IOException   普通读取数据。
     * 3、public int read(byte[] b,int off,int len) throws IOException 普通方法 读取指定范围的数据。
     * 4、public void unread(int b) throws IOException 普通方法 回退一个数据到缓冲区前面。
     * 5、public void unread(byte[] b) throws IOException 普通方法 回退一组数据到缓冲区前面。
     * 6、public void unread(byte[] b,int off,int len) throws IOException 普通方法 回退指定范围的一组数据到缓冲区前面。
     */
    @Test
    public void testPushBackInputStream() {
        String str = "www.baidu.com";        // 定义字符串

        PushbackInputStream push = null;        // 定义回退流对象
        ByteArrayInputStream bai = null;        // 定义内存输入流

        bai = new ByteArrayInputStream(str.getBytes());    // 实例化内存输入流
        push = new PushbackInputStream(bai);    // 从内存中读取数据

        System.out.print("读取之后的数据为：");

        int temp = 0;
        while (true) {
            try {
                if ((temp = push.read()) == -1) {
                    break;
                }
            } catch (IOException e) {
                e.printStackTrace();
            }    // 读取内容
            if (temp == '.') {    // 判断是否读取到了“.”
                try {
                    push.unread(temp);    // 放回到缓冲区之中
                    temp = push.read();    // 再读一遍
                } catch (IOException e) {
                    e.printStackTrace();
                }
                System.out.print("（退回" + (char) temp + "）");
            } else {
                System.out.print((char) temp);    // 输出内容
            }
        }
    }

    @Test
    public void tesBufferedInputOutputStream() {
        try {
            String inputFileName = "E:/temp/file.txt";
            String outputFileName = "E:/temp/file1.txt";

            FileInputStream fis = new FileInputStream(inputFileName);
            BufferedInputStream bis = new BufferedInputStream(fis, 1024 * 10);

            FileOutputStream fos = new FileOutputStream(outputFileName);
            BufferedOutputStream bos = new BufferedOutputStream(fos, 1024 * 10);

            byte[] buf = new byte[1024];
            int length = 0;
            while ((length = bis.read(buf)) > 0) {
                bos.write(buf, 0, length);
            }

            bis.close();
            bos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```





## 使用到的设计模式

### 装饰器模式

由于java I/O库需要很多性能的各种组合，如果这些性能都是用继承来实现，那么每一种组合都需要一个类，这样就会造成大量行重复的类出现。如果采用装饰模式，那么类的数目就会大大减少，性能的重复也可以减至最少。因此装饰模式是java I/O库基本模式。装饰模式的引进，造成灵活性和复杂性的提高。因此在使用java IO库时，必须理解java IO库是由一些基本的原始流处理器和围绕它们的装饰流处理器所组成的。

以字节输入流为例。`InputStream`是需要被装饰的抽象构件，而其下的`FileInputStream`、`ByteArrayInputStream`等是被装饰的具体构件；`FilterInputStream`是装饰器的父类，`BufferedInputStream`、`DataInputStream`则是具体的装饰器类。

### 适配器模式

适配器模式是java IO库中第二个重要的设计模式。如：`StringBufferInputStream`就是一个适配器类。它继承了`InputStream`类型，同时持有一个对`String`类型的引用。这是将`String`对象适配成`InputStream`类型的对象形式的适配器模式。`PipedOutputStream`也是一个适配器类。`PipedOutputStream`总是和`PipedInputStream`一起使用，它接收一个类型为`PipedInputStream`的输入类型，并将之转换成`OutputStream`类型的输出流，这是一个对象形式的适配器模式应用。



# BIO/NIO/AIO

BIO、NIO和 AIO 是 Java 语言对操作系统的各种 IO 模型的封装。程序员在使用这些 API 的时候，不需要关心操作系统层面的知识，也不需要根据不同操作系统编写不同的代码。只需要使用Java的API就可以了。

- 同步与异步

```properties
同步： 同步就是发起一个调用后，被调用者未处理完请求之前，调用不返回。

异步： 异步就是发起一个调用后，立刻得到被调用者的回应表示已接收到请求，但是被调用者并没有返回结果，此时我们可以处理其他的请求，被调用者通常依靠事件，回调等机制来通知调用者其返回结果。
```

- 阻塞和非阻塞

```properties
阻塞： 阻塞就是发起一个请求，调用者一直等待请求结果返回，也就是当前线程会被挂起，无法从事其他任务，只有当条件就绪才能继续。

非阻塞： 非阻塞就是发起一个请求，调用者不用一直等着结果返回，可以先去干其他事情。
```



## BIO(Blocking I/O)

- **同步阻塞I/O模式**，数据的读取写入必须阻塞在一个线程内等待其完成。



```java
package com.juc.demo.io;

import org.junit.Test;

import java.io.*;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

class ConnectIOnHandler extends Thread {

    private Socket socket;

    public ConnectIOnHandler(Socket socket) {
        this.socket = socket;
    }

    public void run() {
        try {
            InputStream input = socket.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(input));
            while (true) {
                String line = reader.readLine();
                if (line != null) {
                    System.out.println(line);
                }
                if (line != null && line.equals("quit")) {
                    System.out.println(line);
                    socket.close();
                    break;
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
            try {
                socket.close();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }
}

class ClientThread implements Runnable {

    private Socket cSocket;
    private int i;

    public ClientThread(Socket cSocket, int i) {
        super();
        this.cSocket = cSocket;
        this.i = i;
    }


    @Override
    public void run() {
        try {
            cSocket.connect(new InetSocketAddress(8088));
            OutputStream out = cSocket.getOutputStream();
            String con = "thread" + i + "\r\n";
            out.write(con.getBytes());
            Thread.sleep(1000);
            out.write("quit".getBytes());
            out.close();
            cSocket.close();

        } catch (Exception e) {
            try {
                cSocket.close();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
            e.printStackTrace();
        }
    }

}

public class Bio {

    @Test
    public void bioServer() {
        try {
            ServerSocket socket = new ServerSocket(8088);
            int i = 1;
            while (true) {
                Socket cSocket = socket.accept();
                System.out.println("线程数:" + i);
                System.out.println(cSocket.getPort());
                new Thread(new ConnectIOnHandler(cSocket)).start();
                i++;
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void bioClient() {
        ByteArrayOutputStream output = null;
        try {

            int i = 1;
            while (true) {
                Socket socket = new Socket();
                System.out.println("start new Thread");
                new Thread(new ClientThread(socket, i)).start();
                i++;
                Thread.sleep(2000);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (output != null) {
                try {
                    output.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Test
    public void test() {
        // TODO 服务端处理客户端连接请求
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(3333);
        } catch (IOException e) {
            e.printStackTrace();
        }

        // 接收到客户端连接请求之后为每个客户端创建一个新的线程进行链路处理
        ServerSocket finalServerSocket = serverSocket;
        Thread ts = new Thread(() -> {
            while (true) {
                try {
                    // 阻塞方法获取新的连接
                    Socket socket = finalServerSocket.accept();

                    // 每一个新的连接都创建一个线程，负责读取数据
                    new Thread(() -> {
                        try {
                            int len;
                            byte[] data = new byte[1024];
                            InputStream inputStream = socket.getInputStream();
                            // 按字节流方式读取数据
                            while ((len = inputStream.read(data)) != -1) {
                                System.out.println(new String(data, 0, len));
                            }
                        } catch (IOException e) {
                        }
                    }).start();

                } catch (IOException e) {
                }

            }
        });

        // TODO 创建多个线程，模拟多个客户端连接服务端
        Thread tc = new Thread(() -> {
            try {
                Socket socket = new Socket("127.0.0.1", 3333);
                while (true) {
                    try {
                        socket.getOutputStream().write((new Date() + ": hello world").getBytes());
                        Thread.sleep(2000);
                    } catch (Exception e) {
                    }
                }
            } catch (IOException e) {
            }
        });

        ts.start();
        tc.start();

        try {
            ts.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```



## NIO(New I/O，Non-blocking I/O)

- NIO是一种**同步非阻塞的I/O模型**，在Java 1.4 中引入了NIO框架，对应 `java.nio` 包，提供了 `Channel , Selector，Buffer`等抽象。



```java
package com.juc.demo.io;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.Date;
import java.util.Iterator;
import java.util.Set;

public class Nio {

    public static void main(String[] args) throws IOException {
        // 1. serverSelector负责轮询是否有新的连接，服务端监测到新的连接之后，不再创建一个新的线程，
        // 而是直接将新连接绑定到clientSelector上，这样就不用 IO 模型中 1w 个 while 循环在死等
        Selector serverSelector = Selector.open();
        // 2. clientSelector负责轮询连接是否有数据可读
        Selector clientSelector = Selector.open();

        Thread ts = new Thread(() -> {
            try {
                // 对应IO编程中服务端启动
                ServerSocketChannel listenerChannel = ServerSocketChannel.open();
                listenerChannel.socket().bind(new InetSocketAddress(3333));
                listenerChannel.configureBlocking(false);
                listenerChannel.register(serverSelector, SelectionKey.OP_ACCEPT);

                while (true) {
                    // 监测是否有新的连接，这里的1指的是阻塞的时间为 1ms
                    if (serverSelector.select(1) > 0) {
                        Set<SelectionKey> set = serverSelector.selectedKeys();
                        Iterator<SelectionKey> keyIterator = set.iterator();

                        while (keyIterator.hasNext()) {
                            SelectionKey key = keyIterator.next();

                            if (key.isAcceptable()) {
                                try {
                                    // (1) 每来一个新连接，不需要创建一个线程，而是直接注册到clientSelector
                                    SocketChannel clientChannel = ((ServerSocketChannel) key.channel()).accept();
                                    clientChannel.configureBlocking(false);
                                    clientChannel.register(clientSelector, SelectionKey.OP_READ);
                                } finally {
                                    keyIterator.remove();
                                }
                            }

                        }
                    }
                }
            } catch (IOException ignored) {
            }
        });
        Thread tc = new Thread(() -> {
            try {
                while (true) {
                    // (2) 批量轮询是否有哪些连接有数据可读，这里的1指的是阻塞的时间为 1ms
                    if (clientSelector.select(1) > 0) {
                        Set<SelectionKey> set = clientSelector.selectedKeys();
                        Iterator<SelectionKey> keyIterator = set.iterator();

                        while (keyIterator.hasNext()) {
                            SelectionKey key = keyIterator.next();

                            if (key.isReadable()) {
                                try {
                                    SocketChannel clientChannel = (SocketChannel) key.channel();
                                    ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
                                    // (3) 面向 Buffer
                                    clientChannel.read(byteBuffer);
                                    byteBuffer.flip();
                                    System.out.println(
                                            Charset.defaultCharset().newDecoder().decode(byteBuffer).toString());
                                } finally {
                                    keyIterator.remove();
                                    key.interestOps(SelectionKey.OP_READ);
                                }
                            }

                        }
                    }
                }
            } catch (IOException ignored) {
            }
        });

        ts.start();
        tc.start();

        // TODO 创建多个线程，模拟多个客户端连接服务端
        Thread t = new Thread(() -> {
            try {
                Socket socket = new Socket("127.0.0.1", 3333);
                while (true) {
                    try {
                        socket.getOutputStream().write((new Date() + ": hello world").getBytes());
                        Thread.sleep(2000);
                    } catch (Exception e) {
                    }
                }
            } catch (IOException e) {
            }
        });

        t.start();

        try {
            ts.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try {
            ts.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```



## AIO(Asynchronous I/O)

- AIO 也就是 NIO 2。在 Java 7 中引入了 NIO 的改进版 NIO 2,它是**异步非阻塞的IO模型**。异步 IO 是基于事件和回调机制实现的，也就是应用操作之后会直接返回，不会堵塞在那里，当后台处理完成，操作系统会通知相应的线程进行后续的操作。



```java
package com.juc.demo.io;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.AsynchronousServerSocketChannel;
import java.nio.channels.AsynchronousSocketChannel;
import java.nio.channels.CompletionHandler;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;


public class Aio {

    public static final int    PORT = 8082;
    public static final String HOST = "localhost";

    public static void main(String[] args) throws IOException, InterruptedException, ExecutionException {
        new Aio();
    }

    public Aio() throws IOException, InterruptedException, ExecutionException {
        // open a server channel and bind to a free address, then accept a connection
        System.out.println("Open server channel");
        SocketAddress address = new InetSocketAddress(HOST, PORT);
        AsynchronousServerSocketChannel server = AsynchronousServerSocketChannel.open().bind(address);
        System.out.println("Initiate accept");
        Future<AsynchronousSocketChannel> future = server.accept();

        // wait for the accept to finish
        AsynchronousSocketChannel worker = future.get();
        System.out.println("Accept completed");

        ByteBuffer readBuffer = ByteBuffer.allocate(100);
        try {
            // read a message from the client, timeout after 10 seconds
            worker.read(readBuffer).get(10, TimeUnit.SECONDS);
            System.out.println("Message received from client: " + new String(readBuffer.array()));

            // send a message to the client
            ByteBuffer message = ByteBuffer.wrap("hello client, i am Alice.".getBytes());
            worker.write(message);
        } catch (TimeoutException e) {
            System.out.println("Client didn't respond in time");
        }
        server.close();
    }
}



class AIOSocketClient {

    public static final int    PORT = 8082;
    public static final String HOST = "localhost";

    public static void main(String[] args) throws IOException {
        // create a client
        SocketAddress address = new InetSocketAddress(HOST, PORT);
        ClientWrapper client = new ClientWrapper(address);
        // start client thread
        client.start();
        try {
            client.join();
        } catch (InterruptedException e) {
            System.out.println(e);
        }
        client.close();
    }

}

class ClientWrapper extends Thread {

    AsynchronousSocketChannel client;
    Future<Void>              connectFuture;

    public ClientWrapper(SocketAddress server) throws IOException{
        // open a new socket channel and connect to the server
        System.out.println("Open client channel");
        client = AsynchronousSocketChannel.open();
        System.out.println("Connect to server");
        connectFuture = client.connect(server);
    }

    public void run() {
        System.out.println("client run.");
        // if the connect hasn't happened yet cancel it
        // if (!connectFuture.isDone()) {
        // connectFuture.cancel(true);
        // return;
        // }
        try {
            connectFuture.get();
        } catch (InterruptedException e1) {
            System.out.println("client connect error." + e1);
            return;

        } catch (ExecutionException e1) {
            System.out.println("client connect error." + e1);
            return;
        }

        try {
            // send a message to the server
            ByteBuffer message = ByteBuffer.wrap("hello server, i am Bruce.".getBytes());
            // wait for the response
            System.out.println("Sending message to the server...");
            Integer countBytes = client.write(message).get();
            System.out.println(countBytes);

            final ByteBuffer readBuffer = ByteBuffer.allocate(100);
            // Future<Integer> numberBytes = client.read(readBuffer);
            client.read(readBuffer, null, new CompletionHandler<Integer, Object>() {

                @Override
                public void completed(Integer result, Object attachment) {
                    System.out.println("Message received from server: " + new String(readBuffer.array()));
                    clearUp();
                }

                @Override
                public void failed(Throwable e, Object attachment) {
                    System.err.println("Exception performing write");
                    e.printStackTrace();
                    clearUp();
                }

                private void clearUp() {
                    try {
                        client.close();
                    } catch (IOException e) {
                        System.out.println(e);
                    }
                }
            });

        } catch (InterruptedException e) {
            System.out.println(e);
        } catch (ExecutionException e) {
            System.out.println(e);
        }
    }

    public void close() throws IOException {
        client.close();
    }
}
```





# netty