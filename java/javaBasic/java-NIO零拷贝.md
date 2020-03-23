# Java NIO

在java编程中，一般使用流的方式来处理IO，所有的IO都被视作是单个字节的移动，通过stream对象一次移动一个字节。IO流负责把对象转换为字节，然后再转换为对象。

NIO和IO有相同的作用和目的，但实现方式不同，**NIO主要用到的是块**，所以NIO的效率要比IO高很多。

在Java API中提供了两套NIO，一套是针对**标准输入输出NIO**，另一套就是**网络编程NIO**



##  流与块的比较

NIO和IO最大的区别是数据打包和传输方式。IO是以**流**的方式处理数据，而NIO是以**块**的方式处理数据。

**面向流**的IO一次一个字节的处理数据，一个输入流产生一个字节，一个输出流就消费一个字节。为流式数据创建过滤器就变得非常容易，链接几个过滤器，以便对数据进行处理非常方便而简单，但是面向流的IO通常处理的很慢。

**面向块**的IO系统以块的形式处理数据。每一个操作都在一步中产生或消费一个数据块。按块要比按流快的多，但面向块的IO缺少了面向流IO所具有的有雅兴和简单性。



## 标准输入输出NIO

`Buffer`和`Channel`是标准NIO中的核心对象（网络NIO中还有个`Selector`核心对象）



### Buffer

`Buffer`是一个对象，它包含一些要写入或读出的数据。在`NIO`中，数据是放入`buffer`对象的，而在`IO`中，数据是直接写入或者读到`Stream`对象的。**应用程序不能直接对 `Channel` 进行读写操作，而必须通过 `Buffer` 来进行**，即： `Channel` 是通过 `Buffer` 来读写数据的。

在NIO中，所有的数据都是用`Buffer`处理的，它是NIO读写数据的中转池。`Buffer`实质上是一个数组，<font color=#dd0000>通常是一个字节数据</font>，但也可以是其他类型的数组。但一个缓冲区不仅仅是一个数组，重要的是它提供了对数据的结构化访问，而且还可以跟踪系统的读写进程。

使用 `Buffer` 读写数据一般遵循以下四个步骤：

1. 写入数据到 `Buffer`；
2. 调用 `flip()` 方法；
3. 从 `Buffer` 中读取数据；
4. 调用 `clear()` 方法或者 `compact()` 方法。

当向 `Buffer` 写入数据时，Buffer 会记录下写了多少数据。一旦要读取数据，需要通过 `flip()` 方法将 `Buffer` **从写模式切换到读模式**。在读模式下，可以读取之前写入到 `Buffer` 的所有数据。

一旦读完了所有的数据，就需要清空缓冲区，让它可以再次被写入。有两种方式能清空缓冲区：调用 `clear()` 或 `compact()` 方法。`clear()` 方法会清空整个缓冲区。`compact()` 方法只会清除已经读过的数据。任何未读的数据都被移到缓冲区的起始处，新写入的数据将放到缓冲区未读数据的后面。



```properties
# 继承关系
buffer

	-> ByteBuffer

		-> HeapByteBuffer

		-> MappedByteBuffer
		
			-> DirectByteBuffer

	-> CharBuffer

	-> DubbleBuffer

	-> FloadBuffer

	-> IntBuffer

	-> LongBuffer

	-> ShortBuffer
```



**Buffer 的比较：**

我们可以通过 `equals`() 或 compareTo() 方法比较两个 `Buffer`, 当且仅当如下条件满足时, 两个 Buffer 是相等的



#### ByteBuffer



**ByteBuffer的属性**

```java
byte[] buff  //buff即内部用于缓存的数组。
position //当前读取的位置。
mark //为某一读过的位置做标记，便于某些时候回退到该位置。
capacity //初始化时候的容量。
limit //当写数据到buffer中时，limit一般和capacity相等，当读数据时，limit代表buffer中有效数据的长度。
```



**ByteBuffer的常规方法**

```java
ByteBuffer allocate(int capacity) //创建一个指定capacity的ByteBuffer。
ByteBuffer allocateDirect(int capacity) //创建一个direct的ByteBuffer，这样的ByteBuffer在参与IO操作时性能会更好
ByteBuffer wrap(byte [] array)
ByteBuffer wrap(byte [] array, int offset, int length) //把一个byte数组或byte数组的一部分包装成ByteBuffer。
//get put方法不多说
byte get(int index)
ByteBuffer put(byte b)
int getInt()       　　　　　　//从ByteBuffer中读出一个int值。
ByteBuffer putInt(int value)  // 写入一个int值到ByteBuffer中。
```



**ByteBuffer的特殊方法**

```java
Buffer clear()    //把position设为0，把limit设为capacity，一般在把数据写入Buffer前调用。
Buffer flip() 　  //把limit设为当前position，把position设为0，一般在从Buffer读出数据前调用。
Buffer rewind()  //把position设为0，limit不变，一般在把数据重写入Buffer前调用。
compact()       //将 position 与 limit之间的数据复制到buffer的开始位置，复制后 position = limit -position,limit = capacity, 但如         果position 与limit 之间没有数据的话发，就不会进行复制。
mark() & reset()     //通过调用Buffer.mark()方法，可以标记Buffer中的一个特定position。之后可以通过调用Buffer.reset()方法恢复到这个position。
```



##### HeapByteBuffer



##### MappedByteBuffer



###### DirectByteBuffer



| -                | 描述                                                         | 优点                                                         |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| HeapByteBuffer   | 在jvm堆上面的一个buffer，底层的本质是一个数组                | 由于内容维护在jvm里，所以把内容写进buffer里速度会快些；并且，可以更容易回收 |
| DirectByteBuffer | 底层的数据其实是维护在操作系统的内存中，而不是jvm里，DirectByteBuffer里维护了一个引用address指向了数据，从而操作数据 | 跟外设（IO设备）打交道时会快很多，因为外设读取jvm堆里的数据时，不是直接读取的，而是把jvm里的数据读到一个内存块里，再在这个块里读取的，如果使用DirectByteBuffer，则可以省去这一步，实现zero copy |



#### CharBuffer

#### DubbleBuffer

#### FloadBuffer

#### IntBuffer

#### LongBuffer

#### ShortBuffer



### Channel

Channel是一个对象，可以通过它读取和写入数据。可以把它看做IO中的流。但是它和流相比还有一些不同：

1. Channel是双向的，既可以读又可以写，而流是单向的
2. Channel可以进行异步的读写
3. 对Channel的读写必须通过buffer对象



