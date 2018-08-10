#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

void keypress(const char *prompt)
{
	char buf[80];
	write(1, prompt, strlen(prompt));
	write(1, "> ", 2);
	read(0, buf, 80);
}

int main(void)
{
	int fd = open("mmaptest", O_RDWR|O_CREAT|O_TRUNC, 0600);
	keypress("Created");
	char *mapped = (char *)mmap(NULL, 0x4000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	keypress("Mapped");
	lseek(fd, 0x3FF, SEEK_SET);
	write(fd, "", 1);
	keypress("Extended to 1K");
	int i;
	for (i = 0; i < 0x400; ++i) {
		mapped[i] = (i+1) % 128 ? 'x' : '\n';
	}
	keypress("1K written");
	lseek(fd, 0x1FFF, SEEK_SET);
	write(fd, "", 1);
	keypress("Extended to 8K");
	for (i = 0x400; i < 0x2000; ++i) {
		mapped[i] = (i+1) % 128 ? 'x' : '\n';
	}
	keypress("8K written");
	unlink("mmaptest");
	keypress("Deleted");
	return 0;
}
