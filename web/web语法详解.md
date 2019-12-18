# HTML

​	`Hyper Text Mark-up Language` (超文本标记语言) 的缩写。它规定了自己的语法规则，用来表示比“文本”更丰富的意义，比如图片，表格，链接等。浏览器（IE,FireFox等）软件知道HTML语言的语法，可以用来查看HTML文档。目前互联网上的绝大部分网页都是使用HTML编写的。

​	简单地来说，HTML的语法就是给文本加上表明文本含义的标签(Tag)，让用户（人或程序）能对文本得到更好的理解。



```html
<html>
    <head>
        <title>第一个Html文档</title>
    </head>
    <body>
        欢迎访问<a href="http://deerchao.net">deerchao的个人网页</a>!
    </body>
</html>
```



```txt
所有的HTML文档都应该有一个<html>标签。

<html>标签可以包含两个部分:<head>和<body>。
	
	<head>标签用于包含整个文档的一般信息，比如文档的标题（<title>标签用于包含标题），对整个文档的描述，文档的关键字等等。
	文档的具体内容就要放在<body>标签里了。
	
<a>标签用于表示链接。
```



## 标签

在HTML文本中，用尖括号括起来的部分称为标签。

如果想在正文里使用尖括号（或者大与号小与号，总之是同一个东西），必须使用字符转义，也就是说转换字符的原有意义。<应该使用&lt;代替，>则使用&gt;，至于&符号本身,则应该使用&amp;替代（不得不说的是有很多HTML文档没有遵循这个规则，常用的浏览器也都能够分析出&到底是一个转义的开始，还是一个符号，但是这样做是不推荐的）。



```html
<img src="logo.gif" />
```



通过不同的标签，HTML文档可以包含不同的内容，比如文本，链接，图片，列表，表格，表单，框架等。

### 文本

HTML对文本的支持是最丰富的，你可以设置不同级别的标题，分段和换行，可以指定文本的语义和外观，
可以说明文本是引用自其它的地方，等等等等。



```html
<font color="red" size="5">红色的5</font>
<font face="黑体">黑体的字</font>
```

加粗，下划线，斜体字也是常用的文字效果，它们分别用<b>,<u>,<i>表示：
```html
<b>Bold</b>
<i>italic</i>
<u>underline</u>
```
### 标题

```html
<HTML>

<HEAD>

<TITLE>ALIGN属性的用法</TITLE>

</HEAD>

<BODY>

<P><H1 ALIGN="LEFT">我最大，靠左<H1></P>

<P><H2 ALIGN="CENTER">我第二，居中<H2></P>

<P><H3 ALIGN="RIGHT">我最小，靠右<H3></P>

</BODY></HTML>
```

#### 属性

```html
（1）ALIGN={LEFT， CENTER， RIGHT}：文字左对齐（LEFT）、居中（CENTER）或右对齐（RIGHT）。

（2）ID：指定标记符的ID选择器。

（3）STYLE：指定标记符的样式表命令。

（4）CLASS：指定标记符的样式类型。

（5）LANG：指定标题文字的语种。

（6）DIR：指定标题文字的方向。

（7）TITLE：指定标记符的标题。

（8）OnClink：指定当鼠标在标记符上按一下时所要执行的脚本。

（9）OnDblClink：指定当鼠标在标记符上按两下时所要执行的脚本。

（10）OnMouseUp：指定当鼠标在标记符上放开按键时所要执行的脚本。

（11）OnMouseDown：指定当鼠标在标记符上按下按键时所要执行的脚本。

（12）OnMouseOver：指定当鼠标移过标记符时所要执行的脚本。

（13）OnMouseMove：指定当鼠标在标记符上移动时所要执行的脚本。

（14）OnMouseOut：指定当鼠标自标记符上移开时所要执行的脚本。

（15）OnKeyPress：指定在标记符上按下再放开按键时所要执行的脚本。

（16）OnKeyUp：指定在标记符上放开按键时所要执行的脚本。
```



### 段落

```html
<P />
<br />
<BR /> 分隔符,水平线
```

#### 属性

包括`ALIGN、SIZE、WIDTH、COLOR、NOSHADE`等属性 

```html
<HTML>

<HEAD>

<TITLE>水平线属性设置</TITLE></HEAD>

<BODY>

<HR><HR WIDTH="50%"COLOR="#000080">

<HR WIDTH="300"COLOR="#0000FF"ALIGN="LEFT"SIZE="4">

<HR WIDTH="300"COLOR="FF0000"ALIGN="LEFT"NOSHADE>

</BODY>

</HTML>
```



#### 段落对齐

在HTML中，一般使用标记符的ALIGN属性设置段落对齐方式。 

- ALIGN属性常见取值有4种：RIGHT（右对齐）、LEFT（左对齐）、CENTER（居中对齐）、JUSTIFY（两端对齐）。 
- DIV标记符。 DIV标记符用于为文件分节，目的是为文件的不同部分应用不同的段落格式 
- CENTER标记符。要将文件内容居中，除了可以使用<DIV ALIGN="CENTER"> </DIV>标记符外，还可使用CENTER标记符，方法为：将需居中的内容置于<CENTER>和</CENTER>之间。 



### 文字格式

- 字符格式

| 标记符范例                | 功能说明                               |
| ------------------------- | -------------------------------------- |
| <B>Bold</B>               | 粗体                                   |
| <I>Italic</I>             | 斜体                                   |
| <u>Underlined</u>         | 下划线                                 |
| H<SUB>2</SUB>o            | 下标                                   |
| X<SUP>3</SUP>             | 上标                                   |
| <BIG>BIG</BIG>            | 大写体                                 |
| <SMALL>small</SMALL>      | 小写体                                 |
| <TT>t</TT>                | 固定宽度字体                           |
| <STRIKE>Strike</STRIKE>   | 删除字                                 |
| <S>Strike</S>             | 删除字                                 |
| <EM>强调斜体</EM>         | 用于强调某些字体为斜体                 |
| <STRONG>强调粗体</STRONG> | 用于强调某些字体为粗体                 |
| <DFN>Definition</DFN>     | 用于表示定义了的文字，通常是黑体或斜体 |
| <CITE>Citation</CITE>     | 用于表示文本属性引用，通常是斜体       |
| <CODE>Code</CODE>         | 表示程序代码文字，通常是固定宽度       |
| <KBD>Keyboard</KBD>       | 表示键盘输入文字，通常是固定宽度       |
| <SAMP>SAMPLE</SAMP>       | 表示文本样本，通常是固定宽度           |
| <VAR>Variable</VAR>       | 变数文字，通常是斜体                   |
| <ABBR>如HTTP</ABBR>       | 表示缩写文字                           |



19个字符标记符，其中前10个样式标记符用来为某些文字设置特殊格式，我们称其为物理字符样式，

而后9个样式标记符不仅在文档中可以指定特定文字的格式，而且还能标出文字的含义，我们称其为逻辑字符样式。

不管是物理字符样式还是逻辑字符样式，使用时只需将设置格式的字符括在标记符之间即可。 



```html
<HTML>

<HEAD><TITLE>字符样式设置练习</TITLE></HEAD>

<BODY><P><B>物华天宝、人杰地灵（粗体）</B></P>

<P><I>物华天宝、人杰地灵（斜体）<I></P>

<P><BIG>物华天宝、人杰地灵（大字体）</BIG></P>

<P><SMALL>物华天宝、人杰地灵（小字体）</SMALL></P>

<P><S>此处为使用&lt;S&gt;标记符设置的删除线文本</S></P>

<P>上标示例：x<SUP>3</SUP>+Y<SUP>4</SUP></P>

<P>下标示例：H<SUB>2</SUB>s<P>

</BODY>

</HTML>
```



#### 字体大小、字符颜色和字体样式

 FONT标签 3个属性，即`SIZE、COLOR和FACE`来改变网页的风格。 

FACE： 指定字体样式 默认的中文字体是“宋体”，英文字体是Times New Roman。

但要注意，系统必须安装有FACE属性所指定的字体，所以在指定字体时最好使用几种字体，以增加浏览器找到匹配字体的机会。

```html
<HTML>

<HEAD><TITLE>设置字体的大小颜色及样式</TITLE></HEAD>

<BODY>

<P><FONT SIZE="1"FACE="华文细黑"COLOR="#008000">染红了三湘大地</FONT></P>

<P><FONT SIZE="3">物华天宝人杰地灵</FONT></P>

<P><FONT SIZE="2"FACE"华文中宋"><FONT COLOR="#000080">风起云涌英才辈出</FONT></FONT></P>

<P><FONT SIZE="5"FACE="幼圆"COLOR="#800080"> 物华天宝人杰地灵</FONT></P>

<P><FONT SIZE="6"FACE="华文彩云"COLOR="#008080" >染红了三湘大地</FONT></P>

</BODY>

</HTML>
```











