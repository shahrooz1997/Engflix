//
// Created by shahrooz on 10/29/20.
//

#include "VLC_interface.h"
#include <fstream>
#include <iostream>
#include <cstring>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>
#include <glib.h>
#include <ncurses.h>

using namespace std;
using namespace chrono;

int client_msg(Display *disp, Window win, const char *msg, /* {{{ */
               long data0, long data1,
               long data2, long data3,
               long data4) {
  XEvent event;
  long mask = SubstructureRedirectMask | SubstructureNotifyMask;

  event.xclient.type = ClientMessage;
  event.xclient.serial = 0;
  event.xclient.send_event = True;
  event.xclient.message_type = XInternAtom(disp, msg, False);
  event.xclient.window = win;
  event.xclient.format = 32;
  event.xclient.data.l[0] = data0;
  event.xclient.data.l[1] = data1;
  event.xclient.data.l[2] = data2;
  event.xclient.data.l[3] = data3;
  event.xclient.data.l[4] = data4;

  if (XSendEvent(disp, DefaultRootWindow(disp), False, mask, &event)) {
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Cannot send %s event.\n", msg);
    return EXIT_FAILURE;
  }
}

static gchar *get_property(Display *disp, Window win,
                           Atom xa_prop_type, const gchar *prop_name, unsigned long *size) {
  Atom xa_prop_name;
  Atom xa_ret_type;
  int ret_format;
  unsigned long ret_nitems;
  unsigned long ret_bytes_after;
  unsigned long tmp_size;
  unsigned char *ret_prop;
  gchar *ret;
  xa_prop_name = XInternAtom(disp, prop_name, False);
  if (XGetWindowProperty(disp, win, xa_prop_name, 0, 4096 / 4, False,
                         xa_prop_type, &xa_ret_type, &ret_format,
                         &ret_nitems, &ret_bytes_after, &ret_prop) != Success) {
//    cerr << "Cannot get " << prop_name << " property." << endl;
    return NULL;
  }
  if (xa_ret_type != xa_prop_type) {
//    cerr << "Invalid type of " << prop_name << " property." << endl;
    XFree(ret_prop);
    return NULL;
  }
  /* null terminate the result to make string handling easier */
  tmp_size = (static_cast<unsigned long>(ret_format) / (32 / sizeof(long))) * ret_nitems;
  ret = static_cast<gchar *>(g_malloc(tmp_size + 1));
  memcpy(ret, ret_prop, tmp_size);
  ret[tmp_size] = '\0';
  if (size) {
    *size = tmp_size;
  }
  XFree(ret_prop);
  return ret;
}

gboolean wm_supports(Display *disp, const gchar *prop) {/*{{{*/
  Atom xa_prop = XInternAtom(disp, prop, False);
  Atom *list;
  unsigned long size;

  if (!(list = (Atom *) get_property(disp, DefaultRootWindow(disp),
                                     XA_ATOM, "_NET_SUPPORTED", &size))) {
    return FALSE;
  }

  for (uint i = 0; i < size / sizeof(Atom); i++) {
    if (list[i] == xa_prop) {
      g_free(list);
      return TRUE;
    }
  }

  g_free(list);
  return FALSE;
}

