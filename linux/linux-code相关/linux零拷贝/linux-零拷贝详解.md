# linux�е��㿽��

https://www.sohu.com/a/342256567_463994



�㿽����Zero-copy������ָ�ڼ����ִ�в���ʱ��CPU ����Ҫ�Ƚ����ݴ�һ���ڴ������Ƶ���һ���ڴ����򣬴Ӷ����Լ����������л��Լ� CPU �Ŀ���ʱ�䡣

���������������ݱ��������豸���û�����ռ䴫�ݵĹ����У��������ݿ�������������ϵͳ���ã�ʵ�� CPU ������룬�������� CPU ���ⷽ��ĸ��ء�

<font  color=#dd0000>ʵ���㿽���õ�������Ҫ������ DMA ���ݴ��似�����ڴ�����ӳ�似����</font>

- �㿽�����ƿ��Լ����������ں˻��������û����̻�����֮�䷴���� I/O ����������
- �㿽�����ƿ��Լ����û����̵�ַ�ռ���ں˵�ַ�ռ�֮����Ϊ�������л��������� CPU ������

**�����ڴ�������ڴ�**

���ڲ���ϵͳ�Ľ������û�����֮���ǹ��� CPU ���ڴ���Դ�ģ������Ҫһ�����Ƶ��ڴ������Ʒ�ֹ����֮���ڴ�й©�����⡣

Ϊ�˸�����Ч�ع����ڴ沢���ٳ����ִ�����ϵͳ�ṩ��һ�ֶ�����ĳ������������ڴ棨Virtual Memory����

�����ڴ�Ϊÿ�������ṩ��һ��һ�µġ�˽�еĵ�ַ�ռ䣬����ÿ�����̲�����һ���Լ��ڶ�������Ĵ����ÿ������ӵ��һƬ�����������ڴ�ռ䣩��

**�����ڴ�**

�����ڴ棨Physical Memory��������������ڴ棨Virtual Memory�����Եġ�

�����ڴ�ָͨ�������ڴ�������õ��ڴ�ռ䣬�������ڴ�����ָ��Ӳ�̵�һ�����򻮷�����Ϊ�ڴ档�ڴ���Ҫ�������ڼ��������ʱΪ����ϵͳ�͸��ֳ����ṩ��ʱ���档

��Ӧ���У���Ȼ�ǹ���˼�壬�����ϣ���ʵ���ڵĲ��������ڴ���ϵ��ڴ����������Ĵ�С��

**�����ڴ�**

�����ڴ��Ǽ����ϵͳ�ڴ�����һ�ּ�������ʹ��Ӧ�ó�����Ϊ��ӵ�������Ŀ��õ��ڴ棨һ�����������ĵ�ַ�ռ䣩��

��ʵ���ϣ������ڴ�ͨ���Ǳ��ָ��ɶ�������ڴ���Ƭ�����в�����ʱ�洢���ⲿ���̴洢���ϣ�����Ҫʱ�������ݽ��������ص������ڴ�������

Ŀǰ�����������ϵͳ��ʹ���������ڴ棬�� Windows ϵͳ�������ڴ桢Linux ϵͳ�Ľ����ռ�ȵȡ�

�����ڴ��ַ���û����̽�����أ�һ����˵��ͬ�������ͬһ�������ַָ��������ַ�ǲ�һ���ģ������뿪����̸�����ڴ�û���κ����塣ÿ����������ʹ�õ������ַ��С�� CPU λ���йء�

�� 32 λ��ϵͳ�ϣ������ַ�ռ��С�� 2^32=4G���� 64 λϵͳ�ϣ������ַ�ռ��С�� 2^64=16G����ʵ�ʵ������ڴ����ԶԶС�������ڴ�Ĵ�С��

ÿ���û�����ά����һ��������ҳ��Page Table���������ڴ�������ڴ����ͨ�����ҳ��ʵ�ֵ�ַ�ռ��ӳ��ġ�

��������������� A��B ���Ե������ڴ�ռ��Լ���Ӧ�������ڴ�֮��ĵ�ַӳ��ʾ��ͼ

![](img/linux-zero-copy1.png)

������ִ��һ������ʱ����Ҫ�ȴ��ڴ��ж�ȡ�ý��̵�ָ�Ȼ��ִ�У���ȡָ��ʱ�õ��ľ��������ַ��

��������ַ�ǳ�������ʱȷ���ģ��ں˼��ز���ʼ������ʱ�������̬��ĵ�ַ��Χ����

Ϊ�˻�ȡ��ʵ�ʵ����ݣ�CPU ��Ҫ�������ַת���������ַ��CPU ת����ַʱ��Ҫ�õ����̵�ҳ��Page Table������ҳ��Page Table������������ɲ���ϵͳά����

����ҳ��Page Table�����Լ򵥵����Ϊ�����ڴ�ӳ�䣨Memory Mapping����������Ȼʵ�ʽṹ�ܸ��ӣ���

�����ÿ���ڴ�ӳ�䣨Memory Mapping������һ�������ַӳ�䵽һ���ض��ĵ�ַ�ռ䣨�����ڴ���ߴ��̴洢�ռ䣩��

ÿ������ӵ���Լ���ҳ��Page Table�������������̵�ҳ��Page Table��û�й�ϵ��

ͨ������Ľ��ܣ����ǿ��Լ򵥵Ľ��û��������벢���������ڴ棨����̴洢�ռ䣩�Ĺ����ܽ����£�

- �û����������ϵͳ�����ڴ���������
- ϵͳ������̵������ַ�ռ��Ƿ����꣬�����ʣ�࣬�����̷��������ַ��
- ϵͳΪ��������ַ�����ڴ�ӳ�䣨Memory Mapping�����������Ž��ý��̵�ҳ��Page Table����
- ϵͳ���������ַ���û����̣��û����̿�ʼ���ʸ������ַ��
- CPU ���������ַ�ڴ˽��̵�ҳ��Page Table�����ҵ�����Ӧ���ڴ�ӳ�䣨Memory Mapping������������ڴ�ӳ�䣨Memory Mapping��û�к������ڴ���������ǲ���ȱҳ�жϡ�
- ����ϵͳ�յ�ȱҳ�жϺ󣬷��������������ڴ沢����������ҳ����Ӧ���ڴ�ӳ�䣨Memory Mapping�����жϴ�����ɺ�CPU �Ϳ��Է����ڴ���
- ��Ȼȱҳ�жϲ���ÿ�ζ��ᷢ����ֻ��ϵͳ�����б�Ҫ�ӳٷ����ڴ��ʱ����õ��ţ�Ҳ���ܶ�ʱ��������ĵ� 3 ��ϵͳ����������������ڴ沢���ڴ�ӳ�䣨Memory Mapping�����й�����

���û����̺������ڴ棨���̴洢����֮�����������ڴ���Ҫ�����µ��ŵ㣺

- ��ַ�ռ䣺�ṩ����ĵ�ַ�ռ䣬���ҵ�ַ�ռ��������ģ�ʹ�ó����д�����Ӹ��Ӽ򵥡�
- ���̸��룺��ͬ���̵������ַ֮��û�й�ϵ������һ�����̵Ĳ�������������������Ӱ�졣
- ���ݱ�����ÿ�������ڴ涼����Ӧ�Ķ�д���ԣ��������ܱ�������Ĵ���β����޸ģ����ݿ鲻�ܱ�ִ�еȣ�������ϵͳ�İ�ȫ�ԡ�
- �ڴ�ӳ�䣺���������ڴ�֮�󣬿���ֱ��ӳ������ϵ��ļ�����ִ���ļ���̬�⣩�������ַ�ռ䡣 �����������������ڴ���ʱ���䣬ֻ������Ҫ����Ӧ���ļ���ʱ�򣬲Ž��������ĴӴ����ϼ��ص��ڴ������������ڴ�Խ���ʱ���ֿ��Խ��ⲿ���ڴ���յ�����������ڴ�����Ч�ʣ�����������Щ��Ӧ�ó�����͸���ġ�
- �����ڴ棺���綯̬��ֻ��Ҫ���ڴ��д洢һ�ݣ�Ȼ����ӳ�䵽��ͬ���̵������ַ�ռ��У��ý��̾����Լ���ռ������ļ��� ���̼���ڴ湲��Ҳ����ͨ��ӳ��ͬһ�������ڴ浽���̵Ĳ�ͬ�����ַ�ռ���ʵ�ֹ���
- �����ڴ���������ַ�ռ�ȫ���ɲ���ϵͳ���������޷�ֱ�ӷ���ͻ��գ��Ӷ�ϵͳ���Ը��õ������ڴ棬ƽ����̼���ڴ������

�ں˿ռ���û��ռ�

����ϵͳ�ĺ������ںˣ���������ͨ��Ӧ�ó��򣬿��Է����ܱ������ڴ�ռ䣬Ҳ�з��ʵײ�Ӳ���豸��Ȩ�ޡ�

Ϊ�˱����û�����ֱ�Ӳ����ںˣ���֤�ں˰�ȫ������ϵͳ�������ڴ滮��Ϊ�����֣�һ�������ں˿ռ䣨Kernel-space����һ�������û��ռ䣨User-space����

�� Linux ϵͳ�У��ں�ģ���������ں˿ռ䣬��Ӧ�Ľ��̴����ں�̬�����û������������û��ռ䣬��Ӧ�Ľ��̴����û�̬��

�ں˽��̺��û�������ռ�������ڴ������ 1:3���� Linux x86_32 ϵͳ��Ѱַ�ռ䣨����洢�ռ䣩Ϊ 4G��2 �� 32 �η���������ߵ� 1G ���ֽڣ��������ַ 0xC0000000 �� 0xFFFFFFFF�����ں˽���ʹ�ã���Ϊ�ں˿ռ䡣

���ϵ͵� 3G ���ֽڣ��������ַ 0x00000000 �� 0xBFFFFFFF�����������û�����ʹ�ã���Ϊ�û��ռ䡣

��ͼ��һ�����̵��û��ռ���ں˿ռ���ڴ沼�֣�

![](img/linux-zero-copy2.jpeg)

**�ں˿ռ�**

�ں˿ռ�����פ�����ڴ��У�����Ϊ����ϵͳ���ں˱����ġ�Ӧ�ó����ǲ�����ֱ���ڸ�������ж�д��ֱ�ӵ����ں˴��붨��ĺ����ġ�

��ͼ�������Ϊ�ں˽��̶�Ӧ�������ڴ棬������Ȩ�޿��Է�Ϊ����˽�кͽ��̹�����������

- ����˽�е������ڴ棺ÿ�����̶��е������ں�ջ��ҳ��task �ṹ�Լ� mem_map �ṹ�ȡ�
- ���̹���������ڴ棺�������н��̹�����ڴ����򣬰�������洢�����ں����ݺ��ں˴�������

**�û��ռ�**

ÿ����ͨ���û����̶���һ���������û��ռ䣬�����û�̬�Ľ��̲��ܷ����ں˿ռ��е����ݣ�Ҳ����ֱ�ӵ����ں˺����� �����Ҫ����ϵͳ���õ�ʱ�򣬾�Ҫ�������л����ں�̬���С�

�û��ռ�������¼����ڴ�����

- ����ʱջ���ɱ������Զ��ͷţ���ź����Ĳ���ֵ���ֲ������ͷ�������ֵ�ȡ�ÿ��һ������������ʱ���ú����ķ������ͺ�һЩ���õ���Ϣ���洢��ջ�������ý����������Ϣ�ᱻ�������ͷŵ��ڴ档 ջ���ǴӸߵ�ַλ��͵�ַλ�����ģ���һ���������������������������ϵͳԤ�ȶ���õģ������ջ�ռ䳬���������ʱ����ʾ������û��ܴ�ջ�л�ȡ�Ŀռ��С��
- ����ʱ�ѣ����ڴ�Ž��������б���̬������ڴ�Σ�λ�� BSS ��ջ�м�ĵ�ַλ���ɿ�����Ա������䣨malloc�����ͷţ�free�������Ǵӵ͵�ַλ��ߵ�ַλ������������ʽ�洢�ṹ�� Ƶ���� malloc/free ����ڴ�ռ�Ĳ�����������������Ƭ��������ѿռ�ʱ���⺯������һ�����㷨�������õ��㹻��Ŀռ䡣��˶ѵ�Ч�ʱ�ջҪ�͵Ķࡣ
- ����Σ���� CPU ����ִ�еĻ���ָ��ò����ڴ�ֻ�ܶ�����д��ͨ���������ǹ���ģ�������ִ�г���ɵ��������������������������������ͬ��һ��������ô���ǾͿ���ʹ��ͬһ������Ρ�
- δ��ʼ�������ݶΣ����δ��ʼ����ȫ�ֱ�����BSS �������ڳ���ʼִ��֮ǰ����ʼ��Ϊ 0 �� NULL��
- �ѳ�ʼ�������ݶΣ�����ѳ�ʼ����ȫ�ֱ�����������̬ȫ�ֱ�������̬�ֲ������Լ�������
- �ڴ�ӳ���������罫��̬�⣬�����ڴ������ռ���ڴ�ӳ�䵽����ռ���ڴ棬һ���� mmap ����������������ڴ�ռ䡣

Linux ���ڲ��㼶�ṹ

�ں�̬����ִ�������������ϵͳ��һ����Դ�����û�ֻ̬��ִ�м򵥵����㣬����ֱ�ӵ���ϵͳ��Դ���û�̬����ͨ��ϵͳ�ӿڣ�System Call�����������ں˷���ָ�

![](img/linux-zero-copy3.png)

���磬���û���������һ�� bash ʱ������ͨ�� getpid ���ں˵� pid ������ϵͳ���ã���ȡ��ǰ�û����̵� ID��

���û�����ͨ�� cat ����鿴��������ʱ��������ں˵��ļ���ϵͳ����ϵͳ���ã�

- �ں˿ռ���Է������е� CPU ָ������е��ڴ�ռ䡢I/O �ռ��Ӳ���豸��
- �û��ռ�ֻ�ܷ������޵���Դ�������Ҫ����Ȩ�ޣ�����ͨ��ϵͳ���û�ȡ��Ӧ����Դ��
- �û��ռ�����ҳ���жϣ����ں˿ռ�������
- �ں˿ռ���û��ռ���������Ե�ַ�ռ�ġ�
- x86 CPU ���û��ռ��� 0-3G �ĵ�ַ��Χ���ں˿ռ��� 3G-4G �ĵ�ַ��Χ�� x86_64 CPU �û��ռ��ַ��ΧΪ0x0000000000000000�C0x00007fffffffffff���ں˵�ַ�ռ�Ϊ 0xffff880000000000-����ַ��
- �����ں˽��̣��̣߳�����һ����ַ�ռ䣬���û����̶��и��Եĵ�ַ�ռ䡣

�����û��ռ���ں˿ռ�Ļ��ֺ�Linux �ڲ��㼶�ṹ���Է�Ϊ�����֣�����ײ㵽���ϲ�������Ӳ�����ں˿ռ���û��ռ䣬����ͼ��ʾ��

![](img/linux-zero-copy4.png)

Linux I/O ��д��ʽ

Linux �ṩ����ѯ��I/O �ж��Լ� DMA ������ 3 �ִ���������֮������ݴ�����ơ�������ѯ��ʽ�ǻ�����ѭ���� I/O �˿ڽ��в��ϼ�⡣

I/O �жϷ�ʽ��ָ�����ݵ���ʱ������������ CPU �����ж������� CPU ���������ݵĴ�����̡�

DMA �������� I/O �жϵĻ����������� DMA ���̿��������� DMA ���̿������������ݵĴ��䣬������ I/O �жϲ����� CPU ��Դ�Ĵ������ġ�

**I/O �ж�ԭ��**

�� DMA ��������֮ǰ��Ӧ�ó��������֮��� I/O ��������ͨ�� CPU ���ж���ɵġ�

![](img/linux-zero-copy5.png)

ÿ���û����̶�ȡ��������ʱ������Ҫ CPU �жϣ�Ȼ���� I/O ����ȴ����ݶ�ȡ�Ϳ�����ɣ�ÿ�ε� I/O �ж϶����� CPU ���������л���

- �û������� CPU ���� read ϵͳ���ö�ȡ���ݣ����û�̬�л�Ϊ�ں�̬��Ȼ��һֱ�����ȴ����ݵķ��ء�
- CPU �ڽ��յ�ָ���Ժ�Դ��̷��� I/O ���󣬽����������ȷ�����̿�������������
- ����׼������Ժ󣬴����� CPU ���� I/O �жϡ�
- CPU �յ� I/O �ж��Ժ󽫴��̻������е����ݿ������ں˻�������Ȼ���ٴ��ں˻������������û���������
- �û��������ں�̬�л����û�̬���������״̬��Ȼ��ȴ� CPU ����һ��ִ��ʱ���ӡ�

**DMA ����ԭ��**

DMA ��ȫ�ƽ�ֱ���ڴ��ȡ��Direct Memory Access������һ��������Χ�豸��Ӳ����ϵͳ��ֱ�ӷ���ϵͳ���ڴ�Ļ��ơ�

Ҳ����˵������ DMA ���ʷ�ʽ��ϵͳ���ڴ���Ӳ�̻�����֮������ݴ�������ƿ� CPU ��ȫ�̵��ȡ�

Ŀǰ�������Ӳ���豸���������̿��������������Կ��Լ������ȶ�֧�� DMA ������

![](img/linux-zero-copy6.png)

�������ݴ��������һ�� DMA �������Ŀ����½��еġ�CPU ���������ݴ��俪ʼ�ͽ���ʱ��һ�㴦���⣨��ʼ�ͽ���ʱ��Ҫ���жϴ������ڴ�������� CPU ���Լ������������Ĺ�����

�����ڴ󲿷�ʱ���CPU ����� I/O ���������ڲ��в�����ʹ���������ϵͳ��Ч�ʴ����ߡ�

![](img/linux-zero-copy7.jpeg)

���� DMA ���̿������ӹ����ݶ�д�����Ժ�CPU �ӷ��ص� I/O �����н��ѣ����ݶ�ȡ�������������£�

- �û������� CPU ���� read ϵͳ���ö�ȡ���ݣ����û�̬�л�Ϊ�ں�̬��Ȼ��һֱ�����ȴ����ݵķ��ء�
- CPU �ڽ��յ�ָ���Ժ�� DMA ���̿������������ָ�
- DMA ���̿������Դ��̷��� I/O ���󣬽����������ȷ�����̿�������������CPU ȫ�̲�����˹��̡�
- ���ݶ�ȡ��ɺ�DMA ���̿���������ܵ����̵�֪ͨ�������ݴӴ��̿������������������ں˻�������
- DMA ���̿������� CPU �������ݶ�����źţ��� CPU �������ݴ��ں˻������������û���������
- �û��������ں�̬�л����û�̬���������״̬��Ȼ��ȴ� CPU ����һ��ִ��ʱ���ӡ�

��ͳ I/O ��ʽ

Ϊ�˸��õ�����㿽����������⣬���������˽�һ�´�ͳ I/O ��ʽ���ڵ����⡣

�� Linux ϵͳ�У���ͳ�ķ��ʷ�ʽ��ͨ�� write �� read ����ϵͳ����ʵ�ֵģ�ͨ�� read ������ȡ�ļ������������У�Ȼ��ͨ�� write �����ѻ����е��������������˿ڡ�

α�������£�

```c
read(file_fd, tmp_buf, len);

write(socket_fd, tmp_buf, len);
```

��ͼ�ֱ��Ӧ��ͳ I/O ���������ݶ�д���̣����������漰 2 �� CPU ������2 �� DMA �������ܹ� 4 �ο������Լ� 4 ���������л���

![](img/linux-zero-copy8.png)

����򵥵ز���һ����صĸ��

- �������л������û��������ں˷���ϵͳ����ʱ��CPU ���û����̴��û�̬�л����ں�̬����ϵͳ���÷���ʱ��CPU ���û����̴��ں�̬�л����û�̬��
- CPU �������� CPU ֱ�Ӵ������ݵĴ��ͣ����ݿ���ʱ��һֱռ�� CPU ����Դ��
- DMA �������� CPU ��DMA���̿������´�ָ��� DMA ���������������ݵĴ��ͣ����ݴ�������ٰ���Ϣ������ CPU���Ӷ������� CPU ��Դ��ռ���ʡ�

**��ͳ������**

��Ӧ�ó���ִ�� read ϵͳ���ö�ȡһ�����ݵ�ʱ�������������Ѿ��������û����̵�ҳ�ڴ��У���ֱ�Ӵ��ڴ��ж�ȡ���ݡ�

������ݲ����ڣ����Ƚ����ݴӴ��̼������ݵ��ں˿ռ�Ķ����棨read buffer���У��ٴӶ����濽�����û����̵�ҳ�ڴ��С�

read(file_fd, tmp_buf, len);

���ڴ�ͳ�� I/O ��ȡ��ʽ��read ϵͳ���ûᴥ�� 2 ���������л���1 �� DMA ������ 1 �� CPU ������

�������ݶ�ȡ���������£�

- �û�����ͨ�� read �������ںˣ�kernel������ϵͳ���ã������Ĵ��û�̬��user space���л�Ϊ�ں�̬��kernel space����
- CPU ���� DMA �����������ݴ������Ӳ�̿������ں˿ռ䣨kernel space���Ķ���������read buffer����
- CPU ������������read buffer���е����ݿ������û��ռ䣨user space�����û���������user buffer����
- �����Ĵ��ں�̬��kernel space���л����û�̬��user space����read ����ִ�з��ء�

**��ͳд����**

��Ӧ�ó���׼�������ݣ�ִ�� write ϵͳ���÷�����������ʱ���Ƚ����ݴ��û��ռ��ҳ���濽�����ں˿ռ�����绺������socket buffer���У�Ȼ���ٽ�д�����е����ݿ����������豸������ݷ��͡�

write(socket_fd, tmp_buf, len);

���ڴ�ͳ�� I/O д�뷽ʽ��write ϵͳ���ûᴥ�� 2 ���������л���1 �� CPU ������ 1 �� DMA ������

�û��������������ݵ��������£�

- �û�����ͨ�� write �������ںˣ�kernel������ϵͳ���ã������Ĵ��û�̬��user space���л�Ϊ�ں�̬��kernel space����
- CPU ���û���������user buffer���е����ݿ������ں˿ռ䣨kernel space�������绺������socket buffer����
- CPU ���� DMA �����������ݴ����绺������socket buffer�������������������ݴ��䡣
- �����Ĵ��ں�̬��kernel space���л����û�̬��user space����write ϵͳ����ִ�з��ء�

�㿽����ʽ

�� Linux ���㿽��������Ҫ�� 3 ��ʵ��˼·��

- �û�ֱ̬�� I/O��Ӧ�ó������ֱ�ӷ���Ӳ���洢������ϵͳ�ں�ֻ�Ǹ������ݴ��䡣 ���ַ�ʽ���ɴ����û��ռ���ں˿ռ���������л���Ӳ���ϵ�����ֱ�ӿ��������û��ռ䣬�������ں˿ռ䡣��ˣ�ֱ�� I/O �������ں˿ռ仺�������û��ռ仺����֮������ݿ�����
- �������ݿ��������������ݴ�������У������������û��ռ仺������ϵͳ�ں˿ռ仺����֮��� CPU �������Լ�������ϵͳ�ں˿ռ��ڵ� CPU ��������Ҳ�ǵ�ǰ�����㿽��������ʵ��˼·��
- дʱ���Ƽ�����дʱ����ָ���ǵ�������̹���ͬһ������ʱ���������һ��������Ҫ��������ݽ����޸ģ���ô���俽�����Լ��Ľ��̵�ַ�ռ��У����ֻ�����ݶ�ȡ��������Ҫ���п���������

**�û�ֱ̬�� I/O**

�û�ֱ̬�� I/O ʹ��Ӧ�ý��̻��������û�̬��user space���µĿ⺯��ֱ�ӷ���Ӳ���豸��

����ֱ�ӿ���ں˽��д��䣬�ں������ݴ�����̳��˽��б�Ҫ������洢���ù���֮�⣬�������κ��������������ַ�ʽ�ܹ�ֱ���ƹ��ںˣ�������������ܡ�

![](img/linux-zeor-copy9.png)

�û�ֱ̬�� I/O ֻ�������ڲ���Ҫ�ں˻����������Ӧ�ó�����ЩӦ�ó���ͨ���ڽ��̵�ַ�ռ����Լ������ݻ�����ƣ���Ϊ�Ի���Ӧ�ó��������ݿ����ϵͳ����һ������

��Σ������㿽�����ƻ�ֱ�Ӳ������� I/O������ CPU �ʹ��� I/O ֮���ִ��ʱ���࣬����ɴ�����Դ���˷ѣ��������������첽 I/O ʹ�á�

**mmap+write**

һ���㿽����ʽ��ʹ�� mmap+write ����ԭ���� read+write ��ʽ�������� 1 �� CPU ����������

mmap �� Linux �ṩ��һ���ڴ�ӳ���ļ�����������һ�����̵ĵ�ַ�ռ��е�һ�������ַӳ�䵽�����ļ���ַ��mmap+write ��α�������£�

tmp_buf = mmap(file_fd, len);

write(socket_fd, tmp_buf, len);

ʹ�� mmap ��Ŀ���ǽ��ں��ж���������read buffer���ĵ�ַ���û��ռ�Ļ�������user buffer������ӳ�䡣

�Ӷ�ʵ���ں˻�������Ӧ�ó����ڴ�Ĺ���ʡȥ�˽����ݴ��ں˶���������read buffer���������û���������user buffer���Ĺ��̡�

Ȼ���ں˶���������read buffer�����轫���ݿ������ں�д��������socket buffer�������µ���������ͼ��ʾ��

![](img/linux-zero-copy10.png)

���� mmap+write ϵͳ���õ��㿽����ʽ�������������̻ᷢ�� 4 ���������л���1 �� CPU ������ 2 �� DMA ������

�û������д���ݵ��������£�

- �û�����ͨ�� mmap �������ںˣ�kernel������ϵͳ���ã������Ĵ��û�̬��user space���л�Ϊ�ں�̬��kernel space����
- ���û����̵��ں˿ռ�Ķ���������read buffer�����û��ռ�Ļ�������user buffer�������ڴ��ַӳ�䡣
- CPU ���� DMA �����������ݴ������Ӳ�̿������ں˿ռ䣨kernel space���Ķ���������read buffer����
- �����Ĵ��ں�̬��kernel space���л����û�̬��user space����mmap ϵͳ����ִ�з��ء�
- �û�����ͨ�� write �������ںˣ�kernel������ϵͳ���ã������Ĵ��û�̬��user space���л�Ϊ�ں�̬��kernel space����
- CPU ������������read buffer���е����ݿ��������绺������socket buffer����
- CPU ���� DMA �����������ݴ����绺������socket buffer�������������������ݴ��䡣
- �����Ĵ��ں�̬��kernel space���л����û�̬��user space����write ϵͳ����ִ�з��ء�

mmap ��Ҫ���ô������ I/O ���ܣ��ر�����Դ��ļ�������С�ļ����ڴ�ӳ���ļ������ᵼ����Ƭ�ռ���˷ѡ�

��Ϊ�ڴ�ӳ������Ҫ����ҳ�߽磬��С��λ�� 4 KB��һ�� 5 KB ���ļ�����ӳ��ռ�� 8 KB �ڴ棬Ҳ�ͻ��˷� 3 KB �ڴ档

mmap �Ŀ�����Ȼ������ 1 �ο�����������Ч�ʣ���Ҳ����һЩ���ص����⡣

�� mmap һ���ļ�ʱ���������ļ�����һ���������ػ���ô write ϵͳ���û���Ϊ���ʷǷ���ַ�� SIGBUS �ź���ֹ��SIGBUS Ĭ�ϻ�ɱ�����̲�����һ�� coredump��������������˱���ֹ��

**Sendfile**

Sendfile ϵͳ������ Linux �ں˰汾 2.1 �б����룬Ŀ���Ǽ�ͨ������������ͨ��֮����е����ݴ�����̡�

Sendfile ϵͳ���õ����룬���������� CPU �����Ĵ��������������������л��Ĵ���������α�������£�

sendfile(socket_fd, file_fd, len);

ͨ�� Sendfile ϵͳ���ã����ݿ���ֱ�����ں˿ռ��ڲ����� I/O ���䣬�Ӷ�ʡȥ���������û��ռ���ں˿ռ�֮������ؿ�����

�� mmap �ڴ�ӳ�䷽ʽ��ͬ���ǣ� Sendfile ������ I/O ���ݶ��û��ռ�����ȫ���ɼ��ġ�Ҳ����˵������һ����ȫ�����ϵ����ݴ�����̡�

![](img/linux-zero-copy11.png)

���� Sendfile ϵͳ���õ��㿽����ʽ�������������̻ᷢ�� 2 ���������л���1 �� CPU ������ 2 �� DMA ������

�û������д���ݵ��������£�

- �û�����ͨ�� sendfile �������ںˣ�kernel������ϵͳ���ã������Ĵ��û�̬��user space���л�Ϊ�ں�̬��kernel space����
- CPU ���� DMA �����������ݴ������Ӳ�̿������ں˿ռ䣨kernel space���Ķ���������read buffer����
- CPU ������������read buffer���е����ݿ����������绺������socket buffer����
- CPU ���� DMA �����������ݴ����绺������socket buffer�������������������ݴ��䡣
- �����Ĵ��ں�̬��kernel space���л����û�̬��user space����Sendfile ϵͳ����ִ�з��ء�

��Ƚ��� mmap �ڴ�ӳ��ķ�ʽ��Sendfile ���� 2 ���������л���������Ȼ�� 1 �� CPU ����������

Sendfile ���ڵ��������û������ܶ����ݽ����޸ģ���ֻ�ǵ����������һ�����ݴ�����̡�

**Sendfile+DMA gather copy**

Linux 2.4 �汾���ں˶� Sendfile ϵͳ���ý����޸ģ�Ϊ DMA ���������� gather ������

�����ں˿ռ䣨kernel space���Ķ���������read buffer���ж�Ӧ������������Ϣ���ڴ��ַ����ַƫ��������¼����Ӧ�����绺������ socket buffer���У��� DMA �����ڴ��ַ����ַƫ���������������شӶ���������read buffer�������������豸�С�

������ʡȥ���ں˿ռ��н�ʣ�� 1 �� CPU ����������Sendfile ��α�������£�

sendfile(socket_fd, file_fd, len);

��Ӳ����֧���£�Sendfile ������ʽ���ٴ��ں˻����������ݿ����� socket ��������ȡ����֮�Ľ����ǻ������ļ������������ݳ��ȵĿ�����

���� DMA ����ֱ������ gather ������ҳ���������ݴ�����͵������м��ɣ����ʾ��Ǻ������ڴ�ӳ���˼·���ơ�

![](img/linux-zero-copy12.png)

���� Sendfile+DMA gather copy ϵͳ���õ��㿽����ʽ�������������̻ᷢ�� 2 ���������л���0 �� CPU �����Լ� 2 �� DMA ������

�û������д���ݵ��������£�

- �û�����ͨ�� sendfile �������ںˣ�kernel������ϵͳ���ã������Ĵ��û�̬��user space���л�Ϊ�ں�̬��kernel space����
- CPU ���� DMA �����������ݴ������Ӳ�̿������ں˿ռ䣨kernel space���Ķ���������read buffer����
- CPU �Ѷ���������read buffer�����ļ���������file deor�������ݳ��ȿ��������绺������socket buffer����
- �����ѿ������ļ���������file deor�������ݳ��ȣ�CPU ���� DMA �������� gather/scatter ����ֱ�������ؽ����ݴ��ں˵Ķ���������read buffer�������������������ݴ��䡣
- �����Ĵ��ں�̬��kernel space���л����û�̬��user space����Sendfile ϵͳ����ִ�з��ء�

Sendfile+DMA gather copy ������ʽͬ�������û������ܶ����ݽ����޸ĵ����⣬���ұ�����ҪӲ����֧�֣���ֻ�����ڽ����ݴ��ļ������� socket �׽����ϵĴ�����̡�

**Splice**

Sendfile ֻ�����ڽ����ݴ��ļ������� socket �׽����ϣ�ͬʱ��ҪӲ����֧�֣���Ҳ�޶�������ʹ�÷�Χ��

Linux �� 2.6.17 �汾���� Splice ϵͳ���ã���������ҪӲ��֧�֣���ʵ���������ļ�������֮��������㿽����

Splice ��α�������£�

splice(fd_in, off_in, fd_out, off_out, len, flags);

Splice ϵͳ���ÿ������ں˿ռ�Ķ���������read buffer�������绺������socket buffer��֮�佨���ܵ���pipeline�����Ӷ�����������֮��� CPU ����������

![](img/linux-zero-copy13.png)

���� Splice ϵͳ���õ��㿽����ʽ�������������̻ᷢ�� 2 ���������л���0 �� CPU �����Լ� 2 �� DMA ������

�û������д���ݵ��������£�

- �û�����ͨ�� splice �������ںˣ�kernel������ϵͳ���ã������Ĵ��û�̬��user space���л�Ϊ�ں�̬��kernel space����
- CPU ���� DMA �����������ݴ������Ӳ�̿������ں˿ռ䣨kernel space���Ķ���������read buffer����
- CPU ���ں˿ռ�Ķ���������read buffer�������绺������socket buffer��֮�佨���ܵ���pipeline����
- CPU ���� DMA �����������ݴ����绺������socket buffer�������������������ݴ��䡣
- �����Ĵ��ں�̬��kernel space���л����û�̬��user space����Splice ϵͳ����ִ�з��ء�

Splice ������ʽҲͬ�������û������ܶ����ݽ����޸ĵ����⡣����֮�⣬��ʹ���� Linux �Ĺܵ�������ƣ������������������ļ��������д������ݣ��������������ļ���������������һ�������ǹܵ��豸��

**дʱ����**

��ĳЩ����£��ں˻��������ܱ�����������������ĳ��������Ҫ������������� write ���������� write ���ṩ�κε�����������ô�ͻ�Թ������е���������ƻ���дʱ���Ƶ�������� Linux �����������ݵġ�

дʱ����ָ���ǵ�������̹���ͬһ������ʱ���������һ��������Ҫ��������ݽ����޸ģ���ô����Ҫ���俽�����Լ��Ľ��̵�ַ�ռ��С�

����������Ӱ���������̶�������ݵĲ�����ÿ������Ҫ�޸ĵ�ʱ��Ż���п��������Խ�дʱ������

���ַ�����ĳ�̶ֳ����ܹ�����ϵͳ���������ĳ��������Զ����������ʵ����ݽ��и��ģ���ôҲ����Զ����Ҫ������

**����������**

����������ʽ��ȫ��д�˴�ͳ�� I/O ��������Ϊ��ͳ I/O �ӿڶ��ǻ������ݿ������еģ�Ҫ���⿽���͵�ȥ��ԭ�ȵ����׽ӿڲ����¸�д��

�������ַ����ǱȽ�ȫ����㿽��������Ŀǰ�Ƚϳ����һ���������� Solaris ��ʵ�ֵ� fbuf��Fast Buffer�����ٻ���������

fbuf ��˼����ÿ�����̶�ά����һ���������أ�������������ܱ�ͬʱӳ�䵽�û��ռ䣨user space�����ں�̬��kernel space�����ں˺��û���������������أ������ͱ�����һϵ�еĿ���������

![](img/linux-zero-copy14.png)

������������Ѷ����ڹ�������������ҪӦ�ó�����������Լ��豸��������֮��Ľ��ܺ�����������θ�д API Ŀǰ����������׶β������졣

Linux �㿽���Ա�

�����Ǵ�ͳ I/O ������ʽ���������㿽���ķ�ʽ��2 �� DMA Copy �Ƕ��ٲ��˵ģ���Ϊ���� DMA ��������Ӳ����ɵġ�

����� CPU ����������DMA ���������Լ�ϵͳ���ü��������ܽ�һ���������� I/O ������ʽ�Ĳ��

![](img/linux-zero-copy15.png)



# Java NIO �㿽��ʵ��

�� Java NIO �е�ͨ����Channel�����൱�ڲ���ϵͳ���ں˿ռ䣨kernel space���Ļ�������

����������Buffer����Ӧ���൱�ڲ���ϵͳ���û��ռ䣨user space���е��û���������user buffer����

- ͨ����Channel����ȫ˫���ģ�˫���䣩�����ȿ����Ƕ���������read buffer����Ҳ���������绺������socket buffer����
- ��������Buffer����Ϊ���ڴ棨HeapBuffer���Ͷ����ڴ棨DirectBuffer��������ͨ�� malloc ����������û�̬�ڴ档

�����ڴ棨DirectBuffer����ʹ�ú���ҪӦ�ó����ֶ����գ������ڴ棨HeapBuffer���������� GC ʱ���ܻᱻ�Զ����ա�

��ˣ���ʹ�� HeapBuffer ��д����ʱ��Ϊ�˱��⻺����������Ϊ GC ����ʧ��NIO ���Ȱ� HeapBuffer �ڲ������ݿ�����һ����ʱ�� DirectBuffer �еı����ڴ棨native memory����

��������漰�� sun.misc.Unsafe.copyMemory �ĵ��ã������ʵ��ԭ���� memcpy ���ơ�

��󣬽���ʱ���ɵ� DirectBuffer �ڲ������ݵ��ڴ��ַ���� I/O ���ú����������ͱ�������ȥ���� Java ������ I/O ��д��

**MappedByteBuffer**

MappedByteBuffer �� NIO �����ڴ�ӳ�䣨mmap�������㿽����ʽ�ṩ��һ��ʵ�֣����̳��� ByteBuffer��

FileChannel ������һ�� map �����������԰�һ���ļ��� position λ�ÿ�ʼ�� size ��С������ӳ��Ϊ�ڴ�ӳ���ļ���

���󷽷� map ������ FileChannel �еĶ������£�

```java
public abstract MappedByteBuffer map(MapMode mode, longposition, longsize) throwsIOException;
```

**Mode��**�޶��ڴ�ӳ������MappedByteBuffer�����ڴ�ӳ���ļ��ķ���ģʽ������ֻ�ɶ���READ_ONLY�����ɶ���д��READ_WRITE����дʱ������PRIVATE������ģʽ��

**Position��**�ļ�ӳ�����ʼ��ַ����Ӧ�ڴ�ӳ������MappedByteBuffer�����׵�ַ��

**Size��**�ļ�ӳ����ֽڳ��ȣ��� Position ������ֽ�������Ӧ�ڴ�ӳ������MappedByteBuffer���Ĵ�С��

MappedByteBuffer ��� ByteBuffer ������������Ҫ�ķ�����

- fore�����ڴ��� READ_WRITE ģʽ�µĻ��������ѶԻ��������ݵ��޸�ǿ��ˢ�µ������ļ���
- load�������������������������ڴ��У���������������������á�
- isLoaded������������������������ڴ��У��򷵻� true�����򷵻� false��

�������һ������ MappedByteBuffer ���ļ����ж�д��ʹ��ʾ����

```java
private final static String CONTENT = "Zero copy implemented by MappedByteBuffer";

private final static String FILE_NAME = "/mmap.txt";

private final static String CHARSET = "UTF-8";
```



**д�ļ����ݣ�**���ļ�ͨ�� fileChannel ���ṩ��Ȩ�ޡ�дȨ�޺��������Ȩ�ޣ�ͨ�� fileChannel ӳ�䵽һ����д���ڴ滺���� mappedByteBuffer����Ŀ������д�� mappedByteBuffer��ͨ�� force �����ѻ��������ĵ�����ǿ��д�뱾���ļ���

```java
@ Test
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



**���ļ����ݣ�**���ļ�ͨ�� fileChannel ���ṩֻ��Ȩ�ޣ�ͨ�� fileChannel ӳ�䵽һ��ֻ�ɶ����ڴ滺���� mappedByteBuffer����ȡ mappedByteBuffer �е��ֽ����鼴�ɵõ��ļ����ݡ�

```java
@ Test

public void readFromFileByMappedByteBuffer() {

    Path path = Paths. get(getClass.getResource(FILE_NAME).getPath);
    intlength = CONTENT.getBytes(Charset.forName(CHARSET)).length;

    try(FileChannel fileChannel = FileChannel.open(path, StandardOpenOption.READ)) {
            
        	MappedByteBuffer mappedByteBuffer = fileChannel.map(READ_ONLY, 0, length);
            if(mappedByteBuffer != null) {
            byte[] bytes = newbyte[length];
            mappedByteBuffer. get(bytes);
            String content = newString(bytes, StandardCharsets.UTF_8);
            assertEquals(content, "Zero copy implemented by MappedByteBuffer");
        }
    } catch(IOException e) {
    	e.printStackTrace;
    }
}
```



������� map �����ĵײ�ʵ��ԭ��map ������ java.nio.channels.FileChannel �ĳ��󷽷��������� sun.nio.ch.FileChannelImpl.java ʵ�֡�

�����Ǻ��ڴ�ӳ����صĺ��Ĵ��룺

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



map ����ͨ�����ط��� map0 Ϊ�ļ�����һ�������ڴ棬��Ϊ�����ڴ�ӳ������Ȼ�󷵻�����ڴ�ӳ���������ʼ��ַ��

- �ļ�ӳ����Ҫ�� Java ���д���һ�� MappedByteBuffer ��ʵ���������һ���ļ�ӳ�䵼�� OOM�����ֶ������������գ����� 100ms ���ٳ���ӳ�䣬���ʧ�����׳��쳣��
- ͨ�� Util �� newMappedByteBuffer���ɶ���д���������� newMappedByteBufferR���������������䴴��һ�� DirectByteBuffer ʵ�������� DirectByteBuffer �� MappedByteBuffer �����ࡣ

map �������ص����ڴ�ӳ���������ʼ��ַ��ͨ������ʼ��ַ+ƫ�������Ϳ��Ի�ȡָ���ڴ�����ݡ�

����һ���̶�������� read �� write �������ײ�ֱ�Ӳ��� sun.misc.Unsafe ��� getByte �� putByte ���������ݽ��ж�д��

```java
private native long map0(intprot, longposition, longmapSize)throwsIOException;
```



�����Ǳ��ط�����native method��map0 �Ķ��壬��ͨ�� JNI��Java Native Interface�����õײ� C ��ʵ�֡�

��� native ������Java_sun_nio_ch_FileChannelImpl_map0����ʵ��λ�� JDK Դ����µ� native/sun/nio/ch/FileChannelImpl.c ���Դ�ļ����档

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

���Կ��� map0 ����������ͨ�� mmap64 ��������� Linux �ײ��ں˷����ڴ�ӳ��ĵ��ã� mmap64 ������ԭ�����£�

```c++
#include<sys/mman.h>
void *mmap64(void*addr, size_tlen, intprot, intflags, intfd, off64_toffset);
```



������ϸ����һ�� mmap64 �������������ĺ����Լ�������ѡֵ��

**addr��**�ļ����û����̿ռ���ڴ�ӳ�����е���ʼ��ַ����һ������Ĳ�����ͨ��������Ϊ 0 �� NULL����ʱ���ں�ȥ������ʵ����ʼ��ַ��

�� flags Ϊ MAP_FIXED ʱ��addr ����һ����ѡ�Ĳ���������Ҫ�ṩһ�����ڵĵ�ַ��

**len��**�ļ���Ҫ�����ڴ�ӳ����ֽڳ��ȡ�

**prot��**�����û����̶��ڴ�ӳ�����ķ���Ȩ�ޣ�

- PROT_READ����Ȩ�ޡ�
- PROT_WRITE��дȨ�ޡ�
- PROT_EXEC��ִ��Ȩ�ޡ�
- PROT_NONE����Ȩ�ޡ�

**flags��**�����ڴ�ӳ�������޸��Ƿ񱻶�����̹���

- MAP_PRIVATE�����ڴ�ӳ�������ݵ��޸Ĳ��ᷴӳ���������ļ��������޸ķ���ʱ����дʱ���ƻ��ơ�
- MAP_SHARED�����ڴ�ӳ�������޸Ļ�ͬ�����������ļ����޸ĶԹ�����ڴ�ӳ�����Ľ����ǿɼ��ġ�
- MAP_FIXED��������ʹ�ã�����ģʽ�� addr ����ָ���ı����ṩһ�����ڵ� addr ������

**fd��**�ļ���������ÿ�� map �����ᵼ���ļ������ü����� 1��ÿ�� unmap �������߽������̻ᵼ�����ü����� 1��

**offset��**�ļ�ƫ����������ӳ����ļ�λ�ã����ļ���ʼ��ַ����λ������

�����ܽ�һ�� MappedByteBuffer ���ص�Ͳ���֮����

- MappedByteBuffer ʹ���Ƕ���������ڴ棬��˷��䣨map�����ڴ��С���� JVM �� -Xmx �������ƣ�����Ҳ���д�С���Ƶġ�
- ������ļ����� Integer.MAX_VALUE �ֽ�����ʱ������ͨ�� position �������� map �ļ���������ݡ�
- MappedByteBuffer �ڴ�����ļ�ʱ���ܵ�ȷ�ܸߣ���Ҳ�����ڴ�ռ�á��ļ��رղ�ȷ�������⣬����򿪵��ļ�ֻ�����������յĲŻᱻ�رգ��������ʱ����ǲ�ȷ���ġ�
- MappedByteBuffer �ṩ���ļ�ӳ���ڴ�� mmap ������Ҳ�ṩ���ͷ�ӳ���ڴ�� unmap ������Ȼ�� unmap �� FileChannelImpl �е�˽�з������޷�ֱ����ʾ���á� ��ˣ��û�������Ҫͨ�� Java ����ĵ��� sun.misc.Cleaner ��� clean �����ֶ��ͷ�ӳ��ռ�õ��ڴ�����

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



**DirectByteBuffer**

DirectByteBuffer �Ķ�������λ�� Java �ڴ�ģ�͵Ķ����棬JVM ���Զ� DirectByteBuffer �Ķ�������ڴ����ͻ��չ���

һ��ʹ�� DirectByteBuffer �ľ�̬���� allocateDirect ���� DirectByteBuffer ʵ���������ڴ档

```java
public static ByteBuffer allocateDirect(intcapacity){
	returnnewDirectByteBuffer(capacity);
}
```



DirectByteBuffer �ڲ����ֽڻ�����λ���ڶ���ģ��û�̬��ֱ���ڴ棬����ͨ�� Unsafe �ı��ط��� allocateMemory �����ڴ���䣬�ײ���õ��ǲ���ϵͳ�� malloc ������

```java
DirectByteBuffer( intcap) {

    super( -1, 0, cap, cap);

    boolean pa = VM.isDirectMemoryPageAligned;

    intps = Bits.pageSize;

    longsize = Math.max( 1L, ( long)cap + (pa ? ps : 0));

    Bits.reserveMemory(size, cap);

    longbase= 0;

    try{

        base= unsafe.allocateMemory(size);

    } catch(OutOfMemoryError x) {

        Bits.unreserveMemory(size, cap);

        throwx;

    }

    unsafe.setMemory( base, size, ( byte) 0);

    if(pa && ( base% ps != 0)) {

        address = base+ ps - ( base& (ps - 1));

    } else{

        address = base;

    }

    cleaner = Cleaner.create( this, newDeallocator( base, size, cap));

    att = null;

}
```



����֮�⣬��ʼ�� DirectByteBuffer ʱ���ᴴ��һ�� Deallocator �̣߳���ͨ�� Cleaner �� freeMemory ��������ֱ���ڴ���л��ղ�����freeMemory �ײ���õ��ǲ���ϵͳ�� free ������

```java
private static class DeallocatorimplementsRunnable{

    privatestaticUnsafe unsafe = Unsafe.getUnsafe;

    privatelongaddress;

    privatelongsize;

    privateintcapacity;

    privateDeallocator(longaddress, longsize, intcapacity){

        assert(address != 0);

        this.address = address;

        this.size = size;

        this.capacity = capacity;

    }

    publicvoidrun{

        if(address == 0) {

            return;

        }

        unsafe.freeMemory(address);

        address = 0;

        Bits.unreserveMemory(size, capacity);

    }

}
```



����ʹ�� DirectByteBuffer �������ϵͳ���ص��ڴ棬���� JVM �Ĺܿط�Χ֮�ڣ����ֱ���ڴ�Ļ��պͶ��ڴ�Ļ��ղ�ͬ��ֱ���ڴ����ʹ�ò�������������� OutOfMemoryError��

˵����ô�࣬��ô DirectByteBuffer ���㿽����ʲô��ϵ��ǰ�����ᵽ�� MappedByteBuffer �����ڴ�ӳ��ʱ������ map ������ͨ�� Util.newMappedByteBuffer ������һ��������ʵ����

��ʼ���Ĵ������£�

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



DirectByteBuffer �� MappedByteBuffer �ľ���ʵ���ࡣ

ʵ���ϣ�Util.newMappedByteBuffer ����ͨ��������ƻ�ȡ DirectByteBuffer �Ĺ�������Ȼ�󴴽�һ�� DirectByteBuffer ��ʵ������Ӧ����һ�����������ڴ�ӳ��Ĺ��췽����

```java
protected DirectByteBuffer(intcap, longaddr, FileDeor fd, Runnable unmapper){

    super(- 1, 0, cap, cap, fd);

    address = addr;

    cleaner = Cleaner.create( this, unmapper);

    att = null;

}
```



��ˣ���������������ϵͳ��ֱ���ڴ����⣬DirectByteBuffer ����Ҳ�����ļ��ڴ�ӳ��Ĺ��ܣ����ﲻ������˵����

������Ҫ��ע���ǣ�DirectByteBuffer �� MappedByteBuffer �Ļ������ṩ���ڴ�ӳ���ļ��������ȡ get ��д�� write �Ĳ�����

�ڴ�ӳ���ļ��������������

```java
public byte get() {

    return(( unsafe.getByte(ix(nextGetIndex))));

}

public byte get(inti) {

    return(( unsafe.getByte(ix(checkIndex(i)))));

}
```



�ڴ�ӳ���ļ������д������

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



�ڴ�ӳ���ļ��������д���ǽ��� ix ����ʵ�ֶ�λ�ģ� ix ����ͨ���ڴ�ӳ��ռ���ڴ��׵�ַ��address���͸���ƫ���� i �����ָ���ַ��Ȼ���� unsafe ��� get �� put �����Ͷ�ָ��ָ������ݽ��ж�ȡ��д�롣

```java
private longix(inti){

	return address + (( long)i << 0);

}
```



**FileChannel**

FileChannel ��һ�������ļ���д��ӳ��Ͳ�����ͨ����ͬʱ���ڲ������������̰߳�ȫ�ġ�

���� FileInputStream��FileOutputStream ���� RandomAccessFile �� getChannel �������Դ�������һ���ļ�ͨ����

FileChannel ������ transferFrom �� transferTo �������󷽷�����ͨ����ͨ����ͨ��֮�佨������ʵ�����ݴ���ġ�

**transferTo��**ͨ�� FileChannel ���ļ������Դ����д��һ�� WritableByteChannel ��Ŀ��ͨ����

publicabstractlongtransferTo(longposition, longcount, WritableByteChannel target)

throwsIOException;

**transferFrom��**��һ��Դͨ�� ReadableByteChannel �е����ݶ�ȡ����ǰ FileChannel ���ļ����档

publicabstractlongtransferFrom(ReadableByteChannel src, longposition, longcount)

throwsIOException;

������� FileChannel ���� transferTo �� transferFrom �����������ݴ����ʹ��ʾ����

```java
private static final String CONTENT = "Zero copy implemented by FileChannel";

private static final String SOURCE_FILE = "/source.txt";

private static final String TARGET_FILE = "/target.txt";

private static final String CHARSET = "UTF-8";
```



����������ظ�·���´��� source.txt �� target.txt �����ļ�����Դ�ļ� source.txt �ļ�д���ʼ�����ݡ�

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



���� transferTo �������ԣ�Ŀ��ͨ�� toChannel ����������ĵ����ֽ�дͨ�� WritableByteChannel�������� transferFrom �������ԣ�Դͨ�� fromChannel ����������ĵ����ֽڶ�ͨ�� ReadableByteChannel��

���У�FileChannel��SocketChannel �� DatagramChannel ��ͨ��ʵ���� WritableByteChannel �� ReadableByteChannel �ӿڣ�����ͬʱ֧�ֶ�д��˫��ͨ����

Ϊ�˷�����ԣ������������ FileChannel ��� channel-to-channel �����ݴ���ʾ����

ͨ�� transferTo �� fromChannel �е����ݿ����� toChannel��

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



ͨ�� transferFrom �� fromChannel �е����ݿ����� toChannel��

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



������� transferTo �� transferFrom �����ĵײ�ʵ��ԭ������������Ҳ�� java.nio.channels.FileChannel �ĳ��󷽷��������� sun.nio.ch.FileChannelImpl.java ʵ�֡�

transferTo �� transferFrom �ײ㶼�ǻ��� Sendfile ʵ�����ݴ���ģ����� FileChannelImpl.java ������ 3 �����������ڱ�ʾ��ǰ����ϵͳ���ں��Ƿ�֧�� Sendfile �Լ� Sendfile ��������ԡ�

```java
private static volatile boolean transferSupported = true;

private static volatile boolean pipeSupported = true;

private static volatile boolean fileSupported = true;
```



**transferSupported��**���ڱ�ǵ�ǰ��ϵͳ�ں��Ƿ�֧�� sendfile ���ã�Ĭ��Ϊ true��

**pipeSupported��**���ڱ�ǵ�ǰ��ϵͳ�ں��Ƿ�֧���ļ���������fd�����ڹܵ���pipe���� sendfile ���ã�Ĭ��Ϊ true��

**fileSupported��**���ڱ�ǵ�ǰ��ϵͳ�ں��Ƿ�֧���ļ���������fd�������ļ���file���� sendfile ���ã�Ĭ��Ϊ true��

������ transferTo ��Դ��ʵ��Ϊ����FileChannelImpl ����ִ�� transferToDirectly �������� Sendfile ���㿽����ʽ�������ݿ�����

���ϵͳ�ں˲�֧�� Sendfile����һ��ִ�� transferToTrustedChannel �������� mmap ���㿽����ʽ�����ڴ�ӳ�䣬���������Ŀ��ͨ�������� FileChannelImpl ���� SelChImpl ���͡�

�������������ʧ���ˣ���ִ�� transferToArbitraryChannel ���������ڴ�ͳ�� I/O ��ʽ��ɶ�д�����岽���ǳ�ʼ��һ����ʱ�� DirectBuffer����Դͨ�� FileChannel �����ݶ�ȡ�� DirectBuffer����д��Ŀ��ͨ�� WritableByteChannel ���档

```java
public long transferTo(longposition, longcount, WritableByteChannel target)

    throwsIOException {

    // �����ļ��Ĵ�С

    longsz = size;

    // У����ʼλ��

    if(position > sz)

        return0;

    inticount = ( int)Math.min(count, Integer.MAX_VALUE);

    // У��ƫ����

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



�������ص����һ�� transferToDirectly ������ʵ�֣�Ҳ���� transferTo ͨ�� Sendfile ʵ���㿽���ľ������ڡ�

���Կ�����transferToDirectlyInternal �����Ȼ�ȡ��Ŀ��ͨ�� WritableByteChannel ���ļ������� targetFD����ȡͬ����Ȼ��ִ�� transferToDirectlyInternal ������

```java
private long transferToDirectly(longposition, inticount, WritableByteChannel target)

    throwsIOException {

    // ʡ�Դ�target��ȡtargetFD�Ĺ���

    if(nd.transferToDirectlyNeedsPositionLock) {

        synchronized(positionLock) {

            longpos = position;

            try{

                returntransferToDirectlyInternal(position, icount,

                                                 target, targetFD);

            } finally{

                position(pos);

            }

        }

    } else{

        returntransferToDirectlyInternal(position, icount, target, targetFD);

    }

}
```



������ transferToDirectlyInternal ���ñ��ط��� transferTo0 �������� Sendfile �ķ�ʽ�������ݴ��䡣

���ϵͳ�ں���ȫ��֧�� Sendfile������ Windows ����ϵͳ���򷵻� UNSUPPORTED ���� transferSupported ��ʶΪ false��

���ϵͳ�ں˲�֧�� Sendfile ��һЩ���ԣ�����˵�Ͱ汾�� Linux �ں˲�֧�� DMA gather copy �������򷵻� UNSUPPORTED_CASE ���� pipeSupported ���� fileSupported ��ʶΪ false��

```java
private long transferToDirectlyInternal(longposition, inticount,

                                        WritableByteChannel target,

                                        FileDeor targetFD)throwsIOException {

    assert!nd.transferToDirectlyNeedsPositionLock ||

        Thread.holdsLock(positionLock);

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



���ط�����native method��transferTo0 ͨ�� JNI��Java Native Interface�����õײ� C �ĺ�����

��� native ������Java_sun_nio_ch_FileChannelImpl_transferTo0��ͬ��λ�� JDK Դ����µ� native/sun/nio/ch/FileChannelImpl.c Դ�ļ����档

JNI ���� Java_sun_nio_ch_FileChannelImpl_transferTo0 ������������Բ�ͬ��ϵͳ����Ԥ���룬������ JDK ���� Linux ϵͳ�ں˶� transferTo �ṩ�ĵ��÷�װ��

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



�� Linux��Solaris �Լ� Apple ϵͳ���ԣ�transferTo0 �����ײ��ִ�� sendfile64 ���ϵͳ��������㿽��������sendfile64 ������ԭ�����£�

\#include<sys/sendfile.h>

ssize_tsendfile64( intout_fd, intin_fd, off_t*offset, size_tcount);

����򵥽���һ�� sendfile64 �������������ĺ��壺

- out_fd����д����ļ���������
- in_fd������ȡ���ļ���������
- offset��ָ�� in_fd ��Ӧ�ļ����Ķ�ȡλ�ã����Ϊ�գ���Ĭ�ϴ���ʼλ�ÿ�ʼ��
- count��ָ�����ļ������� in_fd �� out_fd ֮�䴫����ֽ�����

�� Linux 2.6.3 ֮ǰ��out_fd ������һ�� socket������ Linux 2.6.3 �Ժ�out_fd �������κ��ļ���

Ҳ����˵��sendfile64 �����������Խ��������ļ����䣬�����ԶԱ����ļ�ʵ���㿽��������

�������㿽��ʵ��



# Netty �㿽��

Netty �е��㿽���������ᵽ�Ĳ���ϵͳ�����ϵ��㿽����̫һ��, ������˵�� Netty �㿽����ȫ�ǻ��ڣ�Java ���棩�û�̬�ģ����ĸ������ƫ�������ݲ����Ż������ĸ��

������������¼������棺

- Netty ͨ�� DefaultFileRegion ��� java.nio.channels.FileChannel �� tranferTo �������а�װ�����ļ�����ʱ���Խ��ļ�������������ֱ�ӷ��͵�Ŀ��ͨ����Channel����
- ByteBuf ����ͨ�� wrap �������ֽ����顢ByteBuf��ByteBuffer ��װ��һ�� ByteBuf ����, ���������˿���������
- ByteBuf ֧�� Slice ����, ��˿��Խ� ByteBuf �ֽ�Ϊ�������ͬһ���洢����� ByteBuf���������ڴ�Ŀ�����
- Netty �ṩ�� CompositeByteBuf �࣬�����Խ���� ByteBuf �ϲ�Ϊһ���߼��ϵ� ByteBuf�������˸��� ByteBuf ֮��Ŀ�����

���е� 1 �����ڲ���ϵͳ������㿽������������ 3 ��ֻ�����û���������ݲ����Ż���

**RocketMQ �� Kafka �Ա�**

RocketMQ ѡ���� mmap+write �����㿽����ʽ��������ҵ����Ϣ����С���ļ������ݳ־û��ʹ��䡣

�� Kafka ���õ��� Sendfile �����㿽����ʽ��������ϵͳ��־��Ϣ���ָ��������Ĵ���ļ������ݳ־û��ʹ��䡣

����ֵ��ע���һ���ǣ�Kafka �������ļ�ʹ�õ��� mmap+write ��ʽ�������ļ�ʹ�õ��� Sendfile ��ʽ��

![](img/kafka-roketmq.png)



# �ܽ�

���Ŀ�ƪ������ Linux ����ϵͳ�е������ڴ�������ڴ棬�ں˿ռ���û��ռ�ĸ����Լ� Linux �ڲ��Ĳ㼶�ṹ��

�ڴ˻����ϣ���һ�������ͶԱȴ�ͳ I/O ��ʽ���㿽����ʽ������Ȼ������� Linux �ں��ṩ�ļ����㿽��ʵ�֡�

�����ڴ�ӳ�� mmap��Sendfile��Sendfile+DMA gather copy �Լ� Splice ���ֻ��ƣ�����ϵͳ���úͿ���������������ǽ����˶Աȡ�

��������Դ�����ַ����� Java NIO ���㿽����ʵ�֣���Ҫ���������ڴ�ӳ�䣨mmap����ʽ�� MappedByteBuffer �Լ����� Sendfile ��ʽ�� FileChannel��

�����ƪĩ�򵥵Ĳ�����һ�� Netty �е��㿽�����ƣ��Լ� RocketMQ �� Kafka ������Ϣ�������㿽��ʵ�ַ�ʽ�ϵ�����