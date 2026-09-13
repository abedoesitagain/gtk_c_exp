#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

int fill(char ch[], int t[]){
    for(int i=0;i<(int)strlen(ch);i++){
        char c = ch[i];
        t[i]=atoi(&c);
    }
    return 0;
}
int checksp(int t[], int len){
    int res;
    for(int i=0;i<len;i+=4){
        int s=t[i]*t[i+2] + t[i+1]*t[i+3];
        int j=1;
        while (j*j<s){
            j++;
        }
        res = j*j == s;
        printf("stuff: %d\n", res);
        printf("s: %d\n", s);
    }
    return res;
}
int check(int len){
    return len >= 4;
}
int start(char ch[], int len){
    int t[100];
    fill(ch, t);
    return checksp(t, len);
}
void on_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    GtkLabel *output = GTK_LABEL(g_object_get_data(G_OBJECT(button), "output-label"));
    GtkBox *box = GTK_BOX(g_object_get_data(G_OBJECT(button), "box"));
    
    const char *ch = gtk_editable_get_text(GTK_EDITABLE(entry));

    // char ch[255];
    // *ch = *text;
    //printf("%s, %s\n", ch, text);
    int len=(int)strlen(ch);
    if(check(len)){
        if(start(ch, len)){
            gtk_label_set_text(output, "valid and special");
        }else{
            gtk_label_set_text(output, "valid and not special");
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
int main(int argc, char **argv){
    GtkApplication *app = gtk_application_new("tn.stuff.abes", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
