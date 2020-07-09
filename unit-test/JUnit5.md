# JUnit 5 Vintage 和 JUnit Jupiter 扩展模型

了解用于参数注入、参数化测试、动态测试和自定义注解的 JUnit Jupiter 扩展

## JUnit Vintage

### JUnit 3 中的测试



### JUnit 4 中的测试



## JUnit Jupiter 扩展模型

### 扩展 JUnit 4 的核心功能

过去，希望扩展 JUnit 4 核心功能的开发人员或工具供应商会使用 `Runner` 和 `@Rule`。

*Runner* 通常是 `BlockJUnit4ClassRunner` 的子类，用于提供 JUnit 中没有直接提供的某种行为。目前有许多第三方 `Runner`，比如用于运行基于 Spring 的单元测试的 `SpringJUnit4ClassRunner`，以及用于处理单元测试中 Mockito 对象的 `MockitoJUnitRunner`。

必须在测试类级别上使用 `@RunWith` 注解来声明 `Runner`。`@RunWith` 接受一个参数：`Runner` 的实现类。因为每个测试类最多只能拥有一个 `Runner`，所以每个测试类最多也只能拥有一个扩展点。

为了解决 `Runner` 概念的这一内置限制，JUnit 4.7 引入了 `@Rule`。一个测试类可声明多个 `@Rule`，这些规则可在测试方法级别和类级别上运行（而 `Runner` 只能在类级别上运行）。

### 扩展点和测试生命周期

一个扩展点对应于 JUnit test 生命周期中一个预定义的点。从 Java™ 语言的角度讲，*扩展点*是您实现并向 JUnit 注册（激活）的回调接口。因此，*扩展点*是回调接口，*扩展*是该接口的实现。

在本教程中，我将把已实现的扩展点回调接口称为*扩展*。

一旦注册您的扩展，就会将其激活。在测试生命周期中合适的点上，JUnit 将使用回调接口调用它。

| 接口                          | 说明                                                         |
| :---------------------------- | :----------------------------------------------------------- |
| AfterAllCallback              | 定义 API 扩展，希望在调用所有测试后让测试容器执行额外的行为。 |
| AfterEachCallback             | 定义 API 扩展，希望在调用每个测试方法后让测试执行额外的行为。 |
| AfterTestExecutionCallback    | 定义 API 扩展，希望在执行每个测试后让测试立即执行额外的行为。 |
| BeforeAllCallback             | 定义 API 扩展，希望在调用所有测试前让测试容器执行额外的行为。 |
| BeforeEachCallback            | 定义 API 扩展，希望在调用每个测试前让测试执行额外的行为。    |
| BeforeTestExecutionCallback   | 定义 API 扩展，希望在执行每个测试前让测试立即执行额外的行为。 |
| ParameterResolver             | 定义 API 扩展，希望在运行时动态解析参数。                    |
| TestExecutionExceptionHandler | 定义 API 扩展，希望处理在测试执行期间抛出的异常。            |

### 创建扩展

要创建扩展，只需实现该扩展点的回调接口。假设我想创建一个在每个测试方法运行之前就运行的扩展。在此情况下，我只需要实现 `BeforeEachCallback` 接口：

```java
public class MyBeforeEachCallbackExtension implements BeforeEachCallback {
  @Override
  public void beforeEach(ExtensionContext context) throws Exception {
    // Implementation goes here
  }
}
```

实现扩展点接口后，需要激活它，这样 JUnit 才能在测试生命周期中合适的点调用它。通过注册扩展来激活它。

### 激活扩展

要激活上述扩展，只需使用 `@ExtendWith` 注解注册它：

```java
@ExtendWith(MyBeforeEachCallbackExtension.class)
public class MyTestClass {
    @Test
    public void myTestMethod() {
        // Test code here
    }
    @Test
    public void someOtherTestMethod() {
        // Test code here
    }
}
```

当 `MyTestClass` 运行时，在执行每个 `@Test` 方法前，会调用 `MyBeforeEachCallbackExtension`。

注意，这种注册扩展的风格是*声明性的*。JUnit 还提供了一种自动注册机制，它使用了 Java 的 `ServiceLoader` 机制

## 参数注入

如何将一个参数传递给 `@Test` 方法。

### ParameterResolver 接口

如果所编写的测试方法在其签名中包含一个参数，则必须将该参数解析为一个实际对象，然后 JUnit 才能调用该方法。

一种*乐观的场景*如下所示：

