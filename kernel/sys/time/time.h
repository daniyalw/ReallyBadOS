#pragma once

#define CURRENT_YEAR        2021                            // Change this each year!

int century_register = 0x00;                                // Set by ACPI table parsing code if possible

// pm_on means whether it is after 12
// not whether we use 12 or 24 hour clock
bool pm_on = false;

int second;
int minute;
int hour;
int day;
int month;
unsigned int year;
int weekday;
char * weekdays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char * months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

typedef struct {
    int sec, min, h, d, m, y, wd;
    bool pm;
} time_t;

int s = 0;

namespace Time {

void read_rtc();
int get_RTC_register(int reg);
int get_update_in_progress_flag();
time_t get_time();

}