### 链接

链接用来指出内容与另一个页面或当前页面某个地方有关。



超级链接用<a>标签表示，href属性指定了链接到的地址。<a>标签可以包含文本，也可以包含图片。

#### 相关属性

```html
（1）HREF="URL"：是超链接最基本也是最常用的一个属性，用以指定超链接所连接的文件的相对或绝对位置。

（2）NAME="…"：NAME也是一个比较基本的属性，用以创建书签，指定书签名称。

（3）TARGET="…"：在框架网页中该属性很重要，用以指定目标框架的名称。

（4）ACCESSKEY="…"：指定超链接的存取按键，当浏览者按下ACCESSKEY属性所指定的按键时，网页的焦点就会移动到组件上。

（5）ET="…"：指定超链接的字元编码方式。

（6）REV="…"：从HREF指定的文件到当前文件之间的关联。

（7）TYPE="…"：指定内容类型{content type}。

（8）HREFLANG="langcode"：指定HREF属性值的语种。

（9）TABINDEX="n"：指定<A>组件在网页中的TAB键顺序值。
```





```html
<a href="http://deerchao.net">deerchao的个人网站</a>
<a href="http://validator.w3.org"><img src="http://www.w3.org/Icons/valid-xhtml10" alt="验证HTML" /></a>
```



### 图片

img标签。图片用于使页面更加美观，或提供更多的信息。



#### 相关属性

```html
（1）SRC：设置图片的相对或绝对路径。

（2）ALT：设置图片的简单文本说明。

（3）ALIGN：设置图片的对齐方式。

（4）BORDER：设置图片的框线粗细。

（5）HEIGHT：设置图片的高度。

（6）WIDTH：设置图片的宽度。

（7）HSPACE：设置图片的水平间距。

（8）VSPACE：设置图片的垂直间距。

（9）NAME：设置图片的名称，以供Script、Applet或书签使用。

（10）USEMAP：设置热点链接所在的位置及名称。

（11）LONGDESC：设置图片的说明文字。

（12）LOWSRC：设置低分辨率图片的相对或绝对位置。
```





```html
<img src="http://www.w3.org/Icons/valid-xhtml10" alt="图片简介" />

<hr width="90%" color="red" />
# <hr>标签用于在页面上添加横线。可以通过指定width和color属性来控制横线的长度和颜色。
```



### 分段与换行

- 分段换行

```html
<p>这是第一段。</p>
<p>这是第二段。</p>
```

- 换行

```html
这是第一段。<br>
这是第二段。<br />
这是第三段。
```

- 把文档看作不同的部分组合

```html
<div>页头内容</div>
<div>主体内容</div>
<div>页脚内容</div>
```



### 列表

列表用于说明一系列条目是彼此相关的。



- 列表可以分为无序列表（<ul>），有序列表（<ol>）和定义列表（<dl>）。前两种列表更常见一些，都用<li>标签包含列表项目。无序列表表示一系列类似的项目，它们之间没有先后顺序。

```html
<ul>
    <li>苹果</li>
    <li>桔子</li>
    <li>桃</li>
</ul>

有序列表中各个项目间的顺序是很重要的，浏览器通常会自动给它们产生编号。
<ol>
    <li>打开冰箱门</li>
    <li>把大象赶进去</li>
    <li>关上冰箱门</li>
</ol>
```



### 表格

表格是按行与列将数据组织在一起的形式。也有不少人使用表格进行页面布局。



#### 属性

<TABLE>标记符的属性如下：

```html
（1）BORDER="n"：以指定粗度显示表格边框。

（2）ALING="ALIGNMENT"：表格的对齐方式（LEFT，CENTER，RIGHT）。

（3）WIDTH="n"：整个表格的固定宽度（n可以为像素点或以%表示的百分比）。

（4）BGCOLOR="COLOR"：定义表格的背景色。

（5）BORDERCOLOR="COLOR"：定义表格边框的颜色。

（6）BORDERCOLORLIGHT="COLOR"：定义3 D表格边框亮色部分的颜色。

（7）BORDERCOLORDARK="COLOR"：定义3 D表格边框暗色部分的颜色。

（8）BACKGROUND="URL"：定义表格背景图像的位置。

（9）CELLSPACING="n"：设置单元格之间的空间。

（10）CELLPADDING="n"：设置单元格内容与边框之间的空间。

（11）COLS="n"：设置表格的列数。

（12）FRAME="FRAME"：定义表格外边框的显示类型。

（13）HEIGHT="n"：表格的高度（n可以为像素或百分比）。

（14）RULES="RULE"：定义表格内边框的显示类型。
```





- table标签里通常会包含几个 `tr` 标签，`tr` 代表表格里的一行。`tr` 标签又会包含 `td` 标签，每个 `td` 代表一个单元格。

```html
<table>
    <tr>
        <td>2000</td><td>悉尼</td>
    </tr>
    <tr>
        <td>2004</td><td>雅典</td>
    </tr>
    <tr>
        <td>2008</td><td>北京</td>
    </tr>
</table>
```





```html
<tr>标签还可以被<table>里的<thead>或<tbody>或<tfoot>包含。它们分别代表表头，表正文，表脚。在打印网页的时候，如果表格很大，一页打印不完，<thead>和<tfoot>将在每一页出现。

<th>和<td>非常相似，也用在<tr>里边，不同的是<th>代表这个单元格是它所在的行或列的标题。

<table>
    <thead>
        <tr><th>时间</th><th>地点</th></tr>
    </thead>
    <tbody>
        <tr><td>2000</td><td>悉尼</td></tr>
        <tr><td>2004</td><td>雅典</td></tr>
        <tr><td>2000</td><td>北京</td></tr>
    </tbody>
</table>
```



#### 行、表头和数据

在<TABLE>…</TABLE>标记符中，表格被指定为一行接一行的形式，而每一行的定义中包含了这一行所有单元格的定义。要定义一个表格，需从第一行逐级向下，并且按行中单元格的顺序开始定义。行、表头和数据定义的代码为：

<TABLE BORDER="1">

<TR><TH>表头</TH><TD>数据</TD>

</TR></TABLE>



```html
（1）<TR>…</TR>标记符：在表格中新开始一行。该标记符包含下列属性：

  ALIGN="ALIGNMENT"：行入口的水平对齐方式（LEFT，CENTER，RIGHT）。

  VALIGN="ALIGNMENT"：表格行入口的垂直对齐方式（TOP，MIDDLE，BOTTOM，BASELINE）。

  BGCOLOR="COLOR"：定义表格的背景色（可以为名字或十六进制数）。

（2）<TH>…</TH>和<TD>…</TD>。定义表格表头的<TH>和表格数据的</TD>标记符，包含下列属性：

  ALIGN="ALIGNMENT"：行入口的对齐方式（LEFT，CENTER，RIGHT）。

  VALIGN="ALIGNMENT"：表格行入口的垂直对齐方式（TOP，MIDDLE，BOTTOM，BASELINE）。

  BGCOLOR="COLOR"：定义表格的背景色（可以为名字或十六进制数）。

  BORDERCOLOR="COLOR"：定义表格边框的颜色。

  BORDERCOLORLIGHT="COLOR"：定义3 D表格边框亮色部分的颜色。

  BORDERCOLORDARK="COLOR"：定义3 D表格边框暗色部分的颜色。

  BACKGROUND="URL"：定义表格背景图像的位置。

  ROWSPAN="n"：表格的一个单元格可以覆盖的行数。

  COLSPAN="n"：表格的一个单元格可以覆盖的列数。

  NOWRAP：不许单元格内字符回绕。

  WIDTH="n"：以像素计的单元格宽度。

  HEIGHT="n"：以像素计的单元格高度。
```

 <TH>指明了一个单元格同时也是一个表头，<TD>标记符是表格中普通的单元格。表头通常以不同于表格单元格的方式显示，<TH>和<TD>都应该用相关的结束标记符</TH>和</TD>来结束。如果表头放在表格的顶部，表头的<TH>标记符应放在第一行内。 



### 表单

表单通常由文本输入框，按钮，多选框，单选框，下拉列表等组成，使HTML页面更有交互性。

表单的用处很多，也是HTML中比较重要的部分，这类应用在网上随处可见。一般在 <FORM> 标记框架下加上一种或几种的表单输入方式及一个或以上的按键。表单的标签在HTML文档中指定了一个表单。在一个文档中可以有多个表单，但是一点必须注意表单不能嵌套。 

