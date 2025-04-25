#include <TJpg_Decoder.h>

#include "../models/common.h"
#include "../models/constants.h"
#include "../models/handler.h"
#include "../utils.hpp"

class ButtonManager : public Handler {
public:
  ButtonManager() { draw(); }

  bool is(String &type) {
    return type == "wi" || type == "ri" || type == "di" || type == "df" ||
           type == "sp" || type == "ld" || type == "cf";
  }

  void handleWI(Message &mesg) {
    std::vector<String> datas = mesg.parse();
    String path = datas[0];
    int size = datas[1].toInt();

    if (size <= 0) {
      Serial.println(NOT_OK);
      return;
    }

    Serial.println(RD);

    char buf[size];
    Serial.readBytes(buf, size);

    write(path, buf, size);
  }

  void handleRI(Message &mesg) {
    File file = SD.open(mesg.data, FILE_READ);

    if (!file) {
      Serial.println(NOT_OK);
      return;
    }

    Serial.println(Message(String("rd?"), String(file.size())).encode());

    while (Serial.available() <= 0) {
    }

    String reply = Serial.readString();
    if (reply != RD) {
      Serial.println(NOT_OK);
      return;
    }

    while (file.available())
      Serial.write(file.read());
  }

  void handleDI(Message &mesg) {
    Serial.println(SD.remove(mesg.data) ? OK : NOT_OK);

    draw();
  }

  void handleDF(Message &mesg) {
    bool result = removeDirectoryRecursive(mesg.data);
    Serial.println(result ? OK : NOT_OK);

    if (result)
      draw();
  }

  void handleSP(Message &mesg) {
    String tempPath = "/" + mesg.data;
    if (!SD.exists(tempPath)) {
      Serial.println(NOT_OK);
      return;
    }

    path = tempPath;
    draw();

    Serial.println(OK);
  }

  void listDirectory(File dir, std::vector<String> &paths) {
    while (true) {
      File entry = dir.openNextFile();
      if (!entry)
        break;

      // don't list hidden files
      if (entry.name()[0] == '.')
        continue;

      if (entry.isDirectory())
        listDirectory(entry, paths);

      paths.push_back(entry.path());
      entry.close();
    }
  }

  void handleLD(Message &mesg) {
    std::vector<String> paths = {};

    File root = SD.open("/");
    if (root) {
      listDirectory(root, paths);
      Serial.println(Message("ld", paths).encode());
    } else {
      Serial.println(NOT_OK);
    }
  }

  void handleCF(Message &mesg) {
    Serial.println(mkdirRecursive(mesg.data) ? OK : NOT_OK);
  }

  bool handle(Message &mesg) {
    if (mesg.type == "wi")
      handleWI(mesg);

    if (mesg.type == "ri")
      handleRI(mesg);

    if (mesg.type == "di")
      handleDI(mesg);

    if (mesg.type == "df")
      handleDF(mesg);

    if (mesg.type == "sp")
      handleSP(mesg);

    if (mesg.type == "ld")
      handleLD(mesg);

    if (mesg.type == "cf")
      handleCF(mesg);

    return false;
  }

  bool is(uint16_t &x, uint16_t &y) { return x <= width && y <= buttonsHeight; }

  bool handle(uint16_t &x, uint16_t &y) {
    int16_t row = y / (buttonsHeight / NUM_OF_ROWS);
    int16_t col = x / (width / BUTTONS_PER_ROW);
    int idx = row * BUTTONS_PER_ROW + col;

    if (std::find(folders.begin(), folders.end(), idx) != folders.end()) {
      path += "/" + String(idx);
      draw();
    } else {
      if (idx == 0) {
        int lastIdx = path.lastIndexOf('/');
        if (lastIdx != 0 && lastIdx != -1) {
          path = path.substring(0, lastIdx);
          draw();
          return false;
        }
      }

      Serial.println(Message("bc", path + "/" + String(idx)).encode());
    }

    return false;
  }

  void draw() {
    if (!SD.exists(path))
      path = "/default";

    File file = SD.open(path);
    if (!file)
      return;

    tft.fillRect(0, 0, width, buttonsHeight, TFT_BLACK);

    if (SD.exists(path + "/aio.jpg"))
      return draw_aio(path);

    folders.clear();
    File entry;
    while (entry = file.openNextFile()) {
      String fileName = entry.name();
      int idx = fileName.lastIndexOf('.');

      fileName = fileName.substring(0, idx);
      idx = fileName.toInt();

      if (idx != -1)
        draw_idx(path, idx);

      if (entry.isDirectory())
        folders.push_back(idx);
    }
  }

  void draw_idx(String base_dir, int16_t idx) {
    int16_t row = idx / BUTTONS_PER_ROW;
    int16_t col = idx % BUTTONS_PER_ROW;
    int16_t x = col * (width / BUTTONS_PER_ROW + GAP_SIZE / BUTTONS_PER_ROW);
    int16_t y = row * (buttonsHeight / NUM_OF_ROWS + GAP_SIZE / NUM_OF_ROWS);

    TJpgDec.drawSdJpg(x, y, (base_dir + "/" + String(idx) + ".jpg").c_str());
  }

  void draw_aio(String base_dir) {
    TJpgDec.drawSdJpg(0, 0, (base_dir + "/aio.jpg").c_str());
  }

  void write(String path, const char *buf, int size) {
    int lastSlash = path.lastIndexOf('/');
    if (lastSlash != -1) {
      String dirPath = path.substring(0, lastSlash);
      if (!mkdirRecursive(dirPath)) {
        Serial.println(NOT_OK);
        return;
      }
    }

    File file = SD.open(path, FILE_WRITE, true);
    if (!file) {
      Serial.println(NOT_OK);
      return;
    }

    if (file.write((const uint8_t *)buf, size))
      Serial.println(OK);
    else
      Serial.println(NOT_OK);

    file.close();

    draw();
  }

private:
  String path = "/default";
  std::vector<int> folders = {};
};