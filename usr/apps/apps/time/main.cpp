#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    time_t time = get_time();

    // print time
    if (time.min >= 10)
        printf("%d:%d %s\n", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));
    else
        printf("%d:0%d %s\n", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));

    char *months[] = {
        "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
    };

    char * weekdays[] = {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    };

    // print date
    printf("%s, %s %d\n", weekdays[time.wd-1], months[time.m-1], time.d);

    return 0;
}
