typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64;
 
// �����ʹ�С�˻���
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
                            (((uint16)(A) & 0x00ff) << 8))
 // �����ʹ�С�˻���
/*
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                            (((uint32)(A) & 0x00ff0000) >> 8) | \
                            (((uint32)(A) & 0x0000ff00) << 8) | \
                            (((uint32)(A) & 0x000000ff) << 24))
*/							
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 8) | \
                            (((uint32)(A) & 0x00ff0000) >> 8) | \
                            (((uint32)(A) & 0x0000ff00) >> 8) | \
                            (((uint32)(A) & 0x000000ff) << 24))

#define BigLittleSwap64(A)	(((uint64)(A) & 0xff00000000000000) >> 56) | \
                            (((uint64)(A) & 0x00ff000000000000) >> 40) | \
							(((uint64)(A) & 0x0000ff0000000000) >> 24) | \
							(((uint64)(A) & 0x000000ff00000000) >> 8) | \
							(((uint64)(A) & 0x00000000ff000000) << 8) | \
							(((uint64)(A) & 0x0000000000ff0000) << 24) | \
							(((uint64)(A) & 0x000000000000ff00) << 40) | \
                            (((uint64)(A) & 0x00000000000000ff) << 56))
 // ������˷���1��С�˷���0
int checkCPUendian()
{
       union{
              unsigned int i;
              unsigned char s[4];
       }c;
 
       c.i = 0x12345678;
       return (0x12 == c.s[0]);
}
 
// ģ��htonl�����������ֽ���ת�����ֽ���
unsigned int t_htonl(unsigned int h)
{
       // ������Ϊ��ˣ��������ֽ���ͬ��ֱ�ӷ���
       // ������ΪС�ˣ�ת���ɴ���ٷ���
       return checkCPUendian() ? h : BigLittleSwap32(h);
}
 
// ģ��ntohl�����������ֽ���ת�����ֽ���
unsigned int t_ntohl(unsigned int n)
{
       // ������Ϊ��ˣ��������ֽ���ͬ��ֱ�ӷ���
       // ������ΪС�ˣ���������ת����С���ٷ���
       return checkCPUendian() ? n : BigLittleSwap32(n);
}
 
// ģ��htons�����������ֽ���ת�����ֽ���
unsigned short int t_htons(unsigned short int h)
{
       // ������Ϊ��ˣ��������ֽ���ͬ��ֱ�ӷ���
       // ������ΪС�ˣ�ת���ɴ���ٷ���
       return checkCPUendian() ? h : BigLittleSwap16(h);
}
 
// ģ��ntohs�����������ֽ���ת�����ֽ���
unsigned short int t_ntohs(unsigned short int n)
{
       // ������Ϊ��ˣ��������ֽ���ͬ��ֱ�ӷ���
       // ������ΪС�ˣ���������ת����С���ٷ���
       return checkCPUendian() ? n : BigLittleSwap16(n);
}
