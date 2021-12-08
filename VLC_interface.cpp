//
// Created by shahrooz on 10/29/20.
//

#include "VLC_interface.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;
using namespace chrono;

void execute_vlc(const string file_path) {
  if (execute("vlc", vector<string>{file_path}) == 127) {
    cerr << "You need to install VLC by running the following command:\n" << "sudo snap install vlc" << endl;
    exit(-1);
  }
}

DBusMessage *construct_dbus_message(const string &iface, const string &method, const vector<string> &args = {}) {
  DBusMessage *dbus_msg = dbus_message_new_method_call("org.mpris.MediaPlayer2.vlc",
                                                       "/org/mpris/MediaPlayer2",
                                                       iface.c_str(),
                                                       method.c_str());
  if (dbus_msg == nullptr) {
    perror("ERROR: ::dbus_message_new_method_call - Unable to allocate memory for the message!");
    exit(-2);
  }
  if (!args.empty()) {
    DBusMessageIter iter;
    dbus_message_iter_init_append(dbus_msg, &iter);
    for (auto &arg: args) {
      size_t colon_pos = arg.find(':');
      if (colon_pos == string::npos) {
        cerr << "Bad argument input: " << arg << endl;
        exit(-1);
      }
      string type = arg.substr(0, colon_pos);
      string value = arg.substr(colon_pos + 1);
      char *value_string = new char[value.size() + 1];
      strcpy(value_string, value.c_str());

      if (type == "string") {
        dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &value_string);
      } else if (type == "int64") {
        int64_t value_ll = strtoll(value_string, nullptr, 10);
        dbus_message_iter_append_basic(&iter, DBUS_TYPE_INT64, &value_ll);
      } else {
        cerr << "Argument type not supported: " << type << endl;
        exit(-1);
      }
      delete[] value_string;
    }
    dbus_message_iter_init_closed(&iter);
  }
  return dbus_msg;
}

VLC_interface::VLC_interface(const string &file_path) : dbus_conn(nullptr) {
  thread(execute_vlc, file_path).detach();
  lock_guard<mutex> lock(dbus_mutex);
  DBusError dbus_error;
  dbus_error_init(&dbus_error);
  if (nullptr == (dbus_conn = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error))) {
    perror(dbus_error.name);
    perror(dbus_error.message);
    exit(-1);
  }
}

bool dbus_call_without_reply(DBusConnection *dbus_conn,
                             const string &iface,
                             const string &method,
                             const vector<string> &args = {}) {
  DBusError dbus_error;
  dbus_error_init(&dbus_error);
  DBusMessage *dbus_msg = construct_dbus_message(iface, method, args);
  DBusMessage *dbus_reply =
      dbus_connection_send_with_reply_and_block(dbus_conn, dbus_msg, DBUS_TIMEOUT_USE_DEFAULT, &dbus_error);
  if (dbus_reply == nullptr) {
    perror(dbus_error.name);
    perror(dbus_error.message);
    dbus_message_unref(dbus_msg);
    dbus_message_unref(dbus_reply);
    return false;
  }
  dbus_message_unref(dbus_msg);
  dbus_message_unref(dbus_reply);
  return true;
}

VLC_interface::VLC_interface() {
  lock_guard<mutex> lock(dbus_mutex);
  DBusError dbus_error;
  dbus_error_init(&dbus_error);
  if (nullptr == (dbus_conn = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error))) {
    perror(dbus_error.name);
    perror(dbus_error.message);
    exit(-1);
  }
}

VLC_interface::~VLC_interface() {
  dbus_connection_unref(dbus_conn);
  // Todo: close vlc only if it is open
  this->close();
}

void VLC_interface::close() {
  lock_guard<mutex> lock(dbus_mutex);
  assert(dbus_call_without_reply(dbus_conn, "org.mpris.MediaPlayer2", "Quit"));
}