```html
<form action="url" method=GET、POST>
…
</form>
```



#### 输入

-  三个属性：TYPE、VALUE、NAME。 

```html
（1）TYPE（类型）必须为以下的一种：

    "text"（文本），这个是缺省的。

    "password"（密码），看不到键入的字符，只有星号。

    "checkbox" （复选框），是一个单一的切换按钮，有开和关两种状态。

    "radio" （单选按钮），单一的切换按钮，有开和关两种状态，可以组成一个组，用于多选一的操作。

    "file" （文件浏览），可以选择你想上载的文件。

    "submit" （提交），它是一个按钮，将当前的表单包装到查询URL中并且将它发送到远程的服务器中。

    "reset" （复位），也是一个按钮，它可以使表单中的各种输入复位到它的缺省数值。

（2）NAME 是为输入区域的一个符号名字（并不是显示的名字）。

（3）VALUE是文本或者密码区域，它可以用于指定缺省区域内容。对于"submit" （提交）and "reset"（重置），VALUE可以用于指定标志。

（4）CHECKED （不需要数值）指定复选框或者单选按钮被选中，它只适用于复选框或者单选按钮。SIZE指定输入区域字符串的大小，它只对文本区域和密码输入区域有效。如果这个没给出，缺省的设置为20。	
```



```html
<html>

<head>

<title>表单标记</title>

</head>

<body>

<form action="" method="post"  name="form1" id="form1" >

用户名：<input type="text" name="textfield" /><br>

密码：<input type="password" name="textfield" /><br>

你感兴趣的球类有： <input type="checkbox" name="checkbox2" value="checkbox"  checked="checked"/>足球

<input type="checkbox" name="checkbox3" value="checkbox" />篮球

<input type="checkbox" name="checkbox" value="checkbox" /> 乒乓球<br>

 你是： <input type="radio" name="radiobutton" value="radiobutton"  checked="checked"/> 学生

<input type="radio" name="radiobutton" value="radiobutton" /> 教师

<input type="radio" name="radiobutton" value="radiobutton" />  管理<br>

请选择文件路径： <input type="file" name="file" /> <br>

<input type="submit" name="Submit" value="提交" />

<input type="reset" name="reset" value="重填" /><br>

</form>

</body>

</html>
```



#### 选择框

在<FORM> ... </FORM>里面有多少个SELECT标签都是允许的，它可以混合其它HTML元素（包括INPUT和TEXTAREA元素）和文本，但是不能包括FORMS。

与INPUT不同，SELECT有终止标签。在SELECT里面，有一系列的OPTION标签，每一个OPTION标签之后跟着一些文本

```html
<SELECT NAME="select ">
	<OPTION>First option
	<OPTION> Second option 
</SELECT>
        
NAME是为这个SELECT元素起的名字。它不能为空，必须给出具体值。
MULTIPLE：如果出现（非数值），它指定选择框可以进行多行选择。
```



OPTION的属性如下：

SELECTED 指定缺省状态这个OPTION被选择。如果SELECT允许多行选择，可以指定多个OPTION为SELECTED。 



#### 文本域

TEXTAREA标签被用来放置一个多行的文本输入区域。它有以下的属性：

```html
①NAME是文本域的名字。

②ROWS 是文本域的行数。

③COLS 是文本域的列数（即字符的水平宽度）。
```

 TEXTAREA 域自动有滚动条。不论多少的文本都可以件入到里面。TEXTAREA元素需要一个开始和终止的标签即<TEXTAREA>和</TEXTAREA> 。 



```html
<html>

<head>

<title>无标题文档</title>

</head>

<body>

请选择:

<select name="select">

  <option value="1">足球</option>

  <option value="2">篮球</option>

  <option value="3">乒乓球</option>

</select>

<br><br><br>

请输入:

<textarea name="textarea"></textarea>

</body>

</html>
```





### 框架

框架使页面里能包含其它的页面。

最后谈一下框架，曾经非常流行的技术，框架使一个窗口里能同时显示多个文档。主框架页里面没有<body>标签，取代它的是<frameset>。	<frameset>标签的属性Rows和Cols用于指定框架集(frameset)里有多少行（列），以及每行（列）的高度（宽度）。<frameset>标签可以包含<frame>标签，每个<frame>标签代表一个文档（src属性指定文档的地址）。

如果觉得这样的页面还不够复杂的话，还可以在<frameset>标签里包含<frameset>标签。



### 其余常见标签

```html
<meta charset="utf-8">

<link rel="stylesheet" type="text/css" href="test1.css">
```



# CSS

- `Cascading Stylesheets`，层叠样式表 
-  利用CSS可以定义HTML中元素的显示效果，包括元素的位置、颜色、背景、边空、字体、排版格式等。
- CSS的基本思想是为文档中的各个标记定义出相应的一组显示样式。 
-  定义的格式为：选择符 { 样式属性：取值；样式属性：取值；... } 



```html
<HTML>

<HEAD>

    <STYLE>

        H1{color:red; }

        myclass{color:green}

        H2.myclass{color:blue}

        #myid{color:brown}

    </STYLE>
    
</HEAD>

<BODY>

    <H1>这是红色的一号标题。</H1>

    <P class="myclass">"myclass"类中的正文是绿色的。</P>

    <H2 class="myclass">但"myclass"类中的二号标题是蓝色的。</H2>

    <P class="myclass" id="myid">以"myid"为标识的正文则是棕色的。</P>

</BODY>

</HTML>
```



## CSS的定义方法



### 选择符

选择符是指被施加样式的元素，浏览器在文件中碰到这些元素时，就使用定义好的样式来显示它们。基本的选择符包括标记、类、标识、伪类等。 



#### 标记（tag）选择符

标记可以是HTML中的标记，也可以是XML中已定义的标记。具体的定义方式是：

标记名 { 样式属性：取值；样式属性：取值；... }

在本例中为HTML中的标记<H1>定义了样式，将该标记下的文本用红色显示，因此，浏览结果中的第一行是红色的。



#### 类（class）选择符

无论是HTML还是XML文档，有些内容是可以分类处理的，相应地，对于某一类的内容可以定义不同的样式予以显示。例子中定义了一个类“myclass”，并为它定义了绿色显示的样式，所以属于该类的元素，即第二行文本，显示出来是绿色的。

注意，定义样式时class可以与标记相关联。在样式单的第三行为属于myclass类的标记<H2>定义了蓝色显示的样式，相应地，第三行文本呈蓝色。

与标记相关的类选择符与不相关的类选择符的定义方法分别是：

标记名.类名 { 样式属性：取值；样式属性：取值；... }

另外，在对XML文档中的类定义样式时，首先该class应该在DTD中预先声明，否则会导致错误的发生。



#### 标识（id）选择符

在HTML/XML文档中，常常要惟一地标识一个元素，即赋予它一个id标识，以便在对整个文档进行处理时能够很快地找到这个元素。CSS也可以将标识id作为选择符进行样式设定，定义的方法与类大同小异，只要把符号“.”改成“#”就行了。方法是：

标记名#标识名 { 样式属性：取值；样式属性：取值；... }

\#标识名 { 样式属性：取值；样式属性：取值；... }

一般情况下，为标识定义的样式是优先于为类定义的样式的，因此例子中第四行中的文本虽然属于类“myclass”，但显示效果为棕色。



#### 伪类（pseudo-classe）选择符

伪类选择符主要是指链接锚点的一些特性，在CSS中可以为链接锚点的不同状态赋予不同的属性。 

伪类选择符

| 示例                      | 效果                         |
| ------------------------- | ---------------------------- |
| A:link{ color: blue }     | 没访问过的链接颜色显示为蓝色 |
| A:visited{ color: red }   | 访问过的链接颜色显示为红色   |
| A:active{ color: yellow } | 激活的链接颜色显示为黄色     |
| A:hover { color: green }  | 鼠标滑过链接时颜色显示为绿色 |

可以将某个样式同时施加在多个选择符指定的不同元素上，只要将在大括号括起来的样式定义之前的各选择符之间用逗号分隔即可。如果选择符之间用空格分隔，则是用前面的父元素来约束后面的子元素。

选择符，选择符，... { 样式属性：取值；样式属性：取值；... } 



### 样式属性

样式属性就是指元素的哪些属性可以在样式单中给予改变，包括字体属性、颜色属性、背景属性、文本属性、边框属性等，还有一些与页面排版、跨媒体出版相关的内容。

在定义样式时，除需指出样式所施加的元素、元素的属性之外，还要给属性赋予一个新值。根据属性的不同，属性值的选取也有所不同，主要有以下4种：长度、URL、颜色、关键字。