> 正如上面提到的，所有数据都通过Buffer对象处理，所以，我们永远不会将字节直接写入到Channel中，相反，我们应该将数据写入到Buffer中；
>
> 同样，也不会从Channel中读取字节，而是将数据从Channel读入Buffer，再从Buffer获取这个字节。



Channel类型

#### FileChannel

从文件读取数据

无法直接打开一个FileChannel，需要通过 InputStream、OutputStream 或 RandomAccessFile 的 getChannel 方法来获取一个 FileChannel 实例。



##### 常用方法

```properties
read(ByteBuffer)，从文件通道读取数据到ByteBuffer，-1 表示已经到达输入的末尾。
write(ByteBuffer)，从字节缓存区写到文件通道。
close()，关闭。
transferFrom()，将数据从源通道传输到FileChannel中 toChannel
transferTo()，将数据从FileChannel传输到其他的channel中 fromChannel

position() 获取当前位置
position(int) 设置当前位置
size() 返回该实例所关联文件的大小
truncate(int) 截取文件的前int个字节
force(boolean) 将通道里尚未写入磁盘的内存缓存数据强制写到磁盘上，boolean指明是否同时将文件元数据（权限信息等）写到磁盘上

map()

lock()
tryLock()
release()
```



##### 内存映射文件

通过 `FileChannel` 的 `map` 方法，允许我们创建和修改那些因为太大而不能放入内存的文件，我们可以假定整个文件都放在内存中（实际上只是一部分），可以把它当做非常大的数组来访问，可以很容易的修改。但是，它的缺点是，创建映射文件的花费大于以常规方式读写几十 MB 的数据，所以只在操作较大文件是才推荐这么做。



`MappedByteBuffer` 本质上是一种 `Direct Buffer`。`Direct Buffer` 生命周期内内存地址都不会发生改变，进而内核可以安全地对其进行访问，很多 IO 操作会很高效，同时它保存在堆外，减少了堆内对象存储的可能额外维护工作。但是，它的创建和销毁过程，都会比一般的堆类 BUffer 增加部分开销。通常都建议用于长期使用、数据量较大的场景。

```java
FileChannel fc = new RandomAccessFile(new File("bigfile.txt"), "rw").getChannel();
MappedByteBuffer mbb = fc.map(FileChannel.MapMode.READ_WRITE, 0, fc.size());
...
fc.close();
```



##### 文件锁

>  从JDK1.4之后，Java引进了文件锁机制，用来在进程之间进行文件的共享与独占锁定。注意两点，文件锁定是在进程之间进行的，一个进程的多个线程之间，文件锁定无效；第二，锁定分为共享锁与独占锁，但是若操作系统或文件系统不支持，则锁的种类会自动升级。

通过文件加锁来同步访问作为共享资源的文件，文件锁对其他的操作系统进程时可见的，因为 Java 的文件加锁直接映射到了本地操作系统的加锁工具。

通过 `FileChannel` 对象的 `lock` 或 `tryLock` 方法可以对文件加锁并返回 `FileLock` 对象，通过 `FileLock` 对象的 `release` 方法可以释放锁。

```java
public class FileLockExample {
    private static String filepath = "D:\\tmp\\filelock.txt";
    private static Random rand = new Random();

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: [-r | -w]");
            System.exit(1);
        }
        boolean isWriter = args[0].equals("-w");
        try {
            RandomAccessFile randomAccessFile = new RandomAccessFile(filepath, (isWriter) ? "rw" : "r");
            FileChannel channel = randomAccessFile.getChannel();
            if (isWriter) {
                lockAndWrite(channel);
            } else {
                lockAndRead(channel);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    private static void lockAndWrite(FileChannel channel) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(4);
            int i=0;
            while (true) {
                System.out.println("Writer try to lock file...");
                // 锁住一个文件的一段内容，模式是false，意味着这是一个独占锁
                FileLock lock = channel.lock(0,4,false);

                buffer.putInt(0,i);
                buffer.position(0).limit(4);
                System.out.println("buffer is :"+buffer);
                channel.write(buffer,0);
                channel.force(true);
                buffer.clear();
                System.out.println("Writer write :" + i++);

                lock.release();
                System.out.println("Sleeping...");
                TimeUnit.SECONDS.sleep(rand.nextInt(3));
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void lockAndRead(FileChannel channel) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(4);
            while (true) {
                System.out.println("Reader try to lock file...");
                // true,意味着这是一个共享锁
                FileLock lock = channel.lock(0,4,true);

                buffer.clear();
                channel.read(buffer,0);
                buffer.flip();
                System.out.println("buffer is:"+buffer);
                int i = buffer.getInt(0);
                System.out.println("Reader read :" + i);

                lock.release();
                System.out.println("Sleeping...");
                TimeUnit.SECONDS.sleep(rand.nextInt(3));
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```



##### Scattering Gathering 

在使用各种Channel类时，我们已经观察到read和write方法还有一种以ByteBuffer数组为参数的形式，这种形式其实是为了支持通道的Scatter和Gather特性。Scatter的意思是从<font color=#dd0000>多个ByteBuffer中依次读取数据到一个Channel</font>中，Gather的意思则是<font color=#dd0000>将Channel中的数据依次写入多个ByteBuffer里</font>。在某些特定场合，Scatter/Gather将大大减轻编程的工作量，例如将某些网络包的包头、内容分别读入不同的变量中。



```java
public class NioGatheringTest {
    public static void main(String[] args) throws IOException {
        ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
        InetSocketAddress address = new InetSocketAddress(8899);
        //服务器监听8899端口
        serverSocketChannel.socket().bind(address);
        int messageLength = 2 + 3 + 4;
        ByteBuffer[] byteBuffers = new ByteBuffer[3];
        byteBuffers[0] = ByteBuffer.allocate(2);
        byteBuffers[1] = ByteBuffer.allocate(3);
        byteBuffers[2] = ByteBuffer.allocate(4);

        SocketChannel socketChannel = serverSocketChannel.accept();

        while (true) {
            int bytesRead = 0;
            while (bytesRead < messageLength) {
                //将buffer数组进行写入操作。第一个满了就写第二个，返回该次写入的总长度
                long r = socketChannel.read(byteBuffers);
                bytesRead += r;
                System.out.println("byteRead:" + bytesRead);
                //stream将每一个buffer对象转化成String对象
                Arrays.asList(byteBuffers).stream().map(buffer->"position:"+buffer.position()+"limit:"+buffer.limit()).forEach(System.out::println);
            }
            Arrays.asList(byteBuffers).forEach(byteBuffer -> {
                byteBuffer.flip();
            });
            long byteWritten =0;
            while (byteWritten < messageLength) {
                long r = socketChannel.write(byteBuffers);
                byteWritten += r;
            }
            Arrays.asList(byteBuffers).forEach(byteBuffer -> byteBuffer.clear());
            System.out.println("bytesRead:" + bytesRead + ",bytesWritten:" + byteWritten + ",messageLength:"+messageLength);
        }

    }
}
```