void VLC_interface::play_pause() {
  lock_guard<mutex> lock(dbus_mutex);
  assert(dbus_call_without_reply(dbus_conn, "org.mpris.MediaPlayer2.Player", "PlayPause"));
}

void VLC_interface::play() {
  lock_guard<mutex> lock(dbus_mutex);
  assert(dbus_call_without_reply(dbus_conn, "org.mpris.MediaPlayer2.Player", "Play"));
}

void VLC_interface::pause() {
  lock_guard<mutex> lock(dbus_mutex);
  assert(dbus_call_without_reply(dbus_conn, "org.mpris.MediaPlayer2.Player", "Pause"));
}

void VLC_interface::seek(const milliseconds &dur) {
  lock_guard<mutex> lock(dbus_mutex);
  vector<string> args = {"afefaef"};
  assert(dbus_call_without_reply(dbus_conn, "org.mpris.MediaPlayer2.Player", "Seek", {
  string("int64:") + to_string(microseconds(dur).count())
}
));
}

// Todo: set position instead of the following function
void VLC_interface::seek(const time_point<steady_clock, milliseconds> &tp) {
  time_point<steady_clock, milliseconds> pos = tell();
  seek(tp - pos);
}

time_point<steady_clock, milliseconds> VLC_interface::tell() {
  lock_guard<mutex> lock(dbus_mutex);
  DBusError dbus_error;
  dbus_error_init(&dbus_error);
  DBusMessage *dbus_msg = construct_dbus_message("org.freedesktop.DBus.Properties",
                                                 "Get",
                                                 {"string:org.mpris.MediaPlayer2.Player", "string:Position"});
  DBusMessage *dbus_reply =
      dbus_connection_send_with_reply_and_block(dbus_conn, dbus_msg, DBUS_TIMEOUT_USE_DEFAULT, &dbus_error);
  if (dbus_reply == nullptr) {
    perror(dbus_error.name);
    perror(dbus_error.message);
    dbus_message_unref(dbus_msg);
    dbus_message_unref(dbus_reply);
    assert(false);
  }
  dbus_message_unref(dbus_msg);
  DBusMessageIter iter;
  DBusMessageIter container_iter;
  dbus_message_iter_init(dbus_reply, &iter);
  dbus_message_iter_recurse(&iter, &container_iter);
  int64_t position;
  dbus_message_iter_get_basic(&container_iter, &position);

  dbus_message_unref(dbus_reply);
  return time_point<steady_clock, milliseconds>(milliseconds(position / 1000));
}

State VLC_interface::get_state() {
  lock_guard<mutex> lock(dbus_mutex);
  DBusError dbus_error;
  dbus_error_init(&dbus_error);
  DBusMessage *dbus_msg = construct_dbus_message("org.freedesktop.DBus.Properties",
                                                 "Get",
                                                 {"string:org.mpris.MediaPlayer2.Player", "string:PlaybackStatus"});
  DBusMessage *dbus_reply =
      dbus_connection_send_with_reply_and_block(dbus_conn, dbus_msg, DBUS_TIMEOUT_USE_DEFAULT, &dbus_error);
  if (dbus_reply == nullptr) {
    perror(dbus_error.name);
    perror(dbus_error.message);
    dbus_message_unref(dbus_msg);
    dbus_message_unref(dbus_reply);
    assert(false);
  }
  dbus_message_unref(dbus_msg);
  DBusMessageIter iter;
  DBusMessageIter container_iter;
  dbus_message_iter_init(dbus_reply, &iter);
  dbus_message_iter_recurse(&iter, &container_iter);
  char *status = nullptr;
  dbus_message_iter_get_basic(&container_iter, &status);
  string temp(status);
  State ret;
  if (temp == "Paused") {
    ret = Paused;
  } else if (temp == "Stopped") {
    ret = Stopped;
  } else if (temp == "Playing") {
    ret = Playing;
  } else {
    assert(false);
  }
  dbus_message_unref(dbus_reply);
  return ret;
}


