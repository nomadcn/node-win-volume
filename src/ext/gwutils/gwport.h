#ifndef GW_PORT_H
#define GW_PORT_H

#if defined(WIN32)

#define SZ_PATH_DELIMITER "\\"
#define CH_PATH_DELIMITER '\\'

#define gw_close(x) closesocket(x)
#define gw_strcasecmp(s1, s2) _stricmp(s1, s2)
#define gw_max(s1, s2) max(s1, s2)
#define gw_min min
#define gw_strtok_r(s1, s2, s3) strtok_s(s1, s2, s3)
#define gw_fseeko(s1, s2, s3) _fseeki64(s1, s2, s3)
#define gw_snprintf(buf, sizeOfBuffer, ...) \
	_snprintf_s(buf, sizeOfBuffer, _TRUNCATE, __VA_ARGS__)
#define gw_ioctl ioctlsocket
#define gw_ftello _ftelli64
#define gw_stat(s1, s2) _stat(s1, s2)
#define gw_unlink(s1) _unlink(s1)
#define gw_atoll(s1) _atoi64(s1)
#define gw_mkdir(dir, mode) _mkdir(dir)
#define gw_tmkdir(dir, mode) _tmkdir(dir)

#else

#define SZ_PATH_DELIMITER "/"
#define CH_PATH_DELIMITER '/'

#define gw_close(x) close(x)
#define gw_strcasecmp(s1, s2) strcasecmp(s1, s2)
#define gw_max(s1, s2) std::max(s1, s2)
#define gw_min(s1, s2) std::min(s1, s2)
#define gw_strtok_r(s1, s2, s3) strtok_r(s1, s2, s3)
#define gw_fseeko(s1, s2, s3) fseeko(s1, s2, s3)
#define gw_snprintf snprintf
#define gw_ioctl ioctl
#define gw_ftello ftello
#define gw_stat(s1, s2) stat(s1, s2)
#define gw_unlink(s1) unlink(s1)
#define gw_atoll(s1) atoll(s1)
#define gw_mkdir(dir, mode) mkdir(dir, mode)
#define gw_tmkdir(dir, mode) mkdir(dir, mode)

#define _tfopen fopen
#define _tunlink unlink
#define _tstat stat
#define _ttoi atoi
#define _stprintf sprintf
#define _tcsncpy strncpy
#define _tcscpy strcpy
#define _tcsicmp strcasecmp
#define _taccess access
#define _stprintf_s snprintf

#endif

#endif
