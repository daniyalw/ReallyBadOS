#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

typedef struct datetime {
    uint8_t century;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} datetime_t;

datetime_t current_datetime;

char * weekday_map[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char * month_map[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
/*
 * Check if rtc is updating time currently
 * */
int is_updating_rtc() {
    outb(0x70, 0x0A);
    uint32_t status = inb(0x71);
    return (status & 0x80);
}

/*
 * Get the value of a specific rtc register
 * */
uint8_t get_rtc_register(int reg_num) {
    outb(CMOS_ADDR, reg_num);
    return inb(CMOS_DATA);
}

int is_leap_year(int year, int month) {
    if(year % 4 == 0 && (month == 1 || month == 2)) return 1;
    return 0;
}
/*
 * Read current date and time from rtc, store in global var current_datetime
 * */
void rtc_read_datetime() {
    // Wait until rtc is not updating
    while(is_updating_rtc());

    current_datetime.second = get_rtc_register(0x00);
    current_datetime.minute = get_rtc_register(0x02);
    current_datetime.hour = get_rtc_register(0x04);
    current_datetime.day = get_rtc_register(0x07);
    current_datetime.month = get_rtc_register(0x08);
    current_datetime.year = get_rtc_register(0x09);

    uint8_t registerB = get_rtc_register(0x0B);

    // Convert BCD to binary values if necessary
    if (!(registerB & 0x04)) {
        current_datetime.second = (current_datetime.second & 0x0F) + ((current_datetime.second / 16) * 10);
        current_datetime.minute = (current_datetime.minute & 0x0F) + ((current_datetime.minute / 16) * 10);
        current_datetime.hour = ( (current_datetime.hour & 0x0F) + (((current_datetime.hour & 0x70) / 16) * 10) ) | (current_datetime.hour & 0x80);
        current_datetime.day = (current_datetime.day & 0x0F) + ((current_datetime.day / 16) * 10);
        current_datetime.month = (current_datetime.month & 0x0F) + ((current_datetime.month / 16) * 10);
        current_datetime.year = (current_datetime.year & 0x0F) + ((current_datetime.year / 16) * 10);
    }
}

int get_weekday_from_date(datetime_t dt) {
    char month_code_array[] = {0x0,0x3, 0x3, 0x6, 0x1, 0x4, 0x6, 0x2, 0x5, 0x0, 0x3, 0x5};
    char century_code_array[] = {0x4, 0x2, 0x0, 0x6, 0x4, 0x2, 0x0};    // Starting from 18 century

    // Simple fix...
    dt.century = 21;

    // Calculate year code
    int year_code = (dt.year + (dt.year / 4)) % 7;
    int month_code = month_code_array[dt.month - 1];
    int century_code = century_code_array[dt.century - 1 - 17];
    int leap_year_code = is_leap_year(dt.year, dt.month);

    int ret = (year_code + month_code + century_code + dt.day - leap_year_code) % 7;
    return ret;
}
