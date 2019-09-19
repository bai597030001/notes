#include<iostream>
#include<arpa/inet.h>
#include<cstdio>
#include<cstring>
using namespace std;
#define TransBigLittle16(X)	((((unsigned short)(X) & 0xff00) >> 8) |(((unsigned short)(X) & 0x00ff) << 8))
#define TransBigLittle32(X) ((((unsigned int)(X) & 0xff000000) >> 8) | \
		                    (((unsigned int)(X) & 0x00ff0000) >> 8) | \
		                    (((unsigned int)(X) & 0x0000ff00) >> 8) | \
							(((unsigned int)(X) & 0x000000ff) << 24))

int main()
{
	unsigned char carr[4] = {0};
	carr[0] = 0x00;
	carr[1] = 0x04;
	carr[2] = 0x64;
	carr[3] = 0xE4;
	for(int i = 0; i < 4; i++)
	{
		printf("0x%X ", carr[i]);
	}
	printf("\n");

	unsigned int ln = 0;
	memcpy(&ln, &carr[0], sizeof(ln));
	printf("ln = %u\n", ln);

	unsigned int a = TransBigLittle32(ln);
	printf("%u\n", a);

	return 0;
}