(1) JUnit 寻找一个实现 `ParameterResolver` 接口的已注册扩展；

(2) 调用它来解析该参数；

(3) 然后调用您的测试方法，传入解析后的参数值。

```java
package org.junit.jupiter.api.extension;

import static org.junit.platform.commons.meta.API.Usage.Experimental;
import java.lang.reflect.Parameter;
import org.junit.platform.commons.meta.API;

@API(Experimental)
public interface ParameterResolver extends Extension {

    boolean supportsParameter(ParameterContext parameterContext, 
                              ExtensionContext extensionContext)
        throws ParameterResolutionException;

    Object resolveParameter(ParameterContext parameterContext, 
                            ExtensionContext extensionContext)
        throws ParameterResolutionException;

}
```

Jupiter 测试引擎需要解析您的测试类中的一个参数时，它首先会调用 `supports()` 方法，查看该扩展是否能处理这种参数类型。如果 `supports()` 返回 `true`，则 Jupiter 测试引擎调用 `resolve()` 来获取正确类型的 `Object`，随后在调用测试方法时会使用该对象。

如果未找到能处理该参数类型的扩展，您会看到一条与下面类似的消息：

```log
org.junit.jupiter.api.extension.ParameterResolutionException: 
No ParameterResolver registered for parameter [java.lang.String arg0] in executable 
[public void com.makotojava.learn.junit5.PersonDaoBeanTest$WhenDatabaseIsPopulated.findAllByLastName(java.lang.String)].
```

### 创建 ParameterResolver 实现

```java
import org.junit.jupiter.api.extension.ExtensionContext;
import org.junit.jupiter.api.extension.ParameterContext;
import org.junit.jupiter.api.extension.ParameterResolutionException;
import org.junit.jupiter.api.extension.ParameterResolver;

import com.makotojava.learn.junit.Person;
import com.makotojava.learn.junit.PersonGenerator;

public class GeneratedPersonParameterResolver implements ParameterResolver {

    @Override
    public boolean supportsParameter(ParameterContext parameterContext, ExtensionContext extensionContext)
        throws ParameterResolutionException {
        return parameterContext.getParameter().getType() == Person.class;
    }

    @Override
    public Object resolveParameter(ParameterContext parameterContext, ExtensionContext extensionContext)
        throws ParameterResolutionException {
        return PersonGenerator.createPerson();
    }

}
```

在这个特定的用例中，如果参数的类型是 `Person`，则 `supports()` 返回 `true`。JUnit 需要将参数解析为 `Person` 对象时，它调用 `resolve()`，后者返回一个新生成的 `Person` 对象。

### 使用 ParameterResolver 实现

要使用 `ParameterResolver`，必须向 JUnit Jupiter 测试引擎注册它。与前面的演示一样，可使用 `@ExtendWith` 注解完成注册工作。

```java
@DisplayName("Testing PersonDaoBean")
@ExtendWith(GeneratedPersonParameterResolver.class)
public class PersonDaoBeanTest extends AbstractBaseTest {
    @Test
    @DisplayName("Add generated Person should succeed - uses Parameter injection")
    public void add(Person person) {
        assertNotNull(classUnderTest, "PersonDaoBean reference cannot be null.");
        Person personAdded = classUnderTest.add(person);
        assertNotNull(personAdded, "Add failed but should have succeeded");
        assertNotNull(personAdded.getId());
        performPersonAssertions(person.getLastName(), person.getFirstName(), person.getAge(), person.getEyeColor(),
                                person.getGender(), personAdded);
    }
}
```

`PersonDaoBeanTest` 类运行时，它将向 Jupiter 测试引擎注册 `GeneratedPersonParameterResolver`。每次需要解析一个参数时，就会调用自定义 `ParameterResolver`。

扩展有一个影响范围 - 类级别或方法级别。

在这个特定的用例中，我选择在类级别注册扩展（第 2 行）。在类级别注册意味着，接受*任何*参数的任何测试方法都会导致 JUnit 调用 `GeneratedPersonParameterResolver` 扩展。如果参数类型为 `Person`，则返回一个已生成的 `Person` 对象并将其传递给测试方法（第 8 行）。

要将扩展的范围缩小到单个方法，可按如下方式注册扩展：