int window_move_resize(Display *disp, Window win, long grav, long x, long y, long w, long h) {
//  signed long grav, x, y, w, h;
  long grflags;
//  const char *argerr = "The -e option expects a list of comma separated integers: \"gravity,X,Y,width,height\"\n";

//  if (!arg || strlen(arg) == 0) {
//    fputs(argerr, stderr);
//    return EXIT_FAILURE;
//  }
//
//  if (sscanf(arg, "%ld,%ld,%ld,%ld,%ld", &grav, &x, &y, &w, &h) != 5) {
//    fputs(argerr, stderr);
//    return EXIT_FAILURE;
//  }

  if (grav < 0) {
    fputs("Value of gravity mustn't be negative. Use zero to use the default gravity of the window.\n", stderr);
    return EXIT_FAILURE;
  }

  grflags = grav;
  if (x != -1) grflags |= (1 << 8);
  if (y != -1) grflags |= (1 << 9);
  if (w != -1) grflags |= (1 << 10);
  if (h != -1) grflags |= (1 << 11);

  if (wm_supports(disp, "_NET_MOVERESIZE_WINDOW")) {
    return client_msg(disp, win, "_NET_MOVERESIZE_WINDOW",
                      grflags, x, y, w, h);
  } else {
    if ((w < 1 || h < 1) && (x >= 0 && y >= 0)) {
      XMoveWindow(disp, win, static_cast<int>(x), static_cast<int>(y));
    } else if ((x < 0 || y < 0) && (w >= 1 && h >= -1)) {
      XResizeWindow(disp, win, static_cast<unsigned int>(w), static_cast<unsigned int>(h));
    } else if (x >= 0 && y >= 0 && w >= 1 && h >= 1) {
      XMoveResizeWindow(disp,
                        win,
                        static_cast<int>(x),
                        static_cast<int>(y),
                        static_cast<unsigned int>(w),
                        static_cast<unsigned int>(h));
    }
    return EXIT_SUCCESS;
  }
}

static gchar *get_window_title(Display *disp, Window win) {
  gchar *title_utf8;
  gchar *wm_name;
  gchar *net_wm_name;
  wm_name = get_property(disp, win, XA_STRING, "WM_NAME", NULL);
  net_wm_name = get_property(disp, win,
                             XInternAtom(disp, "UTF8_STRING", False), "_NET_WM_NAME", NULL);
  if (net_wm_name) {
    title_utf8 = g_strdup(net_wm_name);
  } else {
    if (wm_name) {
      title_utf8 = g_locale_to_utf8(wm_name, -1, NULL, NULL, NULL);
    } else {
      title_utf8 = NULL;
    }
  }
  g_free(wm_name);
  g_free(net_wm_name);
  return title_utf8;
}

Display *wm_manger_init(Window *app_win, Window *vlc_win) {
  Display *disp = nullptr;
  setlocale(LC_ALL, "");
  if (!(disp = XOpenDisplay(NULL))) {
    cerr << "Cannot open display." << endl;
    exit(-1);
  }

  Window *client_list;
  unsigned long client_list_size;
  if ((client_list = (Window *) get_property(disp, DefaultRootWindow(disp),
                                             XA_WINDOW, "_NET_CLIENT_LIST", &client_list_size)) == NULL) {
    if ((client_list = (Window *) get_property(disp, DefaultRootWindow(disp),
                                               XA_CARDINAL, "_WIN_CLIENT_LIST", &client_list_size)) == NULL) {
      cerr << "Cannot get client list properties. \n"
              "(_NET_CLIENT_LIST or _WIN_CLIENT_LIST)"
           << endl;
      XCloseDisplay(disp);
      disp = nullptr;
      exit(-1);
    }
  }

  bool win_set = false;
  for (unsigned long i = 0; i < client_list_size / sizeof(Window); i++) {
    gchar *title = get_window_title(disp, client_list[i]);
    string title_str(title);
    delete[] title;

    if (title_str.find("VLC media player") != string::npos) {
      *vlc_win = client_list[i];
      win_set = true;
    }
  }
  g_free(client_list);
  if (!win_set) {
    cerr << "Window engflix not found" << endl;
    XCloseDisplay(disp);
    disp = nullptr;
    exit(-1);
  }

  char *prop;
  unsigned long size;
  *app_win = (Window) 0;
  prop = get_property(disp, DefaultRootWindow(disp), XA_WINDOW, "_NET_ACTIVE_WINDOW", &size);
  if (prop) {
    *app_win = *((Window *) prop);
    g_free(prop);
  }

  return disp;
}