#### DatagramChannel

读写UDP网络协议数据



#### SocketChannel

读写TCP网络协议数据



#### ServerSocketChannel

可以监听TCP连接



CopyFile示例

CopyFile执行三个基本的操作：创建一个Buffer，然后从源文件读取数据到缓冲区，然后再将缓冲区写入目标文件。

```java
public class NioReadWrite {

    public static void main(String[] args) {
        try {
            copyFileUseNIO("E:\\temp\\test.txt", "E:\\temp\\test-copy.txt");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void copyFileUseNIO(String src, String dst) throws IOException {
        //声明源文件和目标文件
        FileInputStream fi = new FileInputStream(new File(src));
        FileOutputStream fo = new FileOutputStream(new File(dst));
        //获得传输通道channel
        FileChannel inChannel = fi.getChannel();
        FileChannel outChannel = fo.getChannel();
        //获得容器buffer
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        while (true) {
            //判断是否读完文件
            int eof = inChannel.read(buffer);
            if (eof == -1) {
                break;
            }
            //重设一下buffer的position=0，limit=position
            buffer.flip();
            //开始写
            outChannel.write(buffer);
            //写完要重置buffer，重设position=0,limit=capacity
            buffer.clear();
        }
        inChannel.close();
        outChannel.close();
        fi.close();
        fo.close();
    }
}
```



### Pipe

Pipe管道的概念最先应该出现在Unix系统里，用来表示连接不同进程间的一种单向数据通道，很多Unix系统都提供了支持管道的API。

Java NIO借用了这个概念，发明了NIO中的Pipe，它是指<font color=#dd0000>同一个Java进程内，不同线程间的一种单向数据管道，其sink端通道写入数据，source端通道则读出数据，其间可以保证数据按照写入顺序到达。</font>

```java
public class SimplePipe {
    public static void main(String[] args) throws IOException {
        //创建一个管道，并拿到管道两端的channel
        Pipe pipe = Pipe.open();
        WritableByteChannel writableByteChannel = pipe.sink();
        ReadableByteChannel readableByteChannel = pipe.source();

        //创建一个线程从sink端写入数据
        WorkerThread thread = new WorkerThread(writableByteChannel);
        thread.start();


        //主线程从source端读取数据，并组成String打印
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        while ( readableByteChannel.read(buffer) >= 0) {
            buffer.flip();
            byte[] bytes = new byte[buffer.remaining()];
            buffer.get(bytes);
            String str = new String(bytes);
            System.out.println(str);
            buffer.clear();
        }
        readableByteChannel.close();
    }

    private static class WorkerThread extends Thread {
        WritableByteChannel channel;

        public WorkerThread(WritableByteChannel writableByteChannel) {
            this.channel = writableByteChannel;
        }

        @Override
        public void run() {
            ByteBuffer buffer = ByteBuffer.allocate(1024);
            for (int i = 0; i < 10; i++) {
                String str = "pipe sink data " + i;

                buffer.put(str.getBytes());
                buffer.flip();
                try {
                    channel.write(buffer);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                buffer.clear();
            }
            try {
                channel.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```





## 网络编程NIO



# Java NIO 零拷贝实现

在 Java NIO 中的通道（Channel）就相当于操作系统的内核空间（kernel space）的缓冲区。而缓冲区（Buffer）对应的相当于操作系统的用户空间（user space）中的用户缓冲区（user buffer）：

- 通道（Channel）是全双工的（双向传输），它既可能是读缓冲区（read buffer），也可能是网络缓冲区（socket buffer）。
- 缓冲区（Buffer）分为堆内存（HeapBuffer）和堆外内存（DirectBuffer），这是通过 malloc 分配出来的用户态内存。

堆外内存（DirectBuffer）在使用后需要应用程序手动回收，而堆内存（HeapBuffer）的数据在 GC 时可能会被自动回收。

因此，在使用 HeapBuffer 读写数据时，为了避免缓冲区数据因为 GC 而丢失，NIO 会先把 HeapBuffer 内部的数据拷贝到一个临时的 DirectBuffer 中的本地内存（native memory）。

这个拷贝涉及到 sun.misc.Unsafe.copyMemory 的调用，背后的实现原理与 memcpy 类似。

最后，将临时生成的 DirectBuffer 内部的数据的内存地址传给 I/O 调用函数，这样就避免了再去访问 Java 对象处理 I/O 读写。



Java NIO引入了用于通道的缓冲区的ByteBuffer。 ByteBuffer有三个主要的实现：

**HeapByteBuffer**

在调用ByteBuffer.allocate（）时使用。 它被称为堆，因为它保存在JVM的堆空间中，因此您可以获得所有优势，如GC支持和缓存优化。 但是，它不是页面对齐的，这意味着如果您需要通过JNI与本地代码交谈，JVM将不得不复制到对齐的缓冲区空间。

**DirectByteBuffer**

在调用ByteBuffer.allocateDirect（）时使用。 JVM将使用malloc（）在堆空间之外分配内存空间。 因为它不是由JVM管理的，所以你的内存空间是页面对齐的，不受GC影响，这使得它成为处理本地代码的完美选择。 然而，你要C程序员一样，自己管理这个内存，必须自己分配和释放内存来防止内存泄漏。

**MappedByteBuffer**

在调用FileChannel.map（）时使用。 与DirectByteBuffer类似，这也是JVM堆外部的情况。 它基本上作为OS mmap（）系统调用的包装函数，以便代码直接操作映射的物理内存数据。



## MappedByteBuffer

MappedByteBuffer 是 NIO 基于内存映射（mmap）这种零拷贝方式提供的一种实现，它继承自 ByteBuffer。

FileChannel 定义了一个 map 方法，它可以把一个文件从 position 位置开始的 size 大小的区域映射为内存映像文件。

抽象方法 map 方法在 FileChannel 中的定义如下：

```java
public abstract MappedByteBuffer map(MapMode mode, long position, long size) throwsIOException;
```

**Mode：**限定内存映射区域（MappedByteBuffer）对内存映像文件的访问模式，包括只可读（READ_ONLY）、可读可写（READ_WRITE）和写时拷贝（PRIVATE）三种模式。

