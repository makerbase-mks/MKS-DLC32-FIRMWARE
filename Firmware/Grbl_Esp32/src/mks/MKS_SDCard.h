
#ifndef __mks_sdcard_h
#define __mks_sdcard_h

#include "MKS_LVGL.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define FILE_SD_BUF			128

class SdCard
{
private:
	char buf[FILE_SD_BUF];

public:
	void init();

	void listDir(  const char* dirname, uint8_t levels);

	void createDir( const char* path);

	void removeDir(  const char* path);

	void readFile(  const char* path);

	String readFileLine( const char* path, int num);

	int Serch_data(const char* path, const char *str);

	bool sd_data_update(const char* path, const char *str);

	void writeFile(  const char* path, const char* message);

	void appendFile(  const char* path, const char* message);

	void renameFile(  const char* path1, const char* path2);

	void deleteFile(  const char* path);

	void readBinFromSd(const char* path, uint8_t* buf);

	void writeBinToSd(const char* path, uint8_t* buf);

	void fileIO(const char* path);

	bool file_check(const char* path);

	void writeFile_line(const char* path, const char* message, uint32_t line_num);
};

extern SdCard tf;

#endif