void keep_the_app_on_top_of_VLC(Display *disp, Window app_win) {
  char *prop;
  unsigned long size;
  Window active_window = (Window) 0;
  prop = get_property(disp, DefaultRootWindow(disp), XA_WINDOW, "_NET_ACTIVE_WINDOW", &size);
  if (prop) {
    active_window = *((Window *) prop);
    g_free(prop);
  }
  // if the focus is on vlc, change it back to the app
  if (string(get_window_title(disp, active_window)).find("VLC media player") != string::npos) {
    XEvent event;
    memset(&event, 0, sizeof(event));
    long mask = SubstructureRedirectMask | SubstructureNotifyMask;
    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = True;
    event.xclient.message_type = XInternAtom(disp, "_NET_ACTIVE_WINDOW", False);
    event.xclient.window = app_win;
    event.xclient.format = 32;

    if (!XSendEvent(disp, DefaultRootWindow(disp), False, mask, &event)) {
      cerr << "Cannot send _NET_ACTIVE_WINDOW event." << endl;
      exit(-1);
    }
    XMapRaised(disp, app_win);
  }
}

void setup_windows(Display *disp, Window app_win, Window vlc_win) {
  int x, y, junkx, junky;
  unsigned int wwidth, wheight, bw, depth;
  Window junkroot;
  XGetGeometry(disp, vlc_win, &junkroot, &junkx, &junky,
               &wwidth, &wheight, &bw, &depth);
  XTranslateCoordinates(disp, vlc_win, junkroot, junkx, junky,
                        &x, &y, &junkroot);

  int old_x = x, old_y = y;
  unsigned int old_wwidth = wwidth, old_wheight = wheight;

//  client_msg(disp, app_win, "_MOTIF_WM_HINTS", 2, 0, 0, 0, 0);
  unsigned long data = 2;
  Atom atom = XInternAtom(disp, "_MOTIF_WM_HINTS", False);
  XChangeProperty(disp, app_win, atom, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char *>(&data), 1);

//  Atom *protocols = new Atom[1];
//  protocols[0] = XInternAtom(disp, "WM_DELETE_WINDOW", False);
//  XSetWMProtocols(disp, app_win, protocols, 1);
//  client_msg(disp, app_win, "WM_DELETE_WINDOW", 0, 0, 0, 0, 0);
//  delete[] protocols;

  window_move_resize(disp, app_win, 0, x + wwidth / 2 - 250, y + wheight - 400, 500, 200);

  while(true) {
    XGetGeometry(disp, vlc_win, &junkroot, &junkx, &junky,
                 &wwidth, &wheight, &bw, &depth);
    XTranslateCoordinates(disp, vlc_win, junkroot, junkx, junky,
                          &x, &y, &junkroot);
    if (x != old_x || y != old_y || wwidth != old_wwidth || wheight != old_wheight) {
//      window_move_resize(disp, app_win, 0, x + wwidth / 2 - 250, static_cast<long>(1 * wheight) - 250, 500, 200);
      window_move_resize(disp, app_win, 0, x + wwidth / 2 - 250, y + wheight - 400, 500, 200);
      old_x = x;
      old_y = y;
      old_wwidth = wwidth;
      old_wheight = wheight;
    }
    keep_the_app_on_top_of_VLC(disp, app_win);
    this_thread::sleep_for(milliseconds (500));
  }
}

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

VLC_interface::VLC_interface(const string &file_path) : dbus_conn(nullptr) {
  thread(execute_vlc, file_path).detach();
  disp = wm_manger_init(&app_win, &vlc_win);
  thread(setup_windows, disp, app_win, vlc_win).detach();
  lock_guard<mutex> lock(dbus_mutex);
  DBusError dbus_error;
  dbus_error_init(&dbus_error);
  if (nullptr == (dbus_conn = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error))) {
    perror(dbus_error.name);
    perror(dbus_error.message);
    exit(-1);
  }
}

VLC_interface::VLC_interface() {
  disp = wm_manger_init(&app_win, &vlc_win);
  thread(setup_windows, disp, app_win, vlc_win).detach();
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
  if (disp != nullptr) {
    XCloseDisplay(disp);
    disp = nullptr;
  }
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