**Position：**文件映射的起始地址，对应内存映射区域（MappedByteBuffer）的首地址。

**Size：**文件映射的字节长度，从 Position 往后的字节数，对应内存映射区域（MappedByteBuffer）的大小。

MappedByteBuffer 相比 ByteBuffer 新增了三个重要的方法：

- `fore`：对于处于 READ_WRITE 模式下的缓冲区，把对缓冲区内容的修改强制刷新到本地文件。
- `load`：将缓冲区的内容载入物理内存中，并返回这个缓冲区的引用。
- `isLoaded`：如果缓冲区的内容在物理内存中，则返回 true，否则返回 false。



下面给出一个利用 MappedByteBuffer 对文件进行读写的使用示例：

```java
private final static String CONTENT = "Zero copy implemented by MappedByteBuffer";

private final static String FILE_NAME = "/mmap.txt";

private final static String CHARSET = "UTF-8";
```



**写文件数据：**打开文件通道 fileChannel 并提供读权限、写权限和数据清空权限，通过 fileChannel 映射到一个可写的内存缓冲区 mappedByteBuffer，将目标数据写入 mappedByteBuffer，通过 force 方法把缓冲区更改的内容强制写入本地文件。

```java
@Test
public void writeToFileByMappedByteBuffer() {
    
    Path path = Paths. get(getClass.getResource(FILE_NAME).getPath);
    byte[] bytes = CONTENT.getBytes(Charset.forName(CHARSET));
    
    try(FileChannel fileChannel = FileChannel.open(path, StandardOpenOption.READ,
        StandardOpenOption.WRITE, StandardOpenOption.TRUNCATE_EXISTING)) {
        
        MappedByteBuffer mappedByteBuffer = fileChannel.map(READ_WRITE, 0, bytes.length);
        if(mappedByteBuffer != null) {
        	mappedByteBuffer.put(bytes);
        	mappedByteBuffer.force;
        }
    } catch(IOException e) {
    	e.printStackTrace;
    }
}
```



**读文件数据：**打开文件通道 fileChannel 并提供只读权限，通过 fileChannel 映射到一个只可读的内存缓冲区 mappedByteBuffer，读取 mappedByteBuffer 中的字节数组即可得到文件数据。

```java
@Test
public void readFromFileByMappedByteBuffer() {

    Path path = Paths. get(getClass.getResource(FILE_NAME).getPath);
    intlength = CONTENT.getBytes(Charset.forName(CHARSET)).length;

    try(FileChannel fileChannel = FileChannel.open(path, StandardOpenOption.READ)) {
            
        	MappedByteBuffer mappedByteBuffer = fileChannel.map(READ_ONLY, 0, length);
            if(mappedByteBuffer != null) {
            byte[] bytes = newbyte[length];
            mappedByteBuffer.get(bytes);
            String content = newString(bytes, StandardCharsets.UTF_8);
            assertEquals(content, "Zero copy implemented by MappedByteBuffer");
        }
    } catch(IOException e) {
    	e.printStackTrace;
    }
}
```



下面介绍 map 方法的底层实现原理。map 方法是 java.nio.channels.FileChannel 的抽象方法，由子类 sun.nio.ch.FileChannelImpl.java 实现。

下面是和内存映射相关的核心代码：

```java
public MappedByteBuffer map(MapMode mode, longposition, longsize)throwsIOException {

    int pagePosition = ( int)(position % allocationGranularity);

    long mapPosition = position - pagePosition;

    long mapSize = size + pagePosition;

    try{
    	addr = map0(imode, mapPosition, mapSize);
    } catch(OutOfMemoryError x) {

    	System.gc;

        try{
            Thread.sleep( 100);
        } catch(InterruptedException y) {
            Thread.currentThread.interrupt;
        }
        try{
            addr = map0(imode, mapPosition, mapSize);
        } catch(OutOfMemoryError y) {
            thrownewIOException( "Map failed", y);
        }
    }

    intisize = ( int)size;
    Unmapper um = new Unmapper(addr, mapSize, isize, mfd);
    if((!writable) || (imode == MAP_RO)) {
   		returnUtil.newMappedByteBufferR(isize, addr + pagePosition, mfd, um);
    } else{
        returnUtil.newMappedByteBuffer(isize, addr + pagePosition, mfd, um);
    }
}
```



map 方法通过本地方法 map0 为文件分配一块虚拟内存，作为它的内存映射区域，然后返回这块内存映射区域的起始地址：

- 文件映射需要在 Java 堆中创建一个 MappedByteBuffer 的实例。如果第一次文件映射导致 OOM，则手动触发垃圾回收，休眠 100ms 后再尝试映射，如果失败则抛出异常。
- 通过 Util 的 newMappedByteBuffer（可读可写）方法或者 newMappedByteBufferR（仅读）方法反射创建一个 DirectByteBuffer 实例，其中 DirectByteBuffer 是 MappedByteBuffer 的子类。

map 方法返回的是内存映射区域的起始地址，通过（起始地址+偏移量）就可以获取指定内存的数据。

这样一定程度上替代了 read 或 write 方法，底层直接采用 sun.misc.Unsafe 类的 getByte 和 putByte 方法对数据进行读写。

```java
private native long map0(intprot, longposition, longmapSize)throwsIOException;
```



上面是本地方法（native method）map0 的定义，它通过 JNI（Java Native Interface）调用底层 C 的实现。

这个 native 函数（Java_sun_nio_ch_FileChannelImpl_map0）的实现位于 JDK 源码包下的 native/sun/nio/ch/FileChannelImpl.c 这个源文件里面。

```c++
JNIEXPORT jlong JNICALL

    Java_sun_nio_ch_FileChannelImpl_map0(JNIEnv *env, jobject this,
                                         jint prot, jlong off, jlong len){

    void *mapAddress = 0;

    jobject fdo = (*env)->GetObjectField(env, this, chan_fd);

    jint fd = fdval(env, fdo);

    intprotections = 0;

    intflags = 0;

    if(prot == sun_nio_ch_FileChannelImpl_MAP_RO) {

        protections = PROT_READ;

        flags = MAP_SHARED;

    } elseif(prot == sun_nio_ch_FileChannelImpl_MAP_RW) {

        protections = PROT_WRITE | PROT_READ;

        flags = MAP_SHARED;

    } elseif(prot == sun_nio_ch_FileChannelImpl_MAP_PV) {

        protections = PROT_WRITE | PROT_READ;

        flags = MAP_PRIVATE;

    }

    mapAddress = mmap64(

        0, /* Let OS decide location */

        len, /* Number of bytes to map */

        protections, /* File permissions */

        flags, /* Changes are shared */

        fd, /* File deor of mapped file */

        off); /* Offset into file */

    if(mapAddress == MAP_FAILED) {

        if(errno == ENOMEM) {

            JNU_ThrowOutOfMemoryError(env, "Map failed");

            returnIOS_THROWN;

        }

        returnhandle(env, -1, "Map failed");

    }

    return((jlong) ( unsignedlong) mapAddress);

}
```

可以看出 map0 函数最终是通过 mmap64 这个函数对 Linux 底层内核发出内存映射的调用， mmap64 函数的原型如下：

```c++
#include<sys/mman.h>
void *mmap64(void*addr, size_tlen, intprot, intflags, intfd, off64_toffset);
```



下面详细介绍一下 mmap64 函数各个参数的含义以及参数可选值：

**addr：**文件在用户进程空间的内存映射区中的起始地址，是一个建议的参数，通常可设置为 0 或 NULL，此时由内核去决定真实的起始地址。

当 flags 为 MAP_FIXED 时，addr 就是一个必选的参数，即需要提供一个存在的地址。

**len：**文件需要进行内存映射的字节长度。

**prot：**控制用户进程对内存映射区的访问权限：

- PROT_READ：读权限。
- PROT_WRITE：写权限。
- PROT_EXEC：执行权限。
- PROT_NONE：无权限。

**flags：**控制内存映射区的修改是否被多个进程共享：

- MAP_PRIVATE：对内存映射区数据的修改不会反映到真正的文件，数据修改发生时采用写时复制机制。
- MAP_SHARED：对内存映射区的修改会同步到真正的文件，修改对共享此内存映射区的进程是可见的。
- MAP_FIXED：不建议使用，这种模式下 addr 参数指定的必须提供一个存在的 addr 参数。

**fd：**文件描述符。每次 map 操作会导致文件的引用计数加 1，每次 unmap 操作或者结束进程会导致引用计数减 1。

**offset：**文件偏移量。进行映射的文件位置，从文件起始地址向后的位移量。



下面总结一下 MappedByteBuffer 的特点和不足之处：

- MappedByteBuffer 使用是堆外的虚拟内存，因此分配（map）的内存大小不受 JVM 的 -Xmx 参数限制，但是也是有大小限制的。
- 如果当文件超出 Integer.MAX_VALUE 字节限制时，可以通过 position 参数重新 map 文件后面的内容。
- MappedByteBuffer 在处理大文件时性能的确很高，但也存在内存占用、文件关闭不确定等问题，被其打开的文件只有在垃圾回收的才会被关闭，而且这个时间点是不确定的。
- MappedByteBuffer 提供了文件映射内存的 mmap 方法，也提供了释放映射内存的 unmap 方法。然而 unmap 是 FileChannelImpl 中的私有方法，无法直接显示调用。 因此，用户程序需要通过 Java 反射的调用 sun.misc.Cleaner 类的 clean 方法手动释放映射占用的内存区域。

```java
public static void clean(finalObject buffer)throwsException {

    AccessController.doPrivileged((PrivilegedAction<Void>) -> {

        try{

            Method getCleanerMethod = buffer.getClass.getMethod( "cleaner", newClass[ 0]);

            getCleanerMethod.setAccessible( true);

            Cleaner cleaner = (Cleaner) getCleanerMethod.invoke(buffer, newObject[ 0]);

            cleaner.clean;

        } catch(Exception e) {

            e.printStackTrace;

        }

    });

}
```



## DirectByteBuffer

DirectByteBuffer 的对象引用位于 Java 内存模型的堆里面，JVM 可以对 DirectByteBuffer 的对象进行内存分配和回收管理。

一般使用 DirectByteBuffer 的静态方法 allocateDirect 创建 DirectByteBuffer 实例并分配内存。

```java
public static ByteBuffer allocateDirect(intcapacity){
	return new DirectByteBuffer(capacity);
}
```



DirectByteBuffer 内部的字节缓冲区位在于堆外的（用户态）直接内存，它是通过 Unsafe 的本地方法 allocateMemory 进行内存分配，底层调用的是操作系统的 malloc 函数。

```java
DirectByteBuffer( intcap) {

    super( -1, 0, cap, cap);

    boolean pa = VM.isDirectMemoryPageAligned;

    intps = Bits.pageSize;

    longsize = Math.max( 1L, ( long)cap + (pa ? ps : 0));

    Bits.reserveMemory(size, cap);

    longbase= 0;

    try{
		// allocateMemory是一个native方法，并不是jvm能够控制的内存区域，通常称为堆外内存，一般是通过c/c++分配的内存（malloc）。
        base= unsafe.allocateMemory(size);

    } catch(OutOfMemoryError x) {

        Bits.unreserveMemory(size, cap);

        throwx;

    }

    unsafe.setMemory( base, size, ( byte) 0);

    // 也就是说，对于DirectByteBuffer所生成的ByteBuffer对象，一部分是在jvm堆内存上，一部分是操作系统上的堆内存上，那么为了操作堆外内存，一定在jvm堆上的对象有一个堆外内存的引用:
    if(pa && ( base% ps != 0)) {
		// 在DirectByteBuffer的父类中，可以看到address的一个变量，这个就是表示堆外内存所分配对象的地址，如此一来，jvm堆上的对象就会有一个堆外内存的一个引用，之所以需要这样做，是为了提升堆io的效率。
        address = base+ ps - ( base& (ps - 1));

    } else{

        address = base;

    }

    cleaner = Cleaner.create( this, newDeallocator( base, size, cap));

    att = null;

}
```



除此之外，初始化 DirectByteBuffer 时还会创建一个 Deallocator 线程，并通过 Cleaner 的 freeMemory 方法来对直接内存进行回收操作，freeMemory 底层调用的是操作系统的 free 函数。

```java
private static class Deallocator implements Runnable{

    private static Unsafe unsafe = Unsafe.getUnsafe;

    private long address;

    private long size;

    private int capacity;

    private Deallocator(longaddress, longsize, intcapacity){

        assert(address != 0);

        this.address = address;

        this.size = size;

        this.capacity = capacity;

    }

    public void run{

        if(address == 0) {

            return;

        }

        unsafe.freeMemory(address);

        address = 0;

        Bits.unreserveMemory(size, capacity);

    }

}
```



