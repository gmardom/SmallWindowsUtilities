#pragma comment(lib, "user32")
#pragma comment(lib, "shlwapi")

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shlwapi.h>

#include <stdio.h>
#include <string.h>

BOOL FileExists(LPCSTR path)
{
    DWORD attrib = GetFileAttributes(path);
    return (
        attrib != INVALID_FILE_ATTRIBUTES &&
        !(attrib & FILE_ATTRIBUTE_DIRECTORY)
    );
}

BOOL FileIsImage(LPCSTR path)
{
    LPCSTR ext = PathFindExtensionA(path);

    if (strcmp(ext, ".jpg"))  return TRUE;
    if (strcmp(ext, ".jpeg")) return TRUE;
    if (strcmp(ext, ".png"))  return TRUE;
    if (strcmp(ext, ".bmp"))  return TRUE;

    return FALSE;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    char file_path[MAX_PATH];
    strncpy(file_path, argv[1], MAX_PATH);

    if (!FileExists(file_path)) {
        fprintf(stderr, "ERROR: Invalid file: '%s'\n", file_path);
        return 1;
    }

    if (!FileIsImage(file_path)) {
        fprintf(stderr, "ERROR: File is not an image: '%s'\n", file_path);
        return 1;
    }

    BOOL result = SystemParametersInfoA(
        SPI_SETDESKWALLPAPER,
        0, file_path,
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE
    );

    if (result == 0) {
        fprintf(stderr, "ERROR: Could not set wallpaper.\n");
        return 1;
    }

    return 0;
}

// TODO: Use wchar_t for paths.
// TODO: Check file type by it's header, not extension.
