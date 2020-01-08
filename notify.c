// gcc -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gdk-pixbuf-2.0 `pkg-config –cflags –libs gtk+-3.0` notify.c -o notify -l notify
// /sys/class/power_supply/BAT0/capacity
// /sys/class/power_supply/BAT0/status

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <libnotify/notify.h>

char* get_file_contents(char* path);
bool is_charging();
int charge_percentage();
void send_notification();

int main(int argc, char **argv)    {

    for(;;) {
        if ((is_charging() == false && charge_percentage() <= 10 )) {
            send_notification();
        }
    sleep(120);
}

return 0;

}

bool is_charging() {

char* path = "/sys/class/power_supply/BAT0/status";
char* status = get_file_contents(path);

    if (strstr(status, "Charging") != NULL) {
        return true;
     } else {
        return false;
     }
}

int charge_percentage() {

    char* path = "/sys/class/power_supply/BAT0/capacity";
    char* percentage = get_file_contents(path);
    return atoi(percentage);

}

char* get_file_contents(char* path) {

    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;

    return string;

}

void send_notification() {

    char name[11] = "Low Battery";
    char body[50];
    sprintf(body, "Your battery is on %d%%. Plug your computer in.", charge_percentage());

    notify_init(name);
    NotifyNotification *notification;
    notification = notify_notification_new(name, body, NULL);

    notify_notification_set_timeout(notification, 5000);
    notify_notification_set_urgency(notification, NOTIFY_URGENCY_CRITICAL);

    GError *error = NULL;
    notify_notification_show(notification, &error);

}
