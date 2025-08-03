/* TODO: Windows */

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct MemMap MemMap;
struct MemMap
{
	Str s;

	bool (*unmap)(MemMap *);
};

static bool
unmap_posix(MemMap *m)
{
	return munmap(m->s.b, m->s.len) == 0;
}

static bool
memmap_readonly(char *path, MemMap *m)
{
	int fd = open(path, O_RDONLY);
	if (fd < 0)
		return false;

	struct stat st;
	if (fstat(fd, &st) != 0)
		return false;
	i64 len = st.st_size;

	m->s.b = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
	m->s.len = len;
	m->unmap = unmap_posix;

	if (m->s.b == MAP_FAILED) {
		if (close(fd)) {
			/* TODO */
		}
		return false;
	}

	if (close(fd)) {
		/* TODO */
		if (!m->unmap(m)) {
			/* TODO*/
		}
		return false;
	}

	return true;
}

