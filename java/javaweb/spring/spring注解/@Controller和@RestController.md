[原文链接](https://www.cnblogs.com/shuaifing/p/8119664.html)

- @RestController 注解相当于 @ResponseBody ＋ @Controller 合在一起的作用。

1) 如果只是使用 @RestController 注解控制器，则控制器中的方法无法返回 jsp 页面，或者 html ，
			配置的视图解析器 InternalResourceViewResolver 不起作用，返回的内容就是 return 里的内容。
		 

2) 如果需要返回到指定页面，则需要用 @Controller配合视图解析器 InternalResourceViewResolver 才行。
		如果需要返回JSON，XML或自定义mediaType内容到页面，则需要在对应的方法上加上@ResponseBody注解。