## program arguments 与 VM arguments 的区别
---

- program arguments 中的值作为 args[] 的参数传入的。是要传给你的应用程序的，它通过主函数中的 args 来传值。

	方式:
		--server.port

- VM Arguments 是设置的虚拟机的属性。VM arguments 是系统的属性，要传给 java 虚拟机的。

	方式：
		-Dspring.config.location=/conf/application.property