## 使用CSS显示HTML文档

- 为网页添加样式表的方法有四种 



1）最简单的方法是直接添加在HTML的标识符（tag）里

```html
< p style=”color: blue; font-size: 10pt”>CSS实例< /p>
```

 用蓝色显示字体大小为10pt的“CSS实例”。尽管使用简单、显示直观，但是这种方法不怎么常用，因为这样添加无法完全发挥样式表的优势“内容结构和格式控制分别保存”。 



2）添加在HTML的头信息标识符< head>里

```html
< head>

< style type=”text/css”>

< !--样式表的具体内容 -->

< /style>

< /head>
```

`type=”text/css”`表示样式表采用MIME类型，帮助不支持CSS的浏览器过滤掉CSS代码，避免在浏览器面前直接以源代码的方式显示我们设置的样式表。但为了保证上述情况一定不要发生，还是有必要在样式表里加上注释标识符“< !--注释内容-->”。 



3）链接样式表 

 同样是添加在HTML的头信息标识符< head>里： 

```html
< head>
	< link rel=”stylesheet” href=”*.css” type=”text/css” media=”screen”>
< /head>
```

*.css是单独保存的样式表文件，其中不能包含<style>标识符，并且只能以css为后缀。



Media是可选的属性，表示使用样式表的网页将用什么媒体输出。取值范围：

```html
l  Screen（默认）：输出到电脑屏幕

l  Print：输出到打印机

l  TV：输出到电视机

l  Projection：输出到投影仪

l  Aural：输出到扬声器

l  Braille：输出到凸字触觉感知设备

l  Tty：输出到电传打字机

l  All：输出到以上所有设备

如果要输出到多种媒体，可以用逗号分隔取值表。

Rel属性表示样式表将以何种方式与HTML文档结合。取值范围：

l  Stylesheet：指定一个外部的样式表

l  Alternate stylesheet：指定使用一个交互样式表
```



4）联合使用样式表

 同样是添加在HTML的头信息标识符< head>里： 

```html
< head>

< style type=”text/css”>

< !--

@import “*.css”

其他样式表的声明

-->

< /style>

< /head>
```

以@import开头的联合样式表输入方法和链接样式表的方法很相似，但联合样式表输入方式更有优势。因为联合法可以在链接外部样式表的同时，针对该网页的具体情况，做出别的网页不需要的样式规则。

需要注意的是：联合法输入样式表必须以@import开头。如果同时输入多个样式表有冲突的时候，将按照第一个输入的样式表对网页排版；如果输入的样式表和网页里的样式规则冲突时，使用外部的样式表。



# JS

示例

```js
<script>
function changeImage()
{
    element=document.getElementById('myimage')
    if (element.src.match("bulbon"))
    {
        element.src="/images/pic_bulboff.gif";
    }
    else
    {
        element.src="/images/pic_bulbon.gif";
    }
}
</script>
<img id="myimage" onclick="changeImage()" src="/images/pic_bulboff.gif" width="100" height="180">
```



## 用法

- 在html的head标签
- 在html的body标签
- 引入外部的javaScript

```html
<!DOCTYPE html>
<html>
	<body>
		<script src="myScript.js"></script>
	</body>
</html>
```

## JS输出

JavaScript 可以通过不同的方式来输出数据：

- 使用 **window.alert()** 弹出警告框。
- 使用 **document.write()** 方法将内容写到 HTML 文档中。
- 使用 **innerHTML** 写入到 HTML 元素。
- 使用 **console.log()** 写入到浏览器的控制台。



## 函数



### 函数参数

- 函数显式参数(Parameters)与隐式参数(Arguments)



```js
// ES6 支持函数带有默认参数
function myFunction(x, y = 10) {
    // y is 10 if not passed or undefined
    return x + y;
}
 
myFunction(0, 2) // 输出 2
myFunction(5); // 输出 15, y 参数的默认值
```



- arguments 对象

  > JavaScript 函数有个内置的对象 arguments 对象。
  >
  > argument 对象包含了函数调用的参数数组。

```js
x = findMax(1, 123, 500, 115, 44, 88);
 
function findMax() {
    var i, max = arguments[0];
    
    if(arguments.length < 2) return max;
 
    for (i = 0; i < arguments.length; i++) {
        if (arguments[i] > max) {
            max = arguments[i];
        }
    }
    return max;
}
```





### 函数声明

```js
function functionname()
{
    // 执行代码
}

// 带参
function myFunction(var1,var2)
{
	// 代码
}

// 返回值
function myFunction()
{
    var x=5;
    return x;
}
```



### 函数表达式

```js
var x = function (a, b) {return a * b};
var z = x(4, 3);
```



### Function() 构造函数

 函数同样可以通过内置的 JavaScript 函数构造器（Function()）定义。 

```js
var myFunction = new Function("a", "b", "return a * b");

var x = myFunction(4, 3);
```

=> 

```js
var myFunction = function (a, b) {return a * b};

var x = myFunction(4, 3);
```



### 自调用函数

函数表达式可以 "自调用"。

自调用表达式会自动调用。

如果表达式后面紧跟 () ，则会自动调用。

不能自调用声明的函数。

通过添加括号，来说明它是一个函数表达式：

```js
(function () {
    var x = "Hello!!";      // 我将调用自己
})();

```



### 箭头函数

```js
(参数1, 参数2, …, 参数N) => { 函数声明 }

(参数1, 参数2, …, 参数N) => 表达式(单一)
// 相当于：(参数1, 参数2, …, 参数N) =>{ return 表达式; }
```



```js
// ES5
var x = function(x, y) {
     return x * y;
}
 
// ES6
const x = (x, y) => x * y;
```



### 函数调用

-  一般而言，在Javascript中，this指向函数执行时的当前对象。 

#### 作为一个函数调用

```js
function myFunction(a, b) {
    return a * b;
}
myFunction(10, 2);           // myFunction(10, 2) 返回 20
```



以上函数不属于任何对象。但是在 JavaScript 中它始终是默认的全局对象。

在 HTML 中默认的全局对象是 HTML 页面本身，所以函数是属于 HTML 页面。

在浏览器中的页面对象是浏览器窗口(window 对象)。以上函数会自动变为 window 对象的函数。

myFunction() 和 window.myFunction() 是一样的：

```js
function myFunction(a, b) {
    return a * b;
}
window.myFunction(10, 2);    // window.myFunction(10, 2) 返回 20
```



#### 全局对象

当函数没有被自身的对象调用时 **this** 的值就会变成全局对象。

在 web 浏览器中全局对象是浏览器窗口（window 对象）。

该实例返回 **this** 的值是 window 对象:

```js
function myFunction() {
    return this;
}
myFunction();                // 返回 window 对象
```



### 函数作为方法调用

在 JavaScript 中你可以将函数定义为对象的方法。

以下实例创建了一个对象 (**myObject**), 对象有两个属性 (**firstName** 和 **lastName**), 及一个方法 (**fullName**):

```js
var myObject = {
    firstName:"John",
    lastName: "Doe",
    fullName: function () {
        return this.firstName + " " + this.lastName;
    }
}
myObject.fullName();         // 返回 "John Doe"
```



### 使用构造函数调用函数

```js
// 构造函数:
function myFunction(arg1, arg2) {
    this.firstName = arg1;
    this.lastName  = arg2;
}
 
// This    creates a new object
var x = new myFunction("John","Doe");
x.firstName;                             // 返回 "John"
```



#### 作为函数方法调用函数

在 JavaScript 中, 函数是对象。JavaScript 函数有它的属性和方法。

**call()** 和 **apply()** 是预定义的函数方法。 两个方法可用于调用函数，两个方法的第一个参数必须是对象本身。

```js
function myFunction(a, b) {
    return a * b;
}
myObject = myFunction.call(myObject, 10, 2);     // 返回 20
```

```js
function myFunction(a, b) {
    return a * b;
}
myArray = [10, 2];
myObject = myFunction.apply(myObject, myArray);  // 返回 20
```





## 变量

```js
// 在 JavaScript 函数内部声明的变量（使用 var）是局部变量，所以只能在函数内部访问它。

// 在函数外声明的变量是全局变量，网页上的所有脚本和函数都能访问它。

// 向未声明的 JavaScript 变量分配值。如果您把值赋给尚未声明的变量，该变量将被自动作为 window 的一个属性。
```



```js
var var1 = 1; // 不可配置全局属性
var2 = 2; // 没有使用 var 声明，可配置全局属性

console.log(this.var1); // 1
console.log(window.var1); // 1

delete var1; // false 无法删除
console.log(var1); //1

delete var2; 
console.log(delete var2); // true
console.log(var2); // 已经删除 报错变量未定义
```



