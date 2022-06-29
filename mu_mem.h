#ifndef H_MU_MEM
#define H_MU_MEM

#include <stdio.h>
#include <sys/types.h>
#include <sys/user.h>

#define MIN(a, b) ((a) > (b)) ? (b) : (a)

#define CL_SIZE 64
#define CL_SHIFT 6

typedef u_int64_t u64;
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t  u8;

static void hex_dump(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' '
		    && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

static inline void
flush_processor_cache(const void *addr, size_t len)
{
	int i;
	const char *buffer = (const char *)addr;

	__sync_synchronize(); /* Full barrier */

	/* Flush the processor cache for the target range */
	for (i=0; i<len; i+=CL_SIZE) {
		__builtin_ia32_clflush(&buffer[i]);
	}

	__sync_synchronize(); /* Full barrier */
}


#define ASSERT(X)							\
	do {								\
		if (!(X)) {						\
			fprintf(stderr, "Assertion " #X " failed\n");	\
			exit(-1);					\
		}							\
	} while (0)



static inline int page_aligned(u_int64_t a)
{
	if (a % PAGE_SIZE)
		return 0;

	return 1;
}

#endif
