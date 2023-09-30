#pragma once
#include <Arduino.h>
#include <sys/log.h>

#include <map>

#include "../kernel/kernel.h"
#include "../kernel/sysmacros.h"
#include "shell/file_system_commands.h"
#include "shell/global_var.h"
#include "shell/restart.h"

struct Command {
  TaskFunction_t task;
  bool hasArg;
};
using std::string;
string out;
char buffer[256];
size_t read;

std::map<string, Command> commands{
    {"cd", {cdCallback, true}},          {"ls", {lsCallback, false}},
    {"mkdir", {mkdirCallback, true}},    {"rmdir", {rmdirCallback, true}},
    {"restart", {restartCallback, false}}, {"pwd", {pwdCallback, false}}
};

void shell(void *) {
  while (true) {
    read = Serial.readBytesUntil('\n', buffer, 256);

    if (read != 0) {
      out.assign(buffer, read);
      Serial.printf("%s : %s\n", path_s().c_str(), out.c_str());
      auto texts = tokenize_str(out, ' ');
      string cmd = texts.at(0);

      if (commands.find(cmd) == commands.end()) {
        INFO("Command not found");
        continue;
      }
      auto arg =
          commands[cmd].hasArg ? static_cast<void *>(&texts.at(1)) : NULL;
      processManager.start_process(cmd, commands[cmd].task, arg);
    }
    taskYIELD();
  }
}