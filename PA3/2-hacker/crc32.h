#ifndef CRC32_H
#define CRC32_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned z_size_t;
typedef unsigned z_crc_t;

z_crc_t crc32(z_crc_t crc32, const unsigned char *buf, z_size_t len);

#ifdef __cplusplus
}
#endif

#endif // CRC32_H
