# javaע�����

[ԭ������](https://www.cnblogs.com/yangming1996/p/9295168.html)

[ԭ������](https://blog.csdn.net/yuzongtao/article/details/83306182)

## 1.1 ע��ı���

��java.lang.annotation.Annotation���ӿ�������ôһ�仰������������ע�⡻��

The common interface extended by all annotation types

**���е�ע�����Ͷ��̳��������ͨ�Ľӿ�**��Annotation��
��仰�е���󣬵�ȴ˵����ע��ı��ʡ����ǿ�һ�� JDK ����ע��Ķ��壺
	

	@Target(ElementType.METHOD)
	@Retention(RetentionPolicy.SOURCE)
	public @interface Override {
	
	}
	����ע�� @Override �Ķ��壬��ʵ�������Ͼ��ǣ�
	public interface Override extends Annotation{
		
	}

- û��ע��ı��ʾ���һ��**�̳��� Annotation �ӿڵĽӿ�**��
  �й���һ�㣬�����ȥ����������һ��ע���࣬���õ�����ġ�

- һ��ע��׼ȷ��������˵��ֻ������һ�������ע�Ͷ��ѣ����û�н������Ĵ��룬��������ע�Ͷ����硣

## 1.2 ����һ������߷�����ע��

### 1.2.1 ������ֱ�ӵ�ɨ��

- ָ���Ǳ������ڶ� java ��������ֽ���Ĺ����л��⵽ĳ������߷�����һЩע�����Σ���ʱ���ͻ������Щע�����ĳЩ�������͵ľ���ע�� @Override��һ����������⵽ĳ�������������� @Override ע�⣬�������ͻ��鵱ǰ�����ķ���ǩ���Ƿ�������д�˸����ĳ��������Ҳ���ǱȽϸ������Ƿ����һ��ͬ���ķ���ǩ����

- ��һ�����ֻ��������Щ�������Ѿ���֪��ע���࣬���� JDK ���õļ���ע��

- �����Զ����ע�⣬�������ǲ�֪�������ע������õģ���ȻҲ��֪������δ���
  ����ֻ�ǻ���ݸ�ע������÷�Χ��ѡ���Ƿ������ֽ����ļ������˶��ѡ�

### 1.2.2 �����ڷ���

## 1.3 Ԫע��

- ����ע���ע�⣬ͨ������ע��Ķ�����

```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.SOURCE)
public @interface Override {

}
```

�������� @Override ע��Ķ��壬����Կ������е� @Target��@Retention ����ע�����������ν�ġ�Ԫע�⡻����Ԫע�⡻һ������ָ��ĳ��ע�����������Լ�����Ŀ�����Ϣ��

- JAVA �������¼�����Ԫע�⡻

### 1.3.1 @Target��ע�������Ŀ�꣩

����ָ�������ε�ע�����տ������õ�Ŀ����˭��Ҳ����ָ�������ע�⵽�����������η����ģ�������ģ��������������ֶ����Եġ�

@Target �Ķ������£�

```java
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.ANNOTATION_TYPE)
public @interface Target {
    /**
				 * Returns an array of the kinds of elements an annotation type
				 * can be applied to.
				 * @return an array of the kinds of elements an annotation type
				 * can be applied to
				 */
    ElementType[] value();
}
```

���ǿ���ͨ�����µķ�ʽ��Ϊ��� value ��ֵ��

```java
@Target(value = {ElementType.FIELD})
```

����� @Target ע�����ε�ע�⽫ֻ�������ڳ�Ա�ֶ��ϣ������������η��������ࡣ

### 1.3.2 ElementType ��һ��ö������

```java

ElementType.TYPE���������ε�ע���������ࡢ�ӿں�ö����
ElementType.FIELD�����������������ֶ���
ElementType.METHOD�����������ڷ�����
ElementType.PARAMETER�����������ڷ���������
ElementType.CONSTRUCTOR�����������ڹ�������
ElementType.LOCAL_VARIABLE�����������ڱ��ؾֲ�������
ElementType.ANNOTATION_TYPE������������ע����
ElementType.PACKAGE�����������ڰ���
```

### 1.3.3 @Retention��ע�����������

- ����ָ����ǰע����������ڡ�

- �����������£�

  ```java
  @Documented
  		@Retention(RetentionPolicy.RUNTIME)
  		@Target(ElementType.ANNOTATION_TYPE)
  		public @interface Retention {
  			/**
  			 * Returns the retention policy.
  			 * @return the retention policy
  			 */
  			RetentionPolicy value();
  		}
  ```

  ͬ���ģ���Ҳ��һ�� value ����

  ```java
  @Retention(value = RetentionPolicy.RUNTIME
  ```

  

  ����� RetentionPolicy ��Ȼ��һ��ö�����ͣ��������¼���ö��ֵ��ȡ��

  ```java
  RetentionPolicy.SOURCE����ǰע������ڿɼ�������д�� class �ļ�
  RetentionPolicy.CLASS������ؽ׶ζ�������д�� class �ļ�
  RetentionPolicy.RUNTIME�����ñ��棬���Է����ȡ
  ```

## 1.4 JAVA ����������ע��

- ������������Ԫע���⣬JDK ��Ϊ����Ԥ��������������ע�⣬�����ǣ�

  > @Override
  >
  > @Deprecated
  >
  > @SuppressWarnings

  ### 1.4.1 @Override

  ```java
  @Target(ElementType.METHOD)
  @Retention(RetentionPolicy.SOURCE)
  public @interface Override {
  }
  ```

  - ��û���κε����ԣ����Բ����ܴ洢�κ�������Ϣ����ֻ�������ڷ���֮�ϣ���������󽫱�������
  - �����㿴��������һ�ֵ��͵ġ�**���ʽע��**����������������֪���������ڶ� java �ļ����б�����ֽ���Ĺ����У�һ����⵽ĳ�������ϱ������˸�ע�⣬�ͻ�ȥƥ�Ը������Ƿ����һ��ͬ������ǩ���ĺ�����������ǣ���Ȼ����ͨ�����롣

  ### 1.4.2 @Deprecated

  ### 1.4.3 @SuppressWarnings 

  - ��Ҫ����ѹ�� java �ľ���.

  - ����һ�� value ������Ҫ�������Ĵ�ֵ����� value ����һ��ʲô��˼�أ���� value ����ľ�����Ҫ��ѹ�Ƶľ������͡����磺

    ```java
    public static void main(String[] args) {
    	Date date = new Date(2018, 7, 11);
    }
    ```

    > ��ôһ�δ��룬��������ʱ�������ᱨһ�����棺
    >
    > Warning:(8, 21) java: java.util.Date �е� Date(int,int,int) �ѹ�ʱ

  - ��������ǲ�ϣ����������ʱ���������������й�ʱ�ķ������Ϳ���ʹ�� @SuppressWarnings ע�Ⲣ������ value ���Դ���һ������ֵ��ѹ�Ʊ������ļ�顣

    ```java
    @SuppressWarning(value = "deprecated")
    public static void main(String[] args) {
    	Date date = new Date(2018, 7, 11);
    }
    ```

  - ������ͻᷢ�֣����������ټ�� main �������Ƿ��й�ʱ�ķ������ã�Ҳ��ѹ���˱������������־���ļ�顣
    	��Ȼ��JAVA �л��кܶ�ľ������ͣ����Ƕ����Ӧһ���ַ�����ͨ������ value ���Ե�ֵ����ѹ�ƶ�����һ�ྯ�����͵ļ�顣

## 1.5 ע���뷴��

- ���Ǿ�����������Ĳ��濴����ע��ı��ʵ�����ʲô

- ���ȣ������Զ���һ��ע�����ͣ�

  ```java
  package Main;
  
  import java.lang.annotation.*;
  
  @Target(value = {ElementType.FIELD,ElementType.METHOD})
  @Retention(value = RetentionPolicy.RUNTIME)
  public @interface Hello {
      String value();
  }
  ```

- ��������ָ���� Hello ���ע��ֻ�������ֶκͷ��������Ҹ�ע�����ô��Ա����Ƿ����ȡ��֮ǰ����˵����������淶������һϵ�к�ע����ص����Ա�Ҳ����˵���������ֶΡ����������౾�������ע�������ˣ��Ϳ��Ա�д���ֽ����ļ������Ա������¼��֣�

  > RuntimeVisibleAnnotations������ʱ�ɼ���ע��
  > RuntimeInVisibleAnnotations������ʱ���ɼ���ע��
  > RuntimeVisibleParameterAnnotations������ʱ�ɼ��ķ�������ע��
  > RuntimeInVisibleParameterAnnotations������ʱ���ɼ��ķ�������ע��
  > AnnotationDefault��ע����Ԫ�ص�Ĭ��ֵ

- ����ҿ���������⼸��ע����ص����Ա��Ŀ�����ڣ��ô�Ҵ������Ϲ���һ��������
  ӡ��ע�����ֽ����ļ�������δ洢�ġ�

- ���ԣ�����һ������߽ӿ���˵��Class �����ṩ������һЩ�������ڷ���ע�⡣

  > getAnnotation������ָ����ע��
  > isAnnotationPresent���ж���ǰԪ���Ƿ�ָ��ע������
  > getAnnotations���������е�ע��
  > getDeclaredAnnotation�����ر�Ԫ�ص�ָ��ע��
  > getDeclaredAnnotations�����ر�Ԫ�ص�����ע�⣬����������̳ж�����

  �������ֶ�����ط���ע��ķ������������Ƶģ����ﲻ��׸�����������濴һ�����������ӡ�

- ���ȣ�����һ��������������������ڲ��� JDK ��̬�����ࡣ
  **-Dsun.misc.ProxyGenerator.saveGeneratedFiles=true**

  **System.getProperties().put("sun.misc.ProxyGenerator.saveGeneratedFiles","true");**

  ```java
  package Main;
  
  import java.lang.annotation.Annotation;
  import java.lang.reflect.AnnotatedElement;
  import java.lang.reflect.Method;
  
  public class Test {
      @Hello("hello")
      public static void main(String[] args) throws NoSuchMethodException {
          Class cls = Test.class;
          Method method = cls.getMethod("main", String[].class);
          Hello hello = method.getAnnotation(Hello.class);
      }
  }
  ```

  - ����˵����ע�Ȿ�����Ǽ̳��� Annotation �ӿڵĽӿڣ�������ͨ�����䣬Ҳ������������getAnnotation ����ȥ��ȡһ��ע����ʵ����ʱ����ʵ JDK ��ͨ����̬�����������һ��ʵ������ע�⣨�ӿڣ��Ĵ����ࡣ

  - �������г���󣬻ῴ�����Ŀ¼������ôһ�������࣬������֮���������ģ�

  ![](img/annotation1.png)

  ![](img/annotation2.png)

  - ������ʵ�ֽӿ� Hello ����д�����з��������� value �����Լ��ӿ� Hello �� Annotation �ӿڼ̳ж����ķ�����

  - ������ؼ��� InvocationHandler ʵ����˭��

  - AnnotationInvocationHandler �� JAVA ��ר�����ڴ���ע��� Handler�� ���������Ҳ�ǳ�����˼��

    ![](img/annotation3.png)

    ������һ�� memberValues������һ�� Map ��ֵ�ԣ���������ע���������ƣ�ֵ���Ǹ����Ե��������ϵ�ֵ��

    ![](img/annotation4.png)

    ![](img/annotation5.png)

  - ����� invoke �����ͺ�����˼�ˣ����ע�⿴�����ǵĴ���������� Hello �ӿ������еķ��������Զ��ڴ��������κη����ĵ��ö��ᱻת����������

  - var2 ָ�򱻵��õķ���ʵ���������������ñ��� var4 ��ȡ�÷����ļ������ƣ����� switch �ṹ�жϵ�ǰ�ĵ��÷�����˭������� Annotation �е��Ĵ󷽷����� var7 �����ض���ֵ��

  - var2 ָ�򱻵��õķ���ʵ���������������ñ��� var4 ��ȡ�÷����ļ������ƣ����� switch �ṹ�жϵ�ǰ�ĵ��÷�����˭������� Annotation �е��Ĵ󷽷����� var7 �����ض���ֵ��

  - ��ô���� var7 û��ƥ���������ַ�����˵����ǰ�ķ������õ����Զ���ע���ֽ������ķ������������� Hello ע��� value ������**��������£��������ǵ�ע�� map �л�ȡ���ע�����Զ�Ӧ��ֵ��**

## 1.6�Զ���ע�� 

- ע���Ǹ��������ģ�ע���Ǹ�����Ա���ģ��������ߵ�����
- �����Ϊ������: ����ע�⡢ʹ��ע�⡢����ע�⡣

### 1.6.2 ��ע��

```java
import java.lang.annotation.*;

@Inherited
@Target({ ElementType.METHOD, ElementType.TYPE })
@Retention(RetentionPolicy.RUNTIME)
@interface Test {
	String value();
	String test() default "Test";
	int[] numbers() default { 0, 0, 0 };
}

@Test(value = "TestValue", test = "Schwarzes marken", numbers = { 6, 6, 6 })
class A {}

class TestAnnotation extends A {}

public class Main {

	public static void main(String[] args) {

		//ע������ʵ������ֱ��ͨ��Class����
		Test test = TestAnnotation.class.getAnnotation(Test.class);

		System.out.println(test.value() + ": " + test.test() + ": " + test.numbers()[0]);
	}
}
```

### 1.6.1 ����ע�⣺

- ��1������ע��:����һ���򵥵�ע��

```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface MyAnnotation {
    //����ע������ԣ��ⲻ�Ƿ���
    String name();//��ѡע��
    int value() default 20;//�����Ծ��ǿ�ѡ����
}
```

- (2)ע���ʹ��

  ```java
  package annotation;
  
      public class UseMyAnnotion {
  
      //    ����ֻ��һ�����ԣ���һ��value������Ĭ��ֵ��������
      @MyAnnotation(name = "QizoZhi")
      public void show(String str){
      System.out.println(str);
      }
  }
  ```

- ��3������ע��:����ʹ���˵ײ��ӳ��ԭ��

  ```java
  package annotation;
  
  import java.lang.reflect.InvocationTargetException;
  import java.lang.reflect.Method;
  
  public class MyAnnotationParser {
  
      public static void main(String[] args) throws NoSuchMethodException, 
      SecurityException, IllegalAccessException, 
      IllegalArgumentException, InvocationTargetException {
          //        ��ȡ�ֽ������
          Class clazz = UseMyAnnotion.class;
          Method method = clazz.getMethod("show", String.class);
          //        ��ȡ���������ע��
          MyAnnotation annotation = method.getAnnotation(MyAnnotation.class);
          //        ��ȡע������ֵ
          System.out.println(annotation.name()+"\t"+annotation.value());
  
          //        ȡ��ֵ�Ϳ��Ը���ҵ��������
  
          //�������Ҳ�����÷���ִ��
          method.invoke(new UseMyAnnotion(), "HH");
      }
  }
  ```

## 1.7 ע���ʵ��ԭ��

- ʵ��ע����Ҫ�أ�

  > 1. ע������
  >
  > 2. ʹ��ע���Ԫ��
  >
  > 3. ����ע��ʹ��������(ע�⴦����)
  >
  >    �����ע��ʹ�õĺ����ˣ�ǰ������˵����ô��ע����صģ��ǵ���java�����ȥ������Щע����ش�**getAnnotation**��ȥ���Կ���**java.lang.classʵ����AnnotatedElement**����
  >
  >    MyAnTargetType t = AnnotationTest.class.**getAnnotation**(MyAnTargetType.class);

  ```java
  public final class Class<T> implements java.io.Serializable,
  
  									  GenericDeclaration,
  
  									  Type,
  
  									  AnnotatedElement
  ```

  - **java.lang.reflect.AnnotatedElement** �ӿ������г���Ԫ�أ�Class��Method��Constructor���ĸ��ӿڣ����Գ���ͨ�������ȡ��ĳ�����AnnotatedElement����֮�󣬳���Ϳ��Ե��øö���������ĸ�������������Annotation��Ϣ��

  ```xml
  ����1��<T extends Annotation> T getAnnotation(Class<T> annotationClass): 
  				���ظó���Ԫ���ϴ��ڵġ�ָ�����͵�ע�⣬���������ע�ⲻ���ڣ��򷵻�null��
  		����
  ����2��Annotation[] getAnnotations():���ظó���Ԫ���ϴ��ڵ�����ע�⡣
  		��    
  ����3��boolean isAnnotationPresent(Class<?extends Annotation>
  				annotationClass):�жϸó���Ԫ�����Ƿ����ָ�����͵�ע�⣬�����򷵻�true�����򷵻�false.
  		����
  ����4��Annotation[] getDeclaredAnnotations()������ֱ�Ӵ����ڴ�Ԫ���ϵ�����ע�͡�
  				��˽ӿ��е�����������ͬ���÷��������Լ̳е�ע�͡������û��ע��ֱ�Ӵ����ڴ�Ԫ��				�ϣ��򷵻س���Ϊ���һ�����顣���÷����ĵ����߿��������޸ķ��ص����飻
  				�ⲻ������������߷��ص���������κ�Ӱ��
  ```

  

## 1.8 �ܽ�һ����������ע��Ĺ���ԭ��

- ���ȣ�����ͨ����ֵ�Ե���ʽ����Ϊע�����Ը�ֵ����������@Hello��value = "hello"����
- ���ţ�����ע������ĳ��Ԫ�أ����������ڱ�����ɨ��ÿ������߷����ϵ�ע�⣬����һ�������ļ�飬������ע���Ƿ����������ڵ�ǰλ�ã����Ὣע����Ϣд��Ԫ�ص����Ա�
- Ȼ�󣬵�����з����ʱ����������������������� RUNTIME ��ע��ȡ�����ŵ�һ�� map �У�������һ�� AnnotationInvocationHandler ʵ��������� map ���ݸ�����
- �������������� JDK ��̬�����������һ��Ŀ��ע��Ĵ����࣬����ʼ���ô�������
- ��ô������һ��ע���ʵ���ʹ��������ˣ��������Ͼ���һ�������࣬��Ӧ��ȥ���� AnnotationInvocationHandler �� invoke ������ʵ���߼������Ǻ��ġ�һ�仰�������ǣ�**ͨ������������ע������ֵ**��