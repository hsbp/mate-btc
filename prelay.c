#include <stdio.h>
#include <string.h>
#include <sys/io.h>

#define PORT 0x378

int main(int argc, char** argv)
{
	char buf, i;

	if (ioperm(PORT,1,1))
	{
		printf("Cannot open LPT port\n");
		return 1;
	}

	buf=inb(PORT);

	if (argc == 3)
	{
		if (strlen(argv[2]) == 1)
		{
			if (!strcmp("on", argv[1]))
			{
				switch (argv[2][0])
				{
					case '4': buf |= 1; break;
					case '3': buf |= 2; break;
					case '2': buf |= 4; break;
					case '1': buf |= 8; break;
				}
			}
			else if (!strcmp("off", argv[1]))
			{
				switch (argv[2][0])
				{
					case '4': buf &= 254; break;
					case '3': buf &= 253; break;
					case '2': buf &= 251; break;
					case '1': buf &= 247; break;
				}
			}
		}
	}
	else if (argc == 2 && !strcmp("reset", argv[1]))
		buf=0;

	// write to port
	outb(buf, PORT);

	for (i = 1; i != 5; i++)
		printf("relay%d: %s\n", i, (buf & (16 >> i)) == (16 >> i) ? "on" : "off");

	return 0;
}
