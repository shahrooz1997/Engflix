//
// Created by shahrooz on 10/29/20.
//

#ifndef ENGLISH_TEACHER_VLC_INTERFACE_H
#define ENGLISH_TEACHER_VLC_INTERFACE_H

#include "Util.h"
#include <string>
#include "Player.h"
#include <thread>
#include <mutex>

class VLC_interface final : public Player {
 public:
  VLC_interface();
  explicit VLC_interface(const std::string &file_path);

  ~VLC_interface() override;
  VLC_interface(const VLC_interface &orig) = delete;
  void close() override;
  void play_pause() override;
  void play() override;
  void pause() override;
  State get_state() override;
  void seek(const std::chrono::milliseconds &dur) override;
  void seek(const std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> &tp) override;
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> tell() override;

 private:
  mutex dbus_mutex;
  DBusError dbus_error;
  DBusConnection *dbus_conn;
};

#endif //ENGLISH_TEACHER_VLC_INTERFACE_H
