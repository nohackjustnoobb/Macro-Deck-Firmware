#pragma once
#include <Arduino.h>
#include <SD.h>

static bool mkdirRecursive(const String &path) {
  if (!path.startsWith("/"))
    return false;

  String subPath = "";
  int lastSlash = 0;

  while (true) {
    int nextSlash = path.indexOf('/', lastSlash + 1);
    subPath = nextSlash == -1 ? path : path.substring(0, nextSlash);

    if (!SD.exists(subPath) && !SD.mkdir(subPath))
      return false;

    if (nextSlash == -1)
      break;
    lastSlash = nextSlash;
  }

  return true;
}

static bool removeDirectoryRecursive(const String &path) {
  File dir = SD.open(path);
  if (!dir)
    return false;

  File entry = dir.openNextFile();
  while (entry) {
    String entryPath = entry.path();
    if (entry.isDirectory()) {
      if (!removeDirectoryRecursive(entryPath))
        return false;
    } else {
      SD.remove(entryPath);
    }

    entry = dir.openNextFile();
  }

  dir.close();
  return SD.rmdir(path);
}