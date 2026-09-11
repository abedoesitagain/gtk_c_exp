#include "glib-object.h"
#include "glib.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>

static GtkWidget *label[8] = { NULL };

int digit_sum(int n){
    char buf[10];
    snprintf(buf, sizeof(buf), "%d", n);
    int length=(int)strlen(buf);
    int sum=0;
    for (int i=0; i<length; i++) {
        sum += buf[i] - '0';
    }
    return sum; 
}

int prime_factor(int n) {
    int sum = 0;

    if (n < 0) {
        n = -n;
    }

    // Factor n and add the digits of each prime factor
    for (int factor = 2; factor <= n / factor; factor++) {
        while (n % factor == 0) {
            int prime = factor;

            while (prime > 0) {
                sum += prime % 10;
                prime /= 10;
            }

            n /= factor;
        }
    }

    // Remaining prime factor
    if (n > 1) {
        int prime = n;

        while (prime > 0) {
            sum += prime % 10;
            prime /= 10;
        }
    }

    // Keep adding digits until only one digit remains
    while (sum >= 10) {
        int digit_sum = 0;

        while (sum > 0) {
            digit_sum += sum % 10;
            sum /= 10;
        }

        sum = digit_sum;
    }

    printf("%d", sum);
    return sum;
}

int is_smith(int n){
    if (digit_sum(n)  == prime_factor(n)){
        printf("%d is smith", n);
        return 1;
    }else{
        printf("%d is not smith", n);
        return 0;
    }
}

int only_digits(const char *text) {
    if (*text == '\0') {
        return 0;
    }

    for (; *text != '\0'; text++) {
        if (!isdigit((unsigned char)*text)) {
            return 0;
        }
    }

    return 1;
}

void on_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    GtkLabel *output = GTK_LABEL(g_object_get_data(G_OBJECT(button), "output-label"));
    GtkBox *box = GTK_BOX(g_object_get_data(G_OBJECT(button), "box"));
    char tempname[10];
    
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));
    if (only_digits(text)){
        if(is_smith(atoi(text)) ){
            gtk_label_set_text(output, "valid and smtih");
        }else{
            gtk_label_set_text(output, "valid and not smith");
        }
    }else{
        gtk_label_set_text(output, "not valid");
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;

    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_window_set_child(GTK_WINDOW(window), box);

    GtkWidget *entry = gtk_entry_new();
    GtkWidget *button = gtk_button_new_with_label("Check");
    GtkWidget *out_label = gtk_label_new("");

    gtk_box_append(GTK_BOX(box), entry);
    gtk_box_append(GTK_BOX(box), button);
    gtk_box_append(GTK_BOX(box), out_label);

    g_object_set_data(G_OBJECT(button), "output-label", out_label);
    g_object_set_data(G_OBJECT(button), "box", box);
    g_signal_connect(button, "clicked", G_CALLBACK(on_clicked), entry);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("tn.stuff.abes", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
