#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <App.hpp>

extern int getBootDevice(void);
extern void alterPin(int pin);
extern void showChipInfo();
extern void fillBuffer(const char *message);

extern void sendHtmlTemplate(const char *htmlTemplate, const char *(*setupProcessor)(const char *));

#endif
