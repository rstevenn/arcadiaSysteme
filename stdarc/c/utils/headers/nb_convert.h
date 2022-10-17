#ifndef __NB_CONVERT__
#define __NB_CONVERT__

#define big_endian(buf) ((unsigned char)buf[0] << 24) | \
                        ((unsigned char)buf[1] << 16) | \
                        ((unsigned char)buf[2] << 8)  | \
                        (unsigned char)buf[3]


#endif