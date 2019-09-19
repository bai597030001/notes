# springmvc中@Controller注解的返回值
---

- 注意，applicationContext.xml中要开启注解驱动，配置视图解析器

`
	<!-- 可以用mvc的注解驱动 --> 
    <mvc:annotation-driven />

    <!-- 配置视图解析器 -->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <!-- 配置视图名的默认前缀 -->
        <property name="prefix" value="/"></property>
        <!-- 配置视图名的默认后缀 -->
        <property name="suffix" value=".jsp"></property>
    </bean>

## 1.字符串

- 如果想在控制器中直接返回字符串，则需要@ResponseBody注解

## 2.Model/ModelAndView/ModelMap/Map

## 3.void

- 没有返回值的情况下，通过ServletAPI完成参数传递和跳转


	@RequestMapping("/returnVoid")  
    public void doVoid(String vname, String vpassword, HttpServletRequest request, HttpServletResponse response)   throws Exception{  
        request.setAttribute("vname", vname);  
        request.setAttribute("vpassword", vpassword);  
        request.getRequestDispatcher("/showReturn.jsp").forward(request, response);  
    }

## 4.Object

- 返回Object对象需要开启消息转换器HttpMessageConverter

	@RequestMapping("/returnObjectValue")  
    @ResponseBody  
    public Object doObjectValue(){  
        return 12.34;  
    }

# springmvc中@Controller注解的请求参数

- 通过@RequestBody可以接受json格式的数据