```java
@Test
@DisplayName("Add generated Person should succeed - uses Parameter injection")
@ExtendWith(GeneratedPersonParameterResolver.class)
public void add(Person person) {
    assertNotNull(classUnderTest, "PersonDaoBean reference cannot be null.");
    Person personAdded = classUnderTest.add(person);
    assertNotNull(personAdded, "Add failed but should have succeeded");
    assertNotNull(personAdded.getId());
    performPersonAssertions(person.getLastName(), person.getFirstName(), person.getAge(), person.getEyeColor(),
                            person.getGender(), personAdded);
}
```

现在，系统只会调用该扩展来解析 `add()` 测试方法的参数。如果类中的任何其他测试方法需要参数解析，它们需要一个不同的 `ParameterResolver`。

**注意**，任何给定类的特定范围上只能有一个`ParameterResolver`。举例而言，如果您已经为在类级别上声明的 `Person` 对象提供了一个 `ParameterResolver`，并在同一个类中为在方法级别上声明的对象提供了另一个 `ParameterResolver`，那么 JUnit 就不知道使用哪一个。

## 参数化测试

*参数化测试*是指多次调用 `@Test` 方法，但每次都使用不同的参数值。参数化测试必须使用 `@ParameterizedTest` 进行注解，而且必须为其参数指定一个*来源*。

JUnit Jupiter 提供了多个来源。每个来源指定一个 `@ArgumentsSource`，也就是一个 `ArgumentsProvider` 实现。本节将介绍如何使用 3 个来源：

- `@ValueSource`
- `@EnumSource`
- `@MethodSource`

每个来源都在所允许的数据类型的易用性与灵活性之间进行了折中。最容易使用但最不灵活的是 `@ValueSource`。最灵活的是 `@MethodSource`，允许您使用所选的任何复杂对象来参数化测试方法。（注意，`@MethodSource` 也是最难使用的。）

### @ValueSource

在 `@ValueSource` 中，您指定单个文字值数组，系统将这些文字值 — 一次一个地 — 提供给您的 `@ParameterizedTest` 方法。

语法类似于：

```java
@ParameterizedTest
@ValueSource(longs = { 1L, 2L, 3L, 4L, 5L })
public void findById(Long id) {
    assertNotNull(classUnderTest);
    Person personFound = classUnderTest.findById(id);
    assertNotNull(personFound);
    assertEquals(id, personFound.getId());
}
```

首先您告诉 JUnit，`findById()` 方法是一个 `@ParameterizedTest`，如上面第 1 行所示。然后使用数组初始化器语法来指定数组，如第 2 行所示。JUnit 将调用 `findById()` 测试方法，每次将数组中的下一个 `long` 传递给该方法（第 3 行），直到用完数组。您可像任何 Java 方法参数一样使用该参数（第 5 行）。

作为数组名所提供的 `@ValueSource` 属性名必须全部采用小写，而且必须与其末尾有字母 *s* 的类型相匹配。例如，`ints` 与 `int` 数组匹配，`strings` 与 `String` 数组匹配，等等。

并不支持所有的原语类型，仅支持以下类型：

- `String`
- `int`
- `long`
- `double`

### @EnumSource

在 `@EnumSource` 中，您指定一个 `enum`，JUnit — 一次一个地 — 将其中的值提供给 `@ParameterizedTest` 方法。

语法类似于：

```java
@ParameterizedTest
@EnumSource(PersonTestEnum.class)
public void findById(PersonTestEnum testPerson) {
    assertNotNull(classUnderTest);
    Person person = testPerson.getPerson();
    Person personFound = classUnderTest.findById(person.getId());
    assertNotNull(personFound);
    performPersonAssertions(person.getLastName(), person.getFirstName(), person.getAge(), person.getEyeColor(),
                            person.getGender(), personFound);
}
```

首先您告诉 JUnit，`findById()` 方法是一个 `@ParameterizedTest`，如第 1 行所示。然后指定该 `enum` 的 Java 类，如第 2 行所示。JUnit 将调用 `findById()` 测试方法，每次将下一个 `enum` 值传递给该方法（第 3 行），直到用完该 `enum`。您可像任何 Java 方法参数一样使用该参数（第 5 行）。

### @MethodSource

使用注解 `@MethodSource`，可以指定您喜欢的任何复杂对象作为测试方法的参数类型。语法类似于：

```java
@ParameterizedTest
@MethodSource(value = "personProvider")
public void findById(Person paramPerson) {
    assertNotNull(classUnderTest);
    long id = paramPerson.getId();
    Person personFound = classUnderTest.findById(id);
    assertNotNull(personFound);
    performPersonAssertions(paramPerson.getLastName(), paramPerson.getFirstName(),
                            paramPerson.getAge(),
                            paramPerson.getEyeColor(), paramPerson.getGender(), personFound);
}
```