## 事件

HTML 事件可以是浏览器行为，也可以是用户行为。

以下是 HTML 事件的实例：

- HTML 页面完成加载
- HTML input 字段改变时
- HTML 按钮被点击

HTML 元素中可以添加事件属性，使用 JavaScript 代码来添加 HTML 元素。

```html
<some-HTML-element some-event='JavaScript 代码'>
    
<some-HTML-element some-event="JavaScript 代码">
```



常见的HTML事件

| 事件            | 描述                         |
| :-------------- | :--------------------------- |
| onchange        | HTML 元素改变                |
| onclick         | 用户点击 HTML 元素           |
| onmouseover     | 用户在一个HTML元素上移动鼠标 |
| onmouseout      | 用户从一个HTML元素上移开鼠标 |
| onkeydown       | 用户按下键盘按键             |
| onload/onunload | 浏览器已完成页面的加载       |



## JS表单



### JavaScript 表单验证

 HTML 表单验证可以通过 JavaScript 来完成。 

```html
<form name="myForm" action="demo_form.php" onsubmit="return validateForm()" method="post">
    名字: 
    <input type="text" name="fname">
    <input type="submit" value="提交">
</form>


function validateForm() {
    var x = document.forms["myForm"]["fname"].value;
    if (x == null || x == "") {
        alert("需要输入名字。");
        return false;
    }
}
```



```html
// E-mail 验证

<form name="myForm" action="demo-form.php" onsubmit="return validateForm();" method="post">
    Email: <input type="text" name="email">
    <input type="submit" value="提交">
</form>

function validateForm(){
  var x=document.forms["myForm"]["email"].value;
  var atpos=x.indexOf("@");
  var dotpos=x.lastIndexOf(".");
  if (atpos<1 || dotpos<atpos+2 || dotpos+2>=x.length){
    alert("不是一个有效的 e-mail 地址");
    return false;
  }
}
```





### HTML 表单自动验证

HTML 表单验证也可以通过浏览器来自动完成。

如果表单字段 (fname) 的值为空, **required** 属性会阻止表单提交：

```html
<form action="demo_form.php" method="post">
  <input type="text" name="fname" required="required">
  <input type="submit" value="提交">
</form>
```



### HTML 约束验证

| 属性     | 描述                     |
| :------- | :----------------------- |
| disabled | 规定输入的元素不可用     |
| max      | 规定输入元素的最大值     |
| min      | 规定输入元素的最小值     |
| pattern  | 规定输入元素值的模式     |
| required | 规定输入元素字段是必需的 |
| type     | 规定输入元素的类型       |



### this 关键字

在 JavaScript 中 this 不是固定不变的，它会随着执行环境的改变而改变。

- 在方法中，this 表示该方法所属的对象。
- 如果单独使用，this 表示全局对象。
- 在函数中，this 表示全局对象。
- 在函数中，在严格模式下，this 是未定义的(undefined)。
- 在事件中，this 表示接收事件的元素。
- 类似 call() 和 apply() 方法可以将 this 引用到任何对象。



## HTML DOM (文档对象模型)

### 查找 HTML 元素

通常，通过 JavaScript，您需要操作 HTML 元素。

为了做到这件事情，您必须首先找到该元素。有三种方法来做这件事：

- 通过 id 找到 HTML 元素

  ```js
  var x=document.getElementById("intro");
  ```

- 通过标签名找到 HTML 元素

  ```js
  var x=document.getElementById("main");
  var y=x.getElementsByTagName("p");
  ```

- 通过类名找到 HTML 元素

  ```js
  var x=document.getElementsByClassName("intro");
  ```



### HTML DOM

-  HTML DOM 允许 JavaScript 改变 HTML 元素的内容。

#### 改变 HTML 输出流

 document.write() 可用于直接向 HTML 输出流写内容。 

```html
<!DOCTYPE html>
<html>
<body>

<script>
document.write(Date());
</script>

</body>
</html>

```

#### 改变 HTML 内容

修改 HTML 内容的最简单的方法是使用 innerHTML 属性。

如需改变 HTML 元素的内容，请使用这个语法：

document.getElementById(*id*).innerHTML=*新的 HTML*

```html
<html>
<body>

<p id="p1">Hello World!</p>

<script>
document.getElementById("p1").innerHTML="新文本!";
</script>

</body>
</html>
```



#### 改变 HTML 属性

如需改变 HTML 元素的属性，请使用这个语法：

document.getElementById(*id*).*attribute=新属性值*

本例改变了 <img> 元素的 src 属性：

```html
<!DOCTYPE html>
<html>
<body>

<img id="image" src="smiley.gif">

<script>
document.getElementById("image").src="landscape.jpg";
</script>

</body>
</html>
```



### HTML DOM 事件



### HTML DOM EventListener

- addEventListener() 方法

在用户点击按钮时触发监听事件：

```js
document.getElementById("myBtn").addEventListener("click", displayDate);
```

```txt
addEventListener() 方法用于向指定元素添加事件句柄。

addEventListener() 方法添加的事件句柄不会覆盖已存在的事件句柄。

你可以向一个元素添加多个事件句柄。

你可以向同个元素添加多个同类型的事件句柄，如：两个 "click" 事件。

你可以向任何 DOM 对象添加事件监听，不仅仅是 HTML 元素。如： window 对象。

addEventListener() 方法可以更简单的控制事件（冒泡与捕获）。

当你使用 addEventListener() 方法时, JavaScript 从 HTML 标记中分离开来，可读性更强， 在没有控制HTML标记时也可以添加事件监听。

你可以使用 removeEventListener() 方法来移除事件的监听。
```



- 语法

```js
element.addEventListener(event, function, useCapture);
```

第一个参数是事件的类型 (如 "click" 或 "mousedown").

第二个参数是事件触发后调用的函数。

第三个参数是个布尔值用于描述事件是冒泡还是捕获。该参数是可选的。



>  注意:不要使用 "on" 前缀。 例如，使用 "click" ,而不是使用 "onclick"。



#### 向原元素添加事件句柄

```js
当用户点击元素时弹出 "Hello World!" :

element.addEventListener("click", function(){ alert("Hello World!"); });
```



```js
element.addEventListener("click", myFunction);

function myFunction() {
    alert ("Hello World!");
}
```



#### 向同一个元素中添加多个事件句柄

-  addEventListener() 方法允许向同一个元素添加多个事件，且不会覆盖已存在的事件： 

```js
element.addEventListener("click", myFunction);
element.addEventListener("click", mySecondFunction);
```

 你可以向同个元素添加不同类型的事件： 

```js
element.addEventListener("mouseover", myFunction);
element.addEventListener("click", mySecondFunction);
element.addEventListener("mouseout", myThirdFunction);
```



#### 向 Window 对象添加事件句柄

addEventListener() 方法允许你在 HTML DOM 对象添加事件监听， HTML DOM 对象如：

​	 HTML 元素, HTML 文档, window 对象。或者其他支出的事件对象如: xmlHttpRequest 对象。 



当用户重置窗口大小时添加事件监听：

```js
window.addEventListener("resize", function(){
  document.getElementById("demo").innerHTML = sometext;
});
```



#### 传递参数

 当传递参数值时，使用"匿名函数"调用带参数的函数： 

```js
element.addEventListener("click", function(){ myFunction(p1, p2); });
```



#### 事件冒泡或事件捕获

事件传递有两种方式：冒泡与捕获。

事件传递定义了元素事件触发的顺序。 如果你将 <p> 元素插入到 <div> 元素中，用户点击 <p> 元素, 哪个元素的 "click" 事件先被触发呢？

在 *冒泡* 中，内部元素的事件会先被触发，然后再触发外部元素，即： <p> 元素的点击事件先触发，然后会触发 <div> 元素的点击事件。

在 *捕获* 中，外部元素的事件会先被触发，然后才会触发内部元素的事件，即： <div> 元素的点击事件先触发 ，然后再触发 <p> 元素的点击事件。

addEventListener() 方法可以指定 "useCapture" 参数来设置传递类型：

```js
addEventListener(event, function, useCapture);
```

默认值为 false, 即冒泡传递，当值为 true 时, 事件使用捕获传递。



#### removeEventListener() 方法

 removeEventListener() 方法移除由 addEventListener() 方法添加的事件句柄: 

```js
element.removeEventListener("mousemove", myFunction);
```



#### 浏览器支持

IE 8 及更早 IE 版本，Opera 7.0及其更早版本不支持 addEventListener() 和 removeEventListener() 方法。但是，对于这类浏览器版本可以使用 detachEvent() 方法来移除事件句柄: 