由于<font color=#dd0000>使用 DirectByteBuffer 分配的是系统本地的内存，不在 JVM 的管控范围之内</font>，因此直接内存的回收和堆内存的回收不同，直接内存如果使用不当，很容易造成 OutOfMemoryError。

说了这么多，那么 DirectByteBuffer 和零拷贝有什么关系？前面有提到在 MappedByteBuffer 进行内存映射时，它的 map 方法会通过 Util.newMappedByteBuffer 来创建一个缓冲区实例。

初始化的代码如下：

```java
static MappedByteBuffer newMappedByteBuffer(intsize, longaddr, FileDeor fd,
                                           Runnable unmapper) {

    MappedByteBuffer dbb;

    if(directByteBufferConstructor == null)

        initDBBConstructor;

    try{

        dbb = (MappedByteBuffer)directByteBufferConstructor.newInstance(

            newObject[] { newInteger(size), newLong(addr), fd, unmapper });

    } catch(InstantiationException | IllegalAccessException | InvocationTargetException e) {

        thrownewInternalError(e);

    }

    returndbb;

}

private static void initDBBRConstructor() {

    AccessController.doPrivileged( newPrivilegedAction<Void> {

        publicVoid run() {

            try{

                Class<?> cl = Class.forName( "java.nio.DirectByteBufferR");

                Constructor<?> ctor = cl.getDeclaredConstructor(

                    newClass<?>[] { int.class, long.class, FileDeor.class,

                                   Runnable.class });

                ctor.setAccessible( true);

                directByteBufferRConstructor = ctor;

            } catch(ClassNotFoundException | NoSuchMethodException |

                    IllegalArgumentException | ClassCastException x) {

                thrownewInternalError(x);

            }

            returnnull;

        }});

}
```



<font color=#dd0000>DirectByteBuffer 是 MappedByteBuffer 的具体实现类。</font>

实际上，Util.newMappedByteBuffer 方法通过反射机制获取 DirectByteBuffer 的构造器，然后创建一个 DirectByteBuffer 的实例，对应的是一个单独用于内存映射的构造方法：

```java
protected DirectByteBuffer(intcap, longaddr, FileDeor fd, Runnable unmapper){

    super(- 1, 0, cap, cap, fd);

    address = addr;

    cleaner = Cleaner.create( this, unmapper);

    att = null;

}
```



因此，除了允许分配操作系统的直接内存以外，DirectByteBuffer 本身也具有文件内存映射的功能，这里不做过多说明。

我们需要关注的是，<font color=#dd0000>DirectByteBuffer 在 MappedByteBuffer 的基础上提供了内存映像文件的随机读取 get 和写入 write 的操作。</font>

内存映像文件的随机读操作：

```java
public byte get() {
    return(( unsafe.getByte(ix(nextGetIndex))));
}

public byte get(inti) {
    return(( unsafe.getByte(ix(checkIndex(i)))));
}
```



内存映像文件的随机写操作：

```java
public ByteBuffer put(bytex) {

    unsafe.putByte(ix(nextPutIndex), ((x)));

    returnthis;
}

public ByteBuffer put(inti, bytex) {

    unsafe.putByte(ix(checkIndex(i)), ((x)));

    returnthis;
}
```



内存映像文件的随机读写都是借助 ix 方法实现定位的， ix 方法通过内存映射空间的内存首地址（address）和给定偏移量 i 计算出指针地址，然后由 unsafe 类的 get 和 put 方法和对指针指向的数据进行读取或写入。

```java
private longix(inti){

	return address + (( long)i << 0);

}
```



## FileChannel

FileChannel 是一个用于文件读写、映射和操作的通道，同时它在并发环境下是线程安全的。

基于 FileInputStream、FileOutputStream 或者 RandomAccessFile 的 getChannel 方法可以创建并打开一个文件通道。

FileChannel 定义了 transferFrom 和 transferTo 两个抽象方法，它通过在通道和通道之间建立连接实现数据传输的。

**transferTo：**通过 FileChannel 把文件里面的源数据写入一个 WritableByteChannel 的目的通道。

```java
public abstract long transferTo(longposition, longcount, WritableByteChannel target)

throwsIOException;
```



**transferFrom：**把一个源通道 ReadableByteChannel 中的数据读取到当前 FileChannel 的文件里面。

```java
public abstract long transferFrom(ReadableByteChannel src, longposition, longcount)

throwsIOException;
```



下面给出 FileChannel 利用 transferTo 和 transferFrom 方法进行数据传输的使用示例：

首先在类加载根路径下创建 source.txt 和 target.txt 两个文件，对源文件 source.txt 文件写入初始化数据。

```java
@Before
public void setup() {

    Path source = Paths. get(getClassPath(SOURCE_FILE));

    byte[] bytes = CONTENT.getBytes(Charset.forName(CHARSET));

    try(FileChannel fromChannel = FileChannel.open(source, StandardOpenOption.READ,

                                                   StandardOpenOption.WRITE, StandardOpenOption.TRUNCATE_EXISTING)) {

        fromChannel.write(ByteBuffer.wrap(bytes));

    } catch(IOException e) {

        e.printStackTrace;

    }

}
```



对于 transferTo 方法而言，目的通道 toChannel 可以是任意的单向字节写通道 WritableByteChannel；而对于 transferFrom 方法而言，源通道 fromChannel 可以是任意的单向字节读通道 ReadableByteChannel。

其中，FileChannel、SocketChannel 和 DatagramChannel 等通道实现了 WritableByteChannel 和 ReadableByteChannel 接口，都是同时支持读写的双向通道。

为了方便测试，下面给出基于 FileChannel 完成 channel-to-channel 的数据传输示例。

通过 transferTo 将 fromChannel 中的数据拷贝到 toChannel：

```java
@Test
public void transferTothrowsException {

    try(FileChannel fromChannel = newRandomAccessFile(

        getClassPath(SOURCE_FILE), "rw").getChannel;

        FileChannel toChannel = newRandomAccessFile(

            getClassPath(TARGET_FILE), "rw").getChannel) {

        longposition = 0L;

        longoffset = fromChannel.size;

        fromChannel.transferTo(position, offset, toChannel);

    }

}
```



通过 transferFrom 将 fromChannel 中的数据拷贝到 toChannel：

```java
@Test
public void transferFromthrowsException {

    try(FileChannel fromChannel = newRandomAccessFile(

        getClassPath(SOURCE_FILE), "rw").getChannel;

        FileChannel toChannel = newRandomAccessFile(

            getClassPath(TARGET_FILE), "rw").getChannel) {

        longposition = 0L;

        longoffset = fromChannel.size;

        toChannel.transferFrom(fromChannel, position, offset);

    }

}
```



下面介绍 transferTo 和 transferFrom 方法的底层实现原理，这两个方法也是 java.nio.channels.FileChannel 的抽象方法，由子类 sun.nio.ch.FileChannelImpl.java 实现。

transferTo 和 transferFrom 底层都是基于 Sendfile 实现数据传输的，其中 FileChannelImpl.java 定义了 3 个常量，用于标示当前操作系统的内核是否支持 Sendfile 以及 Sendfile 的相关特性。

```java
private static volatile boolean transferSupported = true;

private static volatile boolean pipeSupported = true;

private static volatile boolean fileSupported = true;
```



**transferSupported：**用于标记当前的系统内核是否支持 sendfile 调用，默认为 true。

**pipeSupported：**用于标记当前的系统内核是否支持文件描述符（fd）基于管道（pipe）的 sendfile 调用，默认为 true。

**fileSupported：**用于标记当前的系统内核是否支持文件描述符（fd）基于文件（file）的 sendfile 调用，默认为 true。

下面以 transferTo 的源码实现为例。FileChannelImpl 首先执行 transferToDirectly 方法，以 Sendfile 的零拷贝方式尝试数据拷贝。

如果系统内核不支持 Sendfile，进一步执行 transferToTrustedChannel 方法，以 mmap 的零拷贝方式进行内存映射，这种情况下目的通道必须是 FileChannelImpl 或者 SelChImpl 类型。

如果以上两步都失败了，则执行 transferToArbitraryChannel 方法，基于传统的 I/O 方式完成读写，具体步骤是初始化一个临时的 DirectBuffer，将源通道 FileChannel 的数据读取到 DirectBuffer，再写入目的通道 WritableByteChannel 里面。

```java
public long transferTo(longposition, longcount, WritableByteChannel target)

    throwsIOException {

    // 计算文件的大小
    longsz = size;

    // 校验起始位置
    if(position > sz)

        return0;

    inticount = ( int)Math.min(count, Integer.MAX_VALUE);

    // 校验偏移量
    if((sz - position) < icount)
        icount = ( int)(sz - position);

    longn;
    if((n = transferToDirectly(position, icount, target)) >= 0)
        returnn;

    if((n = transferToTrustedChannel(position, icount, target)) >= 0)
        returnn;

    returntransferToArbitraryChannel(position, icount, target);

}
```



接下来重点分析一下 transferToDirectly 方法的实现，也就是 transferTo 通过 Sendfile 实现零拷贝的精髓所在。

可以看到，transferToDirectlyInternal 方法先获取到目的通道 WritableByteChannel 的文件描述符 targetFD，获取同步锁然后执行 transferToDirectlyInternal 方法。

```java
private long transferToDirectly(longposition, inticount, WritableByteChannel target)

    throwsIOException {

    // 省略从target获取targetFD的过程

    if(nd.transferToDirectlyNeedsPositionLock) {

        synchronized(positionLock) {

            longpos = position;
            try{
                returntransferToDirectlyInternal(position, icount, target, targetFD);
            } finally{
                position(pos);
            }
        }
    } else{
        returntransferToDirectlyInternal(position, icount, target, targetFD);
    }
}
```



最终由 transferToDirectlyInternal 调用本地方法 transferTo0 ，尝试以 Sendfile 的方式进行数据传输。

如果系统内核完全不支持 Sendfile，比如 Windows 操作系统，则返回 UNSUPPORTED 并把 transferSupported 标识为 false。

如果系统内核不支持 Sendfile 的一些特性，比如说低版本的 Linux 内核不支持 DMA gather copy 操作，则返回 UNSUPPORTED_CASE 并把 pipeSupported 或者 fileSupported 标识为 false。

```java
private long transferToDirectlyInternal(longposition, inticount,

                                        WritableByteChannel target,

                                        FileDeor targetFD)throwsIOException {

    assert !nd.transferToDirectlyNeedsPositionLock || Thread.holdsLock(positionLock);

    longn = - 1;

    intti = - 1;

    try{

        begin;
        ti = threads.add;
        if(!isOpen)
            return- 1;

        do{
            n = transferTo0(fd, position, icount, targetFD);
        } while((n == IOStatus.INTERRUPTED) && isOpen);
        
        if(n == IOStatus.UNSUPPORTED_CASE) {
            if(target instanceofSinkChannelImpl)
                pipeSupported = false;
            if(target instanceofFileChannelImpl)
                fileSupported = false;
            returnIOStatus.UNSUPPORTED_CASE;
        }

        if(n == IOStatus.UNSUPPORTED) {
            transferSupported = false;
            returnIOStatus.UNSUPPORTED;
        }
        returnIOStatus.normalize(n);
    } finally{
        threads.remove(ti);
        end (n > - 1);
    }
}
```



本地方法（native method）transferTo0 通过 JNI（Java Native Interface）调用底层 C 的函数。

这个 native 函数（Java_sun_nio_ch_FileChannelImpl_transferTo0）同样位于 JDK 源码包下的 native/sun/nio/ch/FileChannelImpl.c 源文件里面。

JNI 函数 Java_sun_nio_ch_FileChannelImpl_transferTo0 基于条件编译对不同的系统进行预编译，下面是 JDK 基于 Linux 系统内核对 transferTo 提供的调用封装。

```c++
#ifdefined(__linux__) || defined(__solaris__)

#include<sys/sendfile.h>

#elifdefined(_AIX)

#include<sys/socket.h>

#elifdefined(_ALLBSD_SOURCE)

#include<sys/types.h>

#include<sys/socket.h>

#include<sys/uio.h>

#definelseek64 lseek

#definemmap64 mmap

#endif

JNIEXPORT jlong JNICALL

    Java_sun_nio_ch_FileChannelImpl_transferTo0(JNIEnv *env, jobject this,

                                                jobject srcFDO,

                                                jlong position, jlong count,

                                                jobject dstFDO)

{

    jint srcFD = fdval(env, srcFDO);

    jint dstFD = fdval(env, dstFDO);

    #ifdefined(__linux__)

    off64_toffset = ( off64_t)position;

    jlong n = sendfile64(dstFD, srcFD, &offset, ( size_t)count);

    returnn;

    #elifdefined(__solaris__)

    result = sendfilev64(dstFD, &sfv, 1, &numBytes);

    returnresult;

    #elifdefined(__APPLE__)

    result = sendfile(srcFD, dstFD, position, &numBytes, NULL, 0);

    returnresult;

    #endif

}
```