`@MethodSource` 的 `value` 属性用于指定一个或多个方法名，这些方法为测试方法提供参数。一个方法来源的返回类型必须是 `Stream`、`Iterator`、`Iterable` 或数组。此外，提供者方法必须声明为 `static`，所以不能将它用在 `@Nested` 测试类内（至少截至 JUnit 5 Milestone 5 时不能这么做）。

在上面的示例中，`personProvider` 方法类似于：

```java
static Iterator<Person> personProvider() {
    PersonTestEnum[] testPeople = PersonTestEnum.values();
    Person[] people = new Person[testPeople.length];
    for (int aa = 0; aa < testPeople.length; aa++) {
        people[aa] = testPeople[aa].getPerson();
    }
    return Arrays.asList(people).iterator();
}
```

假设您想为测试方法添加一个额外的参数提供者。可以这样声明它：

```java
@ParameterizedTest
@MethodSource(value = { "personProvider", "additionalPersonProvider" })
public void findById(Person paramPerson) {
    assertNotNull(classUnderTest);
    long id = paramPerson.getId();
    Person personFound = classUnderTest.findById(id);
    assertNotNull(personFound);
    performPersonAssertions(paramPerson.getLastName(), paramPerson.getFirstName(),
                            paramPerson.getAge(),
                            paramPerson.getEyeColor(), paramPerson.getGender(), personFound);
}
```

我们使用数组初始化器语法指定这些方法（第 2 行），而且将按指定的顺序调用各个方法，最后调用的是 `additionalPersonProvider()`。

### 自定义显示名称

参数化测试的缺省显示名称包含测试索引（一个从 1 开始的迭代编号），以及该参数的 `String` 表示。如果测试类中有多个测试方法，那么输出容易让人混淆。幸运的是，可以通过向 `@ParameterizedTest` 注解提供任何以下属性值来自定义输出：

- *{index}*：从 1 开始的索引（当前测试迭代 ）。
- *{arguments}*：完整的参数列表，使用逗号分隔。
- *{0}, {1} ...*：一个特定的参数（0 是第一个，依此类推）。

举例而言，假设提供了一个包含 5 个 `long` 的数组。在此情况下，可像这样注解 `@ParameterizedTest`

```java
@ParameterizedTest(name = "@ValueSource: FindById(): Test# {index}: Id: {0}")
@ValueSource(longs = { 1L, 2L, 3L, 4L, 5L })
public void findById(Long id) {
    assertNotNull(classUnderTest);
    Person personFound = classUnderTest.findById(id);
    assertNotNull(personFound);
    assertEquals(id, personFound.getId());
}
```

```log
将会生成以下输出：

@ValueSource: FindById(): Test# 1: Id: 1
@ValueSource: FindById(): Test# 2: Id: 2
@ValueSource: FindById(): Test# 3: Id: 3
@ValueSource: FindById(): Test# 4: Id: 4
@ValueSource: FindById(): Test# 5: Id: 5
```

## 动态测试

目前为止，我们分析的都是*静态测试*，这意味着测试代码、测试数据和测试的通过/失败条件在编译时都是已知的。

JUnit Jupiter 引入了一种称为*动态测试*的新测试类型，这种测试在运行时由一个称为*测试工厂*的特殊方法生成。

### @TestFactory

`@TestFactory` 方法用于生成动态测试。此方法必须返回 `DynamicTest` 实例的 `Stream`、`Collection`、`Iterable` 或 `Iterator`。

不同于 `@Test` 方法，`DynamicTest` 实例没有生命周期回调。所以 `@BeforeEach`、`@AfterEach` 和表 1 中的其他生命周期回调都不适用于 `DynamicTest`。

### 创建 @TestFactory

```java
@TestFactory
@DisplayName("FindById - Dynamic Test Generator")
Stream<DynamicTest> generateFindByIdDynamicTests() {
    Long[] ids = { 1L, 2L, 3L, 4L, 5L };
    return Stream.of(ids).map(id -> dynamicTest("DynamicTest: Find by ID " + id, () -> {
        Person person = classUnderTest.findById(id);
        assertNotNull(person);
        int index = id.intValue() - 1;
        Person testPerson = PersonTestEnum.values()[index].getPerson();
        performPersonAssertions(testPerson.getLastName(), testPerson.getFirstName(),
                                testPerson.getAge(), testPerson.getEyeColor(), testPerson.getGender(), person);
    }));
}
```

