#include "utfcpp-4.0.6/source/utf8/checked.h"
#include <asm-generic/ioctls.h>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <vector>
using namespace std;

vector<string> split(string a, char delim) {
  vector<string> result;
  string left = a;

  while (left.find(delim) != std::string::npos) {
    int index = left.find_first_of(delim);
    result.push_back(left.substr(0, index));
    left = left.substr(index + 1, left.length());
  }

  result.push_back(left);

  return result;
}

string center_x(string a, int w) {
  vector<string> lines = split(a, '\n');
  string result;

  int len = utf8::distance(lines[0].begin(), lines[0].end());
  int padding = (w - len) / 2;
  if (padding <= 0) {
    return result;
  }
  string padstr = string(padding, ' ');
  for (string line : lines) {
    result.append(padstr);
    result.append(line);
    result.append("\n");
  }

  return result;
}

string center_y(string a, int h, bool fill) {
  vector<string> lines = split(a, '\n');
  string result;
  int padding = (h - lines.size()) / 2 + 1;
  if (padding <= 0) {
    return result;
  }
  result.append(string(padding, '\n'));
  result.append(a);

  if (fill) {
    result.append(string(padding, '\n'));
  }

  return result;
}

void clear_screen() { std::cout << "\033[2J\033[1;1H"; }

string format_options(vector<tuple<string, string, string, string>> options) {
  string result;
  int break_c = 0;

  for (tuple<string, string, string, string> el : options) {
    string break_pad;
    break_c++;
    if (break_c == 2) {
      break_pad = "\n";
      break_c = 0;
    } else {
      break_pad = "\t";
    }
    result.append(fmt::format("{} {} [:{}]{}", get<1>(el), get<0>(el),
                              get<2>(el), break_pad));
  }

  return result;
}

int main() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  string ascii_art =
      "██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n██║    "
      "██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n██║ █╗ "
      "██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗  "
      "\n██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝  "
      "\n╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n "
      "╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝";

  string username = getenv("USER");
  string pwd = std::filesystem::current_path().string();
  pwd.replace(0, strlen(getenv("HOME")), "~");
  string subtitle = fmt::format("  {}     {}", username, pwd);
  vector<tuple<string, string, string, string>> options = {
      make_tuple("Neovim", " ", "nv", "nvim"),
      make_tuple("Fastfetch", " ", "ft", "fastfetch"),
      make_tuple("Bash", " ", "bs", "bash"),
      make_tuple("Btop", " ", "bp", "btop"),
  };
  string exit_directory;

  std::cout << "\033[33m";
  string screen = fmt::format("{}\n{}\n{}", center_x(ascii_art, w.ws_col),
                              center_x(subtitle, w.ws_col),
                              center_x(format_options(options), w.ws_col));
  std::cout << center_y(screen, w.ws_row, true);
  std::cout << "\033[0m";

  while (true) {
    std::cout << ":";
    string i;
    cin >> i;

    if (i == "h") {
      clear_screen();
      string msg = "Salut (\033[32mfrench: Hi; /saly/\033[0m) is a terminal "
                   "greeter application\n"
                   "It provides a clean welome screen when launched and let's "
                   "you quickly launch\n"
                   "your most important applications.\n\n"
                   "\033[31mClose this message with\033[0m \t:main\n"
                   "\033[31mOpen this message with\033[0m \t:h\n"
                   "\033[31mQuit with\033[0m \t\t\t:q\n";
      std::cout << center_y(center_x(msg, w.ws_col), w.ws_row, true);
    } else if (i == "q") {
      clear_screen();
      exit(0);
    } else if (i == "main") {
      std::cout << "\033[33m";
      std::cout << center_y(screen, w.ws_row, true);
      std::cout << "\033[0m";
    } else {
      for (tuple<string, string, string, string> el : options) {
        if (get<2>(el) == i) {
          clear_screen();
			 std::system("clear");
			 std::this_thread::sleep_for(chrono::milliseconds(200));
          string v = get<3>(el);
          execlp(v.c_str(), NULL);
        }
      }
    }
  }
}