对 Linux、Solaris 以及 Apple 系统而言，transferTo0 函数底层会执行 sendfile64 这个系统调用完成零拷贝操作，sendfile64 函数的原型如下：

```c
#include<sys/sendfile.h>
ssize_t sendfile64( int out_fd, int in_fd, off_t *offset, size_t count);
```

下面简单介绍一下 sendfile64 函数各个参数的含义：

- out_fd：待写入的文件描述符。
- in_fd：待读取的文件描述符。
- offset：指定 in_fd 对应文件流的读取位置，如果为空，则默认从起始位置开始。
- count：指定在文件描述符 in_fd 和 out_fd 之间传输的字节数。

<font color=#dd0000>在 Linux 2.6.3 之前，out_fd 必须是一个 socket，而从 Linux 2.6.3 以后，out_fd 可以是任何文件。</font>

也就是说，sendfile64 函数不仅可以进行网络文件传输，还可以对本地文件实现零拷贝操作。



## 示例

我们通过代码示例来对比下传统IO与使用了零拷贝技术的NIO之间的差异。
我们通过服务端开启socket监听，然后客户端连接的服务端进行数据的传输，数据传输文件大小为237M。



1、构建传统IO的socket服务端，监听8898端口。

```java
public class OldIOServer {

    public static void main(String[] args) throws Exception {
        try (ServerSocket serverSocket = new ServerSocket(8898)) {
            while (true) {
                Socket socket = serverSocket.accept();
                DataInputStream inputStream = new DataInputStream(socket.getInputStream());

                byte[] bytes = new byte[4096];
                // 从socket中读取字节数据
                while (true) {
                    // 读取的字节数大小，-1则表示数据已被读完
                    int readCount = inputStream.read(bytes, 0, bytes.length);

                    if (-1 == readCount) {
                        break;
                    }
                }

            }
        }
    }
}
```

2、构建传统IO的客户端，连接服务端的8898端口，并从磁盘读取237M的数据文件向服务端socket中发起写请求。

```java
public class OldIOClient {

    public static void main(String[] args) throws Exception {
        Socket socket = new Socket();
        socket.connect(new InetSocketAddress("localhost", 8898)); // 连接服务端socket 8899端口

        // 设置一个大的文件, 237M
        try (FileInputStream fileInputStream = new FileInputStream(new File("/Users/david/Downloads/jdk-8u144-macosx-x64.dmg"));
                // 定义一个输出流
                DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());) {

            // 读取文件数据
            // 定义byte缓存
            byte[] buffer = new byte[4096];
            int readCount; // 每一次读取的字节数
            int total = 0; // 读取的总字节数

            long startTime = System.currentTimeMillis();

            while ((readCount = fileInputStream.read(buffer)) > 0) {
                total += readCount; //累加字节数
                dataOutputStream.write(buffer); // 写入到输出流中
            }

            System.out.println("发送的总字节数：" + total + ", 耗时：" + (System.currentTimeMillis() - startTime));
        }
    }
}
```

运行结果：发送的总字节数：237607747, **耗时：450 （400~600毫秒之间）**



接下来，我们通过使用JDK提供的NIO的方式实现数据传输与上述传统IO做对比。
1、构建基于NIO的服务端，监听8899端口。

```java
public class NewIOServer {

    public static void main(String[] args) throws Exception {
        ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.bind(new InetSocketAddress(8899));

        ByteBuffer byteBuffer = ByteBuffer.allocate(4096);

        while (true) {
            SocketChannel socketChannel = serverSocketChannel.accept();
            socketChannel.configureBlocking(false); // 这里设置为非阻塞模式
            int readCount = socketChannel.read(byteBuffer);

            while (-1 != readCount) {
                readCount = socketChannel.read(byteBuffer);

                // 这里一定要调用下rewind方法，将position重置为0开始位置
                byteBuffer.rewind();
            }
        }
    }
}
```

2、构建基于NIO的客户端，连接NIO的服务端8899端口，通过FileChannel.transferTo传输237M的数据文件。

```java
public class NewIOClient {

    public static void main(String[] args) throws Exception {
        SocketChannel socketChannel = SocketChannel.open();
        socketChannel.connect(new InetSocketAddress("localhost", 8899));
        socketChannel.configureBlocking(true);

        String fileName = "/Users/david/Downloads/jdk-8u144-macosx-x64.dmg";

        FileInputStream fileInputStream = new FileInputStream(fileName);
        FileChannel fileChannel = fileInputStream.getChannel();

        long startTime = System.currentTimeMillis();
        long transferCount = fileChannel.transferTo(0, fileChannel.size(), socketChannel); // 目标channel
        System.out.println("发送的总字节数：" + transferCount + ",耗时：" + (System.currentTimeMillis() - startTime));
        fileChannel.close();
    }
}
```

运行结果：发送的总字节数：237607747,耗时：**161（100到300毫秒之间）**



# Netty 零拷贝

Netty 中的零拷贝和上面提到的操作系统层面上的零拷贝不太一样, 我们所说的 Netty 零拷贝完全是基于（Java 层面）用户态的，它的更多的是偏向于数据操作优化这样的概念。

具体表现在以下几个方面：

- Netty 通过 DefaultFileRegion 类对 java.nio.channels.FileChannel 的 tranferTo 方法进行包装，在文件传输时可以将文件缓冲区的数据直接发送到目的通道（Channel）。
- ByteBuf 可以通过 wrap 操作把字节数组、ByteBuf、ByteBuffer 包装成一个 ByteBuf 对象, 进而避免了拷贝操作。
- ByteBuf 支持 Slice 操作, 因此可以将 ByteBuf 分解为多个共享同一个存储区域的 ByteBuf，避免了内存的拷贝。
- Netty 提供了 CompositeByteBuf 类，它可以将多个 ByteBuf 合并为一个逻辑上的 ByteBuf，避免了各个 ByteBuf 之间的拷贝。

其中第 1 条属于操作系统层面的零拷贝操作，后面 3 条只能算用户层面的数据操作优化。



# RocketMQ 和 Kafka 对比

RocketMQ 选择了 mmap+write 这种零拷贝方式，适用于业务级消息这种小块文件的数据持久化和传输。

而 Kafka 采用的是 Sendfile 这种零拷贝方式，适用于系统日志消息这种高吞吐量的大块文件的数据持久化和传输。

但是值得注意的一点是，Kafka 的索引文件使用的是 mmap+write 方式，数据文件使用的是 Sendfile 方式。

![](img/kafka-roketmq.png.)