`@TestFactory` 注解将此方法标记为一个 `DynamicTest` 工厂（第 1 行），并根据 JUnit Jupiter 的要求返回 `DynamicTest` 实例的一个 `Stream`（第 2 行）。该 `@TestFactory` 所生成的测试不会执行任何花哨的操作；它们仅在 `PersonDaoBean` Spring bean 上调用 `findById`（第 6 行），并执行一些断言（第 10 和 11 行）。但它展示了如何创建一个动态测试。

## 标签和过滤

标签对过滤测试很有用。在本节中，我们将了解如何创建一个自定义过滤器，然后将它转换为一个组合注解，用于控制哪些测试可运行。

### 使用 @Tags

JUnit Jupiter *标签*描述`@Tag` 注解的用法，该注解创建一个新的标识符（标签），并接受单个 `String` 参数来唯一地标识该标签。下面给出了一些示例：

```java
@Tag("foo")
@Tag("bar")
@Tag("advanced")
```

您可使用标签来注解方法或类，比如：

```java
@Tag("advanced")
@TestFactory
@DisplayName("FindById - Dynamic Test Generator")
Stream<DynamicTest> generateFindByIdDynamicTests() {
    Long[] ids = { 1L, 2L, 3L, 4L, 5L, 6L };
    return Stream.of(ids).map(id -> dynamicTest("DynamicTest: Find by ID " + id, () -> {
        Person person = classUnderTest.findById(id);
        assertNotNull(person);
        int index = id.intValue() - 1;
        Person testPerson = PersonTestEnum.values()[index].getPerson();
        performPersonAssertions(testPerson.getLastName(), testPerson.getFirstName(),
                                testPerson.getAge(), testPerson.getEyeColor(), testPerson.getGender(), person);
    }));
}
```

然后可使用 Maven POM 或 Gradle 构建脚本中的过滤器设置来过滤掉此测试。后面将介绍如何执行该操作。

### 创建自己的组合注解

与使用 `@Tag` 和它的唯一名称相比， 使用`@Tag` 创建新的*组合注解*更重要。还记得上节中的 `@Tag("advanced")` 吗？我可以创建一个新的组合注解来表示一种高级测试类型，比如：

```java
import static java.lang.annotation.ElementType.METHOD;
import static java.lang.annotation.ElementType.TYPE;
import static java.lang.annotation.RetentionPolicy.RUNTIME;

import java.lang.annotation.Retention;
import java.lang.annotation.Target;

import org.junit.jupiter.api.Tag;

@Retention(RUNTIME)
@Target({ TYPE, METHOD })
@Tag("advanced")
public @interface Advanced {
    // Nothing to do
}
```

在，我在所有使用 `@Tag("advanced")` 的地方都使用 `@Advanced` 来代替，如下所示：

```java
@Advanced
@TestFactory
@DisplayName("FindById - Dynamic Test Generator")
Stream<DynamicTest> generateFindByIdDynamicTests() {
    Long[] ids = { 1L, 2L, 3L, 4L, 5L, 6L };
    return Stream.of(ids).map(id -> dynamicTest("DynamicTest: Find by ID " + id, () -> {
        Person person = classUnderTest.findById(id);
        assertNotNull(person);
        int index = id.intValue() - 1;
        Person testPerson = PersonTestEnum.values()[index].getPerson();
        performPersonAssertions(testPerson.getLastName(), testPerson.getFirstName(),
                                testPerson.getAge(), testPerson.getEyeColor(), testPerson.getGender(), person);
    }));
}
```

如前所述，可在类级别或方法级别上使用新的组合注解。可以查看示例应用程序中的 `PersonDaoBeanRepeatedTest` 类，看看这么做的实际效果，在其中使用 `@Advanced` 注解了整个类。在 `PersonDaoBeanTest` 中，我只将生成动态测试的 `generateFindByIdDynamicTests()` 方法标记为 `@Advanced`。

## 使用 Maven 运行

```shell
$ mvn clean test
```

打开 POM，找到 Maven surefire 插件，添加 `configuration` 元素：

```xml
<build>
    <plugins>
        <plugin>
            <artifactId>maven-surefire-plugin</artifactId>
            <version>2.19</version>
            <configuration>
                <properties>
                    <excludeTags>advanced</excludeTags>
                </properties>
            </configuration>
    </plugins>
</build>
```

再次运行构建内容，您应该看到运行的测试更少了。