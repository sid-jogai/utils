/* TODO: Windows */

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct MemMap MemMap;
struct MemMap
{
	u8	*b;
	i64	 len;

	bool (*unmap)(MemMap *);
};

static bool
unmap_posix(MemMap *m)
{
	return munmap(m->b, m->len) == 0;
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

	*m = (MemMap) {
		.b = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0),
		.len = len,
		.unmap = unmap_posix,
	};

	if (m->b == MAP_FAILED) {
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

