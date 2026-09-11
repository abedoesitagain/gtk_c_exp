#include "glib-object.h"
#include "glib.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <time.h>

static GtkWidget *label[8] = { NULL };

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
int test(const char *string){
    if (strlen(string) != 8){
        return 0;
    }else{
        return only_digits(string);
    }
}

void genrand(int arr[8], GtkWidget* label[8], GtkBox* box){
    srand(time(0));
    
    for(int i=0;i<8;i++){
        int randomNumber = 10000000 + rand() % 90000000;
        arr[i]=randomNumber;
        char prebuf[100];
        snprintf(prebuf, sizeof(prebuf), "%d",randomNumber);
        const char *buf = prebuf;
        printf("%s\n", buf);
        label[i] = gtk_label_new(buf);
        gtk_box_append(GTK_BOX(box), label[i]);
    }
}

void cleanrand(int arr[8], GtkWidget* label[8], GtkBox* box){
    for(int i=0;i<8;i++){
        if (label[i] != NULL){
            gtk_box_remove(GTK_BOX(box), label[i]);
            label[i] = NULL;
        }
    }
}

void on_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    GtkLabel *output = GTK_LABEL(g_object_get_data(G_OBJECT(button), "output-label"));
    GtkBox *box = GTK_BOX(g_object_get_data(G_OBJECT(button), "box"));
    int *ft = g_object_get_data(G_OBJECT(button), "ft");
    printf("%d\n", *ft);
    char tempname[10];
    
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));
    if (test(text)){
        gtk_label_set_text(output, "valid");
        int randels[8];
        // GtkWidget* label[8];
        if (*ft == 0){
            cleanrand(randels, label, box);
        }
        genrand(randels, label, box);
        *ft = 0;
        for(int i=0;i<8;i++){
            char tempname[10];
            snprintf(tempname, sizeof(tempname), "label-%d", i);
            g_object_set_data(G_OBJECT(button), tempname, label[i]);
        }
        for(int i=0;i<8;i++){
            const char textc = *text;
            const char *otherc = gtk_label_get_text(GTK_LABEL(label[i]));
            printf("comparing %s %s\n", text, otherc);
            if (strcmp(text, otherc) == 0){
                printf("yes\n");
            }else{
                printf("no\n");
            }
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
    int *ft = g_new(int, 1);
    *ft = 1;
    g_object_set_data(G_OBJECT(button), "output-label", out_label);
    g_object_set_data(G_OBJECT(button), "box", box);
    g_object_set_data(G_OBJECT(button), "ft", ft);
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