```js
element.attachEvent(event, function);
element.detachEvent(event, function);
```



 跨浏览器解决方法: 

```js
var x = document.getElementById("myBtn");
if (x.addEventListener) {                    // 所有主流浏览器，除了 IE 8 及更早版本
    x.addEventListener("click", myFunction);
} else if (x.attachEvent) {                  // IE 8 及更早版本
    x.attachEvent("onclick", myFunction);
}
```



#### HTML DOM 事件对象参考手册

所有 HTML DOM 事件，可以查看我们完整的[ HTML DOM Event 对象参考手册](https://www.runoob.com/jsref/dom-obj-event.html)。



#### 使用

 使用 addEventListener 的时候，又无法使用，是因为: 

```js
x = document.getElementById("myBt");
// x ---> null
```

=>  可以这么写: 

```js
window.onload = function () {
    var x = document.getElementById("myBt");
    x.addEventListener("click", myFunction);
};
```



 所以为了兼容所有的浏览器，我们可以定义一个函数：当有 addEventListener 时调用，没有的时候调用 attachEvent。 

```js
/*
 * 参数：
 *     obj：要绑定事件的对象
 *     eventStr：事件(注意：这里不要on)
 *      callback：回调函数
 */
function bind(obj , eventStr , callback){
    if(obj.addEventListener){
        //大部分浏览器
        obj.addEventListener(eventStr , callback , false);
    }else{
        //IE8及以下
        obj.attachEvent("on"+eventStr , function(){
            //在匿名函数中调用回调函数
            callback.call(obj);
        });
    }
}
```



### HTML DOM 元素 (节点)



#### appendChild()

- 要创建新的 HTML 元素 (节点)需要先创建一个元素，然后在已存在的元素中添加它。 
- 它用于添加新元素到**尾部**。

```html
<div id="div1">
	<p id="p1">这是一个段落。</p>
	<p id="p2">这是另外一个段落。</p>
</div>

<script>
    //创建 <p> 元素
	var para = document.createElement("p");
    //为 <p> 元素创建一个新的文本节点：
	var node = document.createTextNode("这是一个新的段落。");
    //将文本节点添加到 <p> 元素中：
	para.appendChild(node);
 
    //查找已存在的元素：
	var element = document.getElementById("div1");
    //添加到已存在的元素中:
	element.appendChild(para);
</script>
```



#### insertBefore()

以上的实例我们使用了 appendChild() 方法，它用于添加新元素到尾部。

如果我们需要将新元素添加到**开始位置**，可以使用 insertBefore() 方法:

```html
<div id="div1">
	<p id="p1">这是一个段落。</p>
	<p id="p2">这是另外一个段落。</p>
</div>
 
<script>
    var para = document.createElement("p");
    var node = document.createTextNode("这是一个新的段落。");
    para.appendChild(node);

    var element = document.getElementById("div1");
    var child = document.getElementById("p1");
    element.insertBefore(para, child);
</script>
```



#### 移除已存在的元素

要移除一个元素，你需要知道该元素的父元素。

```html
<div id="div1">
	<p id="p1">这是一个段落。</p>
	<p id="p2">这是另外一个段落。</p>
</div>
 
<script>
    var parent = document.getElementById("div1");
    var child = document.getElementById("p1");
    parent.removeChild(child);
</script>
```



#### replaceChild()

 我们可以使用 replaceChild() 方法来替换 HTML DOM 中的元素。 

```html
<div id="div1">
    <p id="p1">这是一个段落。</p>
    <p id="p2">这是另外一个段落。</p>
</div>
 
<script>
    var para = document.createElement("p");
    var node = document.createTextNode("这是一个新的段落。");
    para.appendChild(node);

    var parent = document.getElementById("div1");
    var child = document.getElementById("p1");
    parent.replaceChild(para, child);
</script>
```



### HTML DOM 集合(Collection)

getElementsByTagName() 方法返回 HTMLCollection 对象。

HTMLCollection 对象类似包含 HTML 元素的一个数组。

以下代码获取文档所有的 <p> 元素：

```js
var x = document.getElementsByTagName("p");

//集合中的元素可以通过索引(以 0 为起始位置)来访问。
// 访问第二个 <p> 元素可以是以下代码:
y = x[1];
```



#### HTMLCollection 对象 length 属性

 HTMLCollection 对象的 length 属性定义了集合中元素的数量。 

```js
var myCollection = document.getElementsByTagName("p");

document.getElementById("demo").innerHTML = myCollection.length;
```



#### 注意

**HTMLCollection 不是一个数组！**

HTMLCollection 看起来可能是一个数组，但其实不是。

你可以像数组一样，使用索引来获取元素。

HTMLCollection 无法使用数组的方法： valueOf(), pop(), push(), 或 join() 。



### HTML DOM 节点列表

**NodeList** 对象是一个从文档中获取的节点列表 (集合) 。

NodeList 对象类似 HTMLCollection对象。

一些旧版本浏览器中的方法（如：**getElementsByClassName()**）返回的是 NodeList 对象，而不是 HTMLCollection 对象。

所有浏览器的 **childNodes** 属性返回的是 NodeList 对象。

大部分浏览器的 **querySelectorAll()** 返回 NodeList 对象。



```js
// 选取文档中所有的 <p> 节点：
var myNodeList = document.querySelectorAll("p");

// NodeList 中的元素可以通过索引(以 0 为起始位置)来访问。
// 访问第二个 <p> 元素可以是以下代码:

y = myNodeList[1];
```



#### NodeList 对象 length 属性

NodeList 对象 length 属性定义了节点列表中元素的数量。

```js
var myNodelist = document.querySelectorAll("p");
document.getElementById("demo").innerHTML = myNodelist.length;
```



#### HTMLCollection 与 NodeList 的区别

- HTMLCollection是 HTML 元素的集合；NodeList 是一个文档节点的集合。

- NodeList 与 HTMLCollection 都与数组对象有点类似，可以使用索引 (0, 1, 2, 3, 4, ...) 来获取元素；NodeList 与 HTMLCollection 都有 length 属性。

- HTMLCollection 元素可以通过 name，id 或索引来获取；NodeList 只能通过索引来获取。

- 只有 NodeList 对象有包含属性节点和文本节点。



## JS对象

JavaScript 提供多个内建对象，比如 String、Date、Array 等等。 对象只是带有属性和方法的特殊数据类型。

  

### 创建对象的方法

创建新对象有两种不同的方法：

- 定义并创建对象的实例
- 使用函数来定义对象，然后创建新的对象实例



#### 创建直接的实例

 这个例子创建了对象的一个新实例，并向其添加了四个属性： 

```js
person=new Object();
person.firstname="John";
person.lastname="Doe";
person.age=50;
person.eyecolor="blue";
```



```js
person={firstname:"John",lastname:"Doe",age:50,eyecolor:"blue"};
```



#### 使用对象构造器

```js
function person(firstname,lastname,age,eyecolor){
    this.firstname=firstname;
    this.lastname=lastname;
    this.age=age;
    this.eyecolor=eyecolor;
}
```



#### 创建对象实例

```js
var myFather=new person("John","Doe",50,"blue");
var myMother=new person("Sally","Rally",48,"green");
```



#### 把方法添加到 JavaScript 对象

方法只不过是附加在对象上的函数。

在构造器函数内部定义对象的方法：

```js
function person(firstname,lastname,age,eyecolor){
    this.firstname=firstname;
    this.lastname=lastname;
    this.age=age;
    this.eyecolor=eyecolor;

    this.changeName=changeName;
    function changeName(name)
    {
        this.lastname=name;
    }
}

myMother.changeName("Doe");
```



### prototype 继承

所有的 JavaScript 对象都会从一个 prototype（原型对象）中继承属性和方法：

- `Date` 对象从 `Date.prototype` 继承。
- `Array` 对象从 `Array.prototype` 继承。
- `Person` 对象从 `Person.prototype` 继承。

所有 JavaScript 中的对象都是位于原型链顶端的 Object 的实例。

JavaScript 对象有一个指向一个原型对象的链。当试图访问一个对象的属性时，它不仅仅在该对象上搜寻，还会搜寻该对象的原型，以及该对象的原型的原型，依次层层向上搜索，直到找到一个名字匹配的属性或到达原型链的末尾。

`Date` 对象, `Array` 对象, 以及 `Person` 对象从 `Object.prototype` 继承。



#### 添加属性和方法

有的时候我们想要在所有已经存在的对象添加新的属性或方法。

另外，有时候我们想要在对象的构造函数中添加属性或方法。

使用 prototype 属性就可以给对象的构造函数添加新的属性：

```js
function Person(first, last, age, eyecolor) {
  this.firstName = first;
  this.lastName = last;
  this.age = age;
  this.eyeColor = eyecolor;
}
 
Person.prototype.nationality = "English";
```

 然我们也可以使用 prototype 属性就可以给对象的构造函数添加新的方法： 

```js
function Person(first, last, age, eyecolor) {
  this.firstName = first;
  this.lastName = last;
  this.age = age;
  this.eyeColor = eyecolor;
}
 
Person.prototype.name = function() {
  return this.firstName + " " + this.lastName;
};
```



## HTML BOM 浏览器对象模型

 浏览器对象模型 (BOM) 使 JavaScript 有能力与浏览器"对话"。 



### Window 对象

所有浏览器都支持 window 对象。它表示浏览器窗口。

所有 JavaScript 全局对象、函数以及变量均自动成为 window 对象的成员。

全局变量是 window 对象的属性。

全局函数是 window 对象的方法。

甚至 HTML DOM 的 document 也是 window 对象的属性之一：

```js
window.document.getElementById("header");
// 与此相同：
document.getElementById("header");
```



#### Window 尺寸

有三种方法能够确定浏览器窗口的尺寸。

对于Internet Explorer、Chrome、Firefox、Opera 以及 Safari：

- window.innerHeight - 浏览器窗口的内部高度(包括滚动条)
- window.innerWidth - 浏览器窗口的内部宽度(包括滚动条)

对于 Internet Explorer 8、7、6、5：

- document.documentElement.clientHeight
- document.documentElement.clientWidth

或者

- document.body.clientHeight
- document.body.clientWidth



 实用的 JavaScript 方案（涵盖所有浏览器）： 

```js
var w=window.innerWidth
|| document.documentElement.clientWidth
|| document.body.clientWidth;

var h=window.innerHeight
|| document.documentElement.clientHeight
|| document.body.clientHeight;
```



#### 其他 Window 方法

- window.open() - 打开新窗口
- window.close() - 关闭当前窗口
- window.moveTo() - 移动当前窗口
- window.resizeTo() - 调整当前窗口的尺寸



### Window Screen

 window.screen 对象包含有关用户屏幕的信息。 



**window.screen**对象在编写时可以不使用 window 这个前缀。

一些属性：

- screen.availWidth - 可用的屏幕宽度
- screen.availHeight - 可用的屏幕高度



#### Window Screen 可用宽度

 screen.availWidth 属性返回访问者屏幕的宽度，以像素计，减去界面特性，比如窗口任务栏。 

```js
<script>
	document.write("可用宽度: " + screen.availWidth);
</script>
```



#### Window Screen 可用高度

 screen.availHeight 属性返回访问者屏幕的高度，以像素计，减去界面特性，比如窗口任务栏。 

```js
<script>
	document.write("可用高度: " + screen.availHeight);
</script>
```



### Window Location

 window.location 对象用于获得当前页面的地址 (URL)，并把浏览器重定向到新的页面。 



**window.location** 对象在编写时可不使用 window 这个前缀。 一些例子：

一些实例:

- location.hostname 返回 web 主机的域名

- location.pathname 返回当前页面的路径和文件名

- location.port 返回 web 主机的端口 （80 或 443）

- location.protocol 返回所使用的 web 协议（http: 或 https:）

-  location.href  返回（当前页面的）整个 URL 

-  location.assign() 加载新的文档。 

  ```html
  <!DOCTYPE html>
  <html>
      <head>
          <meta charset="utf-8">
          <title>菜鸟教程(runoob.com)</title>
      </head>
      <head>
          <script>
          function newDoc(){
              window.location.assign("https://www.runoob.com")
          }
          </script>
      </head>
      <body>
      	<input type="button" value="加载新文档" onclick="newDoc()">
      </body>
  </html>
  ```

  

###  Window History

window.history 对象包含浏览器的历史。 



**window.history**对象在编写时可不使用 window 这个前缀。

为了保护用户隐私，对 JavaScript 访问该对象的方法做出了限制。

一些方法：

- history.back() - 与在浏览器点击后退按钮相同
- history.forward() - 与在浏览器中点击向前按钮相同



### Window Navigator

 window.navigator 对象包含有关访问者浏览器的信息。 



### JavaScript 弹窗

 可以在 JavaScript 中创建三种消息框：警告框、确认框、提示框。 



#### 警告框

 **window.alert()** 方法可以不带上window对象，直接使用**alert()**方法。 

```js
window.alert("sometext");
```



#### 确认框

 **window.confirm()** 方法可以不带上window对象，直接使用**confirm()**方法。 

```js
window.confirm("sometext");
```



#### 提示框

 **window.prompt()** 方法可以不带上window对象，直接使用**prompt()**方法。 

```js
window.prompt("sometext","defaultvalue");
```



### 计时事件

JavaScript 一个设定的时间间隔之后来执行代码我们称之为计时事件



通过使用 JavaScript，我们有能力做到在一个设定的时间间隔之后来执行代码，而不是在函数被调用后立即执行。我们称之为计时事件。

在 JavaScritp 中使用计时事件是很容易的，两个关键方法是:

- setInterval() - 间隔指定的毫秒数不停地执行指定的代码。
- setTimeout() - 在指定的毫秒数后执行指定代码。

**注意:** setInterval() 和 setTimeout() 是 HTML DOM Window对象的两个方法。



#### setInterval() 方法

**window.setInterval()** 方法可以不使用 window 前缀，直接使用函数 **setInterval()**。

setInterval() 第一个参数是函数（function）。

第二个参数间隔的毫秒数

```js
window.setInterval("javascript function",milliseconds);
```



#### 如何停止执行

 clearInterval() 方法用于停止 setInterval() 方法执行的函数代码。 

```js
window.clearInterval(intervalVariable)
```

**window.clearInterval()** 方法可以不使用window前缀，直接使用函数**clearInterval()**。

要使用 clearInterval() 方法, 在创建计时方法时你必须使用全局变量：

```js
myVar=setInterval("javascript function",milliseconds);
```

 然后你可以使用 clearInterval() 方法来停止执行。 



#### setTimeout() 方法

setTimeout() 方法会返回某个值。在上面的语句中，值被储存在名为 myVar 的变量中。假如你希望取消这个 setTimeout()，你可以使用这个变量名来指定它。

setTimeout() 的第一个参数是含有 JavaScript 语句的字符串。这个语句可能诸如 "alert('5 seconds!')"，或者对函数的调用，诸如 alertMsg。

第二个参数指示从当前起多少毫秒后执行第一个参数。

```js
myVar= window.setTimeout("javascript function", milliseconds);
```



#### 如何停止执行

```js
window.clearTimeout(timeoutVariable)
```

**window.clearTimeout()** 方法可以不使用window 前缀。

要使用clearTimeout() 方法, 你必须在创建超时方法中（setTimeout）使用全局变量:

```js
myVar=setTimeout("javascript function",milliseconds);
```

如果函数还未被执行，你可以使用 clearTimeout() 方法来停止执行函数代码。



### JavaScript Cookie

 Cookie 用于存储 web 页面的用户信息。 



```js
function setCookie(cname,cvalue,exdays){
    var d = new Date();
    d.setTime(d.getTime()+(exdays*24*60*60*1000));
    var expires = "expires="+d.toGMTString();
    document.cookie = cname+"="+cvalue+"; "+expires;
}
function getCookie(cname){
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i=0; i<ca.length; i++) {
        var c = ca[i].trim();
        if (c.indexOf(name)==0) { return c.substring(name.length,c.length); }
    }
    return "";
}
function checkCookie(){
    var user=getCookie("username");
    if (user!=""){
        alert("欢迎 " + user + " 再次访问");
    }
    else {
        user = prompt("请输入你的名字:","");
          if (user!="" && user!=null){
            setCookie("username",user,30);
        }
    }
}
```



# 小结



## HTML一般语法

| 标记符范例              | 功能说明                                        |
| ----------------------- | ----------------------------------------------- |
| <PRE></PRE>             | 预先格式化文本                                  |
| <H1></H1>               | 最大的标题                                      |
| <H6></H6>               | 最小的标题                                      |
| <B></B>                 | 黑体字                                          |
| <I></I>                 | 斜体字                                          |
| <TT></TT>               | 打印机风格的字体                                |
| <CITE></CITE>           | 引用，通常是斜体                                |
| <EM></EM>               | 强调文本（通常是斜体加黑体）                    |
| <STRONG></STRONG>       | 加重文本（通常是斜体加黑体）                    |
| <FONT SIZE=" "></FONT>  | 设置字体大小，从1到7                            |
| <FONT COLOR=" "></FONT> | 设置字体的颜色，使用颜色的名字或RGB的十六进制值 |

## HTML的链接标志

| 标记符范例                | 功能说明                       |
| ------------------------- | ------------------------------ |
| <A HREF="URL"></A>        | 创建超文本链接                 |
| <A HREF=mailto:EMAIL></A> | 创建自动发送电子邮件的链接     |
| <A NAME="NAME"></A>       | 创建位于文档内部的书签         |
| <A HREF="#NAME"></A>      | 创建指向位于文档内部书签的链接 |

## HTML框架标志

| 标记符范例                  | 功能说明                                               |
| --------------------------- | ------------------------------------------------------ |
| <FRAMESET></FRAMESET>       | 放在一个框架文档<BODY>标签之前，也可嵌在其他框架文档中 |
| <FRAME ROWS="VALUE，VALUE"> | 定义一个框架内的行数，可以使用绝对像素值或高度的百分比 |
| <FRAME COLS="VALUE，VALUE"> | 定义一个框架内的列数，可以使用绝对像素值或宽度的百分比 |
| <NOFRAME></NOFRAMES>        | 定义在不支持框架的浏览器中显示什么提示                 |
| <FRAME SRC="URL">           | 规定框架内显示的HTML文档                               |
| <FRAME NAME="NAME">         | 命名框架或区域，使于别的框架可以指向它                 |
| <FRAME MARGINWIDTH=" ">     | 定义框架左右边缘的空白大小，必须大于等于1              |
| <FRAME MARGINHEIGHT=" " >   | 定义框架上下边缘的空白大小，必须大于等于1              |
| <FRAME SCROLLING=" ">       | 设置框架是否有滚动栏，其值可以是yes，no或auto          |
| <FRAME NORESIZE>            | 禁止用户调整一个框架的大小                             |

## HTML格式标志

| 标记符范例            | 功能说明                                                     |
| --------------------- | ------------------------------------------------------------ |
| <P></P>               | 创建一个段落                                                 |
| <P ALIGN=" ">         | 将段落按左、中、右对齐                                       |
| <BR>                  | 插入一个回车换行符                                           |
| <BLOCKQUOTE>          | 从两边缩进文本                                               |
| <DL></DL>             | 定义列表                                                     |
| <DT>                  | 放在每个定义术语词前                                         |
| <DD>                  | 放在每个定义之前                                             |
| <OL></OL>             | 创建一个标有数字的列表                                       |
| <UL></UL>             | 创建一个标有圆点的列表，定义一个框架内的行数                 |
| <LI>                  | 放在每个列表项之前，若在<OL></OL>之间则每个列表项加上一个数字，若在<UL></UL>之间则每个列表项加上一个圆点 |
| <DIV ALIGN=" "></DIV> | 用于排版大块的HTML段落，也用于格式化表                       |

## HTML表单标志

| 标记符范例                                              | 功能说明                                               |
| ------------------------------------------------------- | ------------------------------------------------------ |
| <FORM></FORM>                                           | 创建表单                                               |
| <SELECT MULTIPLE NAME="NAME"  SIZE=" "></SELECT>        | 创建滚动菜单，SIZE设置在需要滚动前可以看到的表单项数目 |
| <OPTION>                                                | 设置每个表单项的内容                                   |
| <SELECT NAME="NAME"</SELECT>                            | 创建下拉菜单                                           |
| <TEXTAREA NAME="NAME" COLS=40  ROWS=8></TEXTAREA>       | 创建一个文本框区域，列的数目设置宽度，行的数目设置高度 |
| <INPUT TYPE="CHECKBOX"  NAME="NAME">                    | 创建一个复选框，文字在标签后面                         |
| <INPUT TYPE="RADIO" NAME="NAME"VALUE=" ">               | 创建一个单选按钮，文字在标志后面                       |
| <INPUT TYPE=TEXT NAME="FOO"SIZE=20>                     | 创建一个单行文本输入区域，SIZE设置字符串宽度           |
| <INPUT TYPE="SUBMIT" VALUE="NAME">                      | 创建提交按钮                                           |
| <INPUT TYPE="IMAGE" BORDER=0NAME="NAME" SRC="NAME.GIF"> | 创建一个使用图像的提交按钮                             |
| <INPUT TYPE="RESET">                                    | 创建重置按钮                                           |



# Ajax

`Asynchronous JavaScript and XML`， 异步 JavaScript 和 XML 。 

AJAX 最大的优点是在不重新加载整个页面的情况下，可以与服务器交换数据并更新部分网页内容。 



## AJAX 工作原理

通过在后台与服务器进行少量数据交换，AJAX 可以使网页实现异步更新。这意味着可以在不重新加载整个网页的情况下，对网页的某部分进行更新。 

![](img/ajax-yl.png)



## 实例讲解



```html
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<script>
function loadXMLDoc()
{
	var xmlhttp;
	if (window.XMLHttpRequest)
	{
		//  IE7+, Firefox, Chrome, Opera, Safari 浏览器执行代码
		xmlhttp=new XMLHttpRequest();
	}
	else
	{
		// IE6, IE5 浏览器执行代码
		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
			document.getElementById("myDiv").innerHTML=xmlhttp.responseText;
		}
	}
	xmlhttp.open("GET","/try/ajax/ajax_info.txt",true);
	xmlhttp.send();
}
</script>
</head>
<body>

<div id="myDiv"><h2>使用 AJAX 修改该文本内容</h2></div>
<button type="button" onclick="loadXMLDoc()">修改内容</button>

</body>
</html>
```



### 响应

| 属性         | 描述                       |
| :----------- | :------------------------- |
| responseText | 获得字符串形式的响应数据。 |
| responseXML  | 获得 XML 形式的响应数据。  |



### onreadystatechange事件

| 属性               | 描述                                                         |
| :----------------- | :----------------------------------------------------------- |
| onreadystatechange | 存储函数（或函数名），每当 readyState 属性改变时，就会调用该函数。 |
| readyState         | 存有 XMLHttpRequest 的状态。从 0 到 4 发生变化。0: 请求未初始化1: 服务器连接已建立2: 请求已接收3: 请求处理中4: 请求已完成，且响应已就绪 |
| status             | 200: "OK" 404: 未找到页面                                    |



# Jquery

JQuery 是一个 JavaScript 库。 是一种JavaScript的框架。



## 什么是 jQuery

jQuery是一个非常受欢迎且广泛使用的JavaScript库，它既简化了JavaScript又增加了浏览器与JavaScript的兼容性。 

jQuery库包含以下功能：

- HTML 元素选取
- HTML 元素操作
- CSS 操作
- HTML 事件函数
- JavaScript 特效和动画
- HTML DOM 遍历和修改
- AJAX
- Utilities

**提示：** 除此之外，Jquery还提供了大量的插件。



## 下载安装

https://jquery.com/download/

有两个版本的 jQuery 可供下载：

- Production version - 用于实际的网站中，已被精简和压缩。
- Development version - 用于测试和开发（未压缩，是可读的代码）



## 使用

当把jQuery下载到本地后，可直接在html引用它。

```js
<script src="jquery-3.4.1.js">
</script>
```



如果不希望下载并存放 jQuery，那么也可以通过 CDN（内容分发网络） 引用它。 

```html
<head>
	<script src="https://apps.bdimg.com/libs/jquery/2.1.4/jquery.min.js">
	</script>
</head>
```



## jQuery AJAX



### jQuery load() 方法



### jQuery get/post 方法



## 关于 jQuery 与 AJAX

jQuery 提供多个与 AJAX 有关的方法。

通过 jQuery AJAX 方法，您能够使用 HTTP Get 和 HTTP Post 从远程服务器上请求文本、HTML、XML 或 JSON - 同时您能够把这些外部数据直接载入网页的被选元素中。



**如果没有 jQuery，AJAX 编程还是有些难度的。**编写常规的 AJAX 代码并不容易，因为不同的浏览器对 AJAX 的实现并不相同。这意味着您必须编写额外的代码对浏览器进行测试。不过，jQuery 团队为我们解决了这个难题，我们只需要一行简单的代码，就可以实现 AJAX 功能。



# BootStrap

Bootstrap 是一个用于快速开发 Web 应用程序和网站的前端框架。Bootstrap 是基于 HTML、CSS、JAVASCRIPT 的。

 Bootstrap需要[jQuery](https://jquery.com/)才能运行。jQuery是一个非常受欢迎且广泛使用的JavaScript库，它既简化了JavaScript又增加了浏览器与JavaScript的兼容性。 



https://www.cnblogs.com/xiaohuochai/p/7097376.html



