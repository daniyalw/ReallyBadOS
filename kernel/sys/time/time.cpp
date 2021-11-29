#pragma once

namespace Time {

int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}

int get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}

void read_rtc() {
    Kernel::system_log("Read RTC.\n");
      int century;
      int last_second;
      int last_minute;
      int last_hour;
      int last_day;
      int last_month;
      int last_year;
      int last_century;
      int registerB;
      int last_wkday;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag());                // Make sure an update isn't in progress
      second = (int)get_RTC_register(0x00);
      minute = (int)get_RTC_register(0x02);
      hour = (int)get_RTC_register(0x04);
      day = (int)get_RTC_register(0x07);
      weekday = (int)get_RTC_register(0x06);
      month = (int)get_RTC_register(0x08);
      year = (int)get_RTC_register(0x09);
      if(century_register != 0) {
            century = (int)get_RTC_register(century_register);
      }

      do {
            last_second = second;
            last_minute = minute;
            last_hour = hour;
            last_day = day;
            last_month = month;
            last_year = year;
            last_wkday = weekday;
            last_century = century;

            while (get_update_in_progress_flag());

            second = (int)get_RTC_register(0x00);
            minute = (int)get_RTC_register(0x02);
            hour = (int)get_RTC_register(0x04);
            day = (int)get_RTC_register(0x07);
            month = (int)get_RTC_register(0x08);
            weekday = (int)get_RTC_register(0x06);
            year = (int)get_RTC_register(0x09);

            if (century_register != 0) {
                  century = (int)get_RTC_register(century_register);
            }

      } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
               (last_day != day) || (last_month != month) || (last_year != year) ||
               (last_century != century) );

      registerB = (int)get_RTC_register(0x0B);

      if (!(registerB & 0x04)) {
            second = (second & 0x0F) + ((second / 16) * 10);
            minute = (minute & 0x0F) + ((minute / 16) * 10);
            hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
            day = (day & 0x0F) + ((day / 16) * 10);
            weekday = (weekday & 0x0F) + ((weekday / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);
            if(century_register != 0) {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }

      if(century_register != 0) {
            year += century * 100;
      } else {
            year += (CURRENT_YEAR / 100) * 100;
            if(year < CURRENT_YEAR) year += 100;
      }
    seconds = second;
}

}
