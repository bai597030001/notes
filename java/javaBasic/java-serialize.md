# 序列化和反序列化

1、什么叫序列化和反序列化

2、作用。为啥要实现这个 `Serializable` 接口，也就是为啥要序列化

3、`serialVersionUID` 这个的值到底是在怎么设置的，有什么用。有的是`1L`，有的是一长串数字

## 概念

序列化：把对象转换为字节序列的过程称为对象的序列化。

反序列化：把字节序列恢复为对象的过程称为对象的反序列化。

> 在程序运行过程中，一个类的对象会有很多的数据，并且带有状态信息（数据有对应的值）。如果我们想把这些类的数据保存起来（例如存文件），或者是通过网络发送给远端主机，那么就需要把内存里面的这些对象给变成一连串的字节(bytes)描述，这个过程叫序列化。
>
> 对应的，反序列化就是从文件读取对象信息、或者从远端接受发送来的对象信息，需要将读到/接收到的字节(bytes)转换成对象，这个过程叫反序列化。

## 什么情况下需要序列化 

需要想把的内存中的对象状态保存到一个文件；

当想用套接字在网络上传送对象及其状态；

当想通过RMI传输对象及其状态；

## 实现序列化 Serializable接口

实现Serializable接口即可



```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class User implements Serializable{
    // 如果实现了Serializable接口，则jdk默认根据当前该类的属性计算一个serialVersionUID
    // 所以默认如果序列化与反序列化时类的属性发生变化，则生成的serialVersionUID也不一样，反序列化会失败
    // 可以先将对象序列化到文件，然后修改/新增/删除一个对象属性，再进行反序列化，会提示serialVersionUID不一致导致失败
    // 针对上述情况，可以手动指定一个serialVersionUID，这样即使序列化与反序列化时属性有修改，也会成功反序列化
    private static final long serialVersionUID = 1L;
    private int id;
    private String name;
    // transient 修饰的属性，是不会被序列化的
    private transient String password;
    // 静态static的属性，不序列化
    private static String action = "go running";
}
```

```java
public class SerializeTest {

    private static String filePath = "serialize.txt";

    public static void main(String[] args) throws IOException, ClassNotFoundException {
        User user = new User(1, "Tom", "123456");
        serializeUser(user);
        User deserializedUser = deserializedUser(filePath);
        System.out.println(deserializedUser.toString());
    }

    /*
        writeObject(Object obj)方法可对参数指定的obj对象进行序列化，把得到的字节序列写到一个目标输出流中
     */
    private static void serializeUser(User user) throws IOException {
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(new File(filePath)));
        oos.writeObject(user);
        System.out.println("user 对象序列化成功！");
        oos.close();
    }

    /*
        readObject()方法从一个源输入流中读取字节序列，再把它们反序列化为一个对象，并将其返回
     */
    private static User deserializedUser(String filePath) throws IOException, ClassNotFoundException {
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream(new File(filePath)));
        User user = (User) ois.readObject();
        System.out.println("user 对象反序列化成功！");
        return user;
    }
}
```

## writeReplace()和readResolve()

在`ObjectInputStream`中的序列化`invokeWriteReplace()`和反序列化`invokeReadResolve()`方法

- `writeReplace()`

> 返回一个对象，该对象为实际被序列化的对象，在原对象序列化之前被调用，替换原对象成为待序列化对象

- `readResolve()`

> 返回一个对象，该对象为实际反序列化的结果对象，在原对象反序列化之后被调用，不影响原对象的反序列化过程，仅替换结果

## Externalizable

`Serializable`接口还有一个比较常见的子类`Externalizable`，它比父类特殊的地方就在于它需要自己实现读写方法 *（readExternal()和writeExternal()）* ，同时必须包含一个自己的无参构造方法 *（默认隐式的也可以）* 。

```java
@Data
public class ExternalizationModel implements Externalizable{
    private String stringField;
    private int intField;

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeUTF(this.stringField);
        out.writeInt(this.intField);
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException {
        this.stringField = "hello, i'm " + in.readUTF();
        this.intField = in.readInt() + 100000;
    }

    public ExternalizationModel(String s, int i) {
        this.stringField = s;
        this.intField = i;
    }

    public ExternalizationModel() {}
}
```

- 序列化调用`writeExternalData()`转给类自定义的写方法，将写入的数据包装在新的Block Data块中，第一个字节为块长度 *（不含块头尾标识）*
- 反序列化调用`readExternalData()`转给类自定义的读方法，再调用对象的无参构造方法 *（若不存在，则返回null）* 进行